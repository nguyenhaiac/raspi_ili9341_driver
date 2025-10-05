#include "gpiod.h"
#include "stdio.h"
#include "gpio.h"
#include "fcntl.h"
#include "stdlib.h"


static struct gpiod_chip *gpiochip;

void get_gpio_chip(){
    gpiochip = gpiod_chip_open_by_name(CHIP_NAME);
    if(!gpiochip){
        perror("Cannot get gpio chip");
        return;
    }
}

void get_gpio_line(struct gpiod_line **line, int gpio_pin){
    *line = gpiod_chip_get_line(gpiochip, gpio_pin);
    if(!*line){
        perror("Cannot get line");
        return;
    }
}

void set_gpio(struct gpiod_line *line, int value){
    if(gpiod_line_set_value(line,value) <0){
        perror("gpio_line_set_value");
        exit(1);
    }
}
