#ifndef TOUCH_H_
#define TOUCH_H_

#define TP_CLK_PIN 16
#define TP_CS_PIN 26
#define TP_MISO_PIN 6
#define TP_MOSI_PIN 5
#define TP_IRQ_PIN 13

#define CMD_RDY 0X90
#define CMD_RDX 0XD0

// RETURN VALUES FOR TP_Touchpad_Pressed
#define TOUCHPAD_NOT_PRESSED 0
#define TOUCHPAD_PRESSED 1

// RETURN VALUES FOR TP_Read_Coordinates
#define TOUCHPAD_DATA_OK 1
#define TOUCHPAD_DATA_NOISY 0

// HARDCODED CALIBRATION, CHANGE IF REQUIRED
#define X_OFFSET 13
#define Y_OFFSET 15
#define X_MAGNITUDE 1.16
#define Y_MAGNITUDE 1.16
#define NO_OF_POSITION_SAMPLES 1000
// CONVERTING 16bit Value to Screen coordinates
//  65535/273 = 240!
//  65535/204 = 320!
#define X_TRANSLATION 273
#define Y_TRANSLATION 204
void tp_gpio_init();
uint16_t TP_Read(void);

// Internal Touchpad command, do not call directly
void TP_Write(uint8_t value);

// Read coordinates of touchscreen press. Position[0] = X, Position[1] = Y
uint8_t TP_Read_Coordinates(uint16_t Coordinates[2]);

// Check if Touchpad was pressed. Returns TOUCHPAD_PRESSED (1) or
// TOUCHPAD_NOT_PRESSED (0)
uint8_t TP_Touchpad_Pressed(void);
#endif // TOUCH_H_
