#include "ili9341.h"

int main(){
    gpio_init();
    ILI9341_Init();
    ILI9341_Fill_Screen(MAGENTA);
}