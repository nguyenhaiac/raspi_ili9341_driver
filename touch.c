#include "gpiod.h"
#include "stdio.h"
#include "stdint.h"
#include "touch.h"
#include "gpio.h"
static struct gpiod_line *tp_clk, *tp_cs, *tp_miso, *tp_mosi, *tp_irq;

void tp_gpio_init() {
  get_gpio_line(&tp_clk, TP_CLK_PIN);
  get_gpio_line(&tp_cs, TP_CS_PIN);
  get_gpio_line(&tp_miso, TP_MISO_PIN);
  get_gpio_line(&tp_mosi, TP_MOSI_PIN);
  get_gpio_line(&tp_irq, TP_IRQ_PIN);
  if (gpiod_line_request_output(tp_clk, "touch", 0) < 0) {
    perror("Cannot get clk line");
    return;
  }
  if (gpiod_line_request_output(tp_mosi, "touch", 0) < 0) {
    perror("Cannot get tp_mosi line");
    return;
  }
  if (gpiod_line_request_output(tp_cs, "touch", 0) < 0) {
    perror("Cannot get tp_cs line");
    return;
  }
  if (gpiod_line_request_input(tp_irq, "touch") < 0) {
    perror("cannot request tp_irq line");
  }

  if (gpiod_line_request_input(tp_miso, "touch") < 0) {
    perror("cannot request tp_miso line");
  }
}

uint16_t TP_Read(void) {
  uint8_t i = 16;
  uint16_t value = 0;

  while (i > 0x00) {
    value <<= 1;

    set_gpio(tp_clk, 1);
    set_gpio(tp_clk, 0);
    if (gpiod_line_get_value(tp_miso) != 0) {
      value++;
    }

    i--;
  };

  return value;
}

void TP_Write(uint8_t value) {
  uint8_t i = 0x08;
  set_gpio(tp_clk, 0);

  while (i > 0) {
    if ((value & 0x80) != 0x00) {
      set_gpio(tp_mosi, 1);
    } else {
      set_gpio(tp_mosi, 0);
    }

    value <<= 1;
    set_gpio(tp_clk, 1);
    set_gpio(tp_clk, 0);
    i--;
  };
}

uint8_t TP_Read_Coordinates(uint16_t Coordinates[2]) {
  set_gpio(tp_clk, 1);
  set_gpio(tp_mosi, 1);
  set_gpio(tp_cs, 1);

  uint32_t avg_x, avg_y = 0;
  uint16_t rawx, rawy = 0;
  uint32_t calculating_x, calculating_y = 0;

  uint32_t samples = NO_OF_POSITION_SAMPLES;
  uint32_t counted_samples = 0;

  set_gpio(tp_cs, 0);

  while ((samples > 0) && (gpiod_line_get_value(tp_irq) == 0)) {
    TP_Write(CMD_RDY);

    rawy = TP_Read();
    avg_y += rawy;
    calculating_y += rawy;

    TP_Write(CMD_RDX);
    rawx = TP_Read();
    avg_x += rawx;
    calculating_x += rawx;
    samples--;
    counted_samples++;
  };

  set_gpio(tp_cs, 1);

  if ((counted_samples == NO_OF_POSITION_SAMPLES) &&
      (gpiod_line_get_value(tp_irq) == 0)) {

    calculating_x /= counted_samples;
    calculating_y /= counted_samples;

    rawx = calculating_x;
    rawy = calculating_y;

    rawx *= -1;
    rawy *= -1;

    // CONVERTING 16bit Value to Screen coordinates
    // 65535/273 = 240!
    // 65535/204 = 320!
    Coordinates[0] = ((240 - (rawx / X_TRANSLATION)) - X_OFFSET) * X_MAGNITUDE;
    Coordinates[1] = ((rawy / Y_TRANSLATION) - Y_OFFSET) * Y_MAGNITUDE;

    return TOUCHPAD_DATA_OK;
  } else {
    Coordinates[0] = 0;
    Coordinates[1] = 0;
    return TOUCHPAD_DATA_NOISY;
  }
}

uint8_t TP_Touchpad_Pressed(void) {
  if (gpiod_line_get_value(tp_irq) == 0) {
    return TOUCHPAD_PRESSED;
  } else {
    return TOUCHPAD_NOT_PRESSED;
  }
}
