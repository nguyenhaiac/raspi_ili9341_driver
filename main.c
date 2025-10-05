#include "lvgl.h"
#include "ili9341.h"
#include "misc/lv_color.h"
#include "unistd.h"
#include "stdio.h"
#include "pthread.h"
#include "lvgl_connect.h"
#include "ui/ui.h"
#include "gpio.h"

int main() {
  get_gpio_chip();
  pthread_t tick;
  pthread_create(&tick, NULL, tick_thread, NULL);
  printf("size of: %lu", sizeof(lv_color_t));
  lv_init();
  gpio_init();
  ILI9341_Init();
  lv_port_disp_init();
  lv_obj_t *label = lv_label_create(lv_scr_act());
  lv_label_set_text(label, "Hello LVGL + ILI9341!");
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
  ui_init();

  while (1) {
    lv_timer_handler();  // Update LVGL tasks
    usleep(5000);        // Small delay to avoid 100% CPU
  }
}
