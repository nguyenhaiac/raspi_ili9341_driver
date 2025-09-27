LVGL_DIR ?= $(shell pwd)

LVGL_SRC := $(shell find $(LVGL_DIR)/lvgl/src -name '*.c')

SRC = main.c ili9341.c lvgl_connect.c $(LVGL_SRC)

INCLUDES = -I$(LVGL_DIR)/lvgl \
           -I$(LVGL_DIR)/lvgl/src \
           -I$(LVGL_DIR)/lvgl/src/core \
           -I$(LVGL_DIR)/lvgl/src/widgets \
           -I$(LVGL_DIR)/lvgl/src/hal \
           -I$(LVGL_DIR)/lvgl/src/misc \
           -I$(LVGL_DIR)/lvgl/src/draw

CFLAGS = -lgpiod -g

build:
	gcc $(SRC) $(CFLAGS) $(INCLUDES) -o app
