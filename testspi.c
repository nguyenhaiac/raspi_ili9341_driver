#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <gpiod.h>
#include <string.h>

#define SPI_DEV     "/dev/spidev0.0"
#define DC_PIN      22   // BCM GPIO for D/C
#define RESET_PIN   27   // BCM GPIO for RESET
#define CS_PIN      25   // BCM GPIO for RESET

// Set a GPIO line (0 or 1)
void set_gpio(struct gpiod_line *line, int value) {
    if (gpiod_line_set_value(line, value) < 0) {
        perror("gpiod_line_set_value");
        exit(1);
    }
}

// Simple SPI transfer
void spi_transfer(int fd, uint8_t *tx, uint8_t *rx, size_t len, uint32_t speed) {
    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long)tx,
        .rx_buf = (unsigned long)rx,
        .len = len,
        .speed_hz = speed,
        .bits_per_word = 8,
    };
    if (ioctl(fd, SPI_IOC_MESSAGE(1), &tr) < 1) {
        perror("SPI transfer failed");
        exit(1);
    }
}

int main() {
    int fd;
    struct gpiod_chip *chip;
    struct gpiod_line *dc, *reset, *cs;

    // --- Open SPI ---
    fd = open(SPI_DEV, O_RDWR);
    if (fd < 0) { perror("open SPI"); return 1; }

    uint8_t mode = SPI_MODE_0;
    uint32_t speed = 100000; // 1 MHz
    ioctl(fd, SPI_IOC_WR_MODE, &mode);
    ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);

    // --- Setup GPIO (D/C and RESET) ---
    chip = gpiod_chip_open_by_name("gpiochip0");
    if (!chip) { perror("gpiod_chip_open_by_name"); return 1; }

    dc = gpiod_chip_get_line(chip, DC_PIN);
    reset = gpiod_chip_get_line(chip, RESET_PIN);
    cs = gpiod_chip_get_line(chip, CS_PIN);
    if (!dc || !reset || !cs) { perror("get line"); return 1; }

    if (gpiod_line_request_output(dc, "ili9341", 0) < 0) { perror("dc request"); return 1; }
    if (gpiod_line_request_output(reset, "ili9341", 1) < 0) { perror("reset request"); return 1; }
    if (gpiod_line_request_output(cs, "ili9341", 0) < 0) { perror("cs request"); return 1; }

    // --- Hardware reset ---
    set_gpio(reset, 0);
    usleep(20000); // 20 ms
    set_gpio(reset, 1);
    usleep(150000); // 150 ms

    // --- Send RDDID command (0x04) ---
    uint8_t cmd = 0x06;
    uint8_t rx[4] = {0};
    uint8_t dummy[4] = {0,0,0,0};
    set_gpio(cs,1);
    usleep(20);
    // DC low = command
    set_gpio(cs, 0);
    set_gpio(dc, 0);
    spi_transfer(fd, &cmd, NULL, 1, speed);

    // DC high = data
    set_gpio(dc, 1);
    spi_transfer(fd, dummy, rx, 3, speed);
    set_gpio(cs,1);
    printf("RDDID response: %02X %02X %02X\n", rx[0], rx[1], rx[2]);

    // --- Cleanup ---
    gpiod_line_release(dc);
    gpiod_line_release(reset);
    gpiod_chip_close(chip);
    close(fd);
    return 0;
}

