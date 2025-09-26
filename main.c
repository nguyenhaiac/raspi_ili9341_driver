#include "ili9341.h"
#include "lvgl.h"

static lv_disp_draw_buf_t disp_buf;
static lv_color_t buf1[ILI9341_SCREEN_WIDTH * 40];

void my_lv_flush(lv_disp_drv_t *disp_drv, const lv_area_t * area, lv_color_t *color_map){
    uint32_t w = area->x2 - area->x1 +1;
    uint32_t h = area->y2 - area->y1 +1;

    ILI9341_Set_Address(area->x1, area->y1, area->x2, area->y2);
    ILI9341_SendBitmap(area->x1, area->x2, w, h, (uint16_t *)color_map);
    lv_disp_flush_ready(disp_drv);
}

void lv_port_disp_init(){
    lv_disp_draw_buf_init(&disp_buf, buf1, NULL, ILI9341_SCREEN_WIDTH*40);
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf = &disp_buf;
    disp_drv.hor_res = ILI9341_SCREEN_WIDTH;
    disp_drv.ver_res = ILI9341_SCREEN_HEIGHT;
    disp_drv.flush_cb = my_lv_flush;
    lv_disp_drv_register(&disp_drv);
}

int main() {
  lv_init();
  gpio_init();
  lv_port_disp_init();
  ILI9341_Init();
  lv_obj_t *label = lv_label_create(lv_scr_act());
  lv_label_set_text(label, "Hello LVGL + ILI9341!");
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
}
