#ifndef LVGL_CONNECT_H_
#define LVGL_CONNECT_H_
#include "lvgl.h"

void my_lv_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_map);

void lv_port_disp_init(void);

void *tick_thread(void *arg);

#endif // LVGL_CONNECT_H_
