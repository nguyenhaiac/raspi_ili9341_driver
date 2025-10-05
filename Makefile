LVGL_DIR ?= $(shell pwd)
UI_DIR ?= ui
LVGL_SRC := $(shell find $(LVGL_DIR)/lvgl/src -name '*.c')
UI_SRC := $(shell find $(UI_DIR) -name '*.c')

SRC = main.c ili9341.c lvgl_connect.c gpio.c touch.c $(LVGL_SRC) $(UI_SRC)

INCLUDES = -I$(LVGL_DIR)/lvgl \
           -I$(LVGL_DIR)/lvgl/src \
           -I$(LVGL_DIR)/lvgl/src/core \
           -I$(LVGL_DIR)/lvgl/src/widgets \
           -I$(LVGL_DIR)/lvgl/src/hal \
           -I$(LVGL_DIR)/lvgl/src/misc \
           -I$(LVGL_DIR)/lvgl/src/draw \
		   -I$(UI_DIR)/ \
		   -I$(UI_DIR)/fonts \
		   -I$(UI_DIR)/images \
		   -I$(UI_DIR)/screens \
		   -I$(UI_DIR)/components \
		   -I$(UI_DIR)/ \


CFLAGS = -lgpiod -g

build:
	gcc $(SRC) $(CFLAGS) $(INCLUDES) -o app
