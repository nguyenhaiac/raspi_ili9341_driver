#ifndef GPIO_H_
#define GPIO_H_

#define CHIP_NAME "gpiochip0"

void get_gpio_chip();

void get_gpio_line(struct gpiod_line **line, int gpio_pin);

void set_gpio(struct gpiod_line *line, int value);
#endif // GPIO_H_
