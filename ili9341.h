#include "stdint.h"
#include "gpiod.h"

#define SPI0 "/dev/spidev0.0"
#define CS_PIN 25
#define DC_PIN 22
#define RESET_PIN 27
#define CHIP_NAME "gpiochip0"
#define SPEED 24000000
#define ILI9341_SCREEN_HEIGHT 240 
#define ILI9341_SCREEN_WIDTH 	320

#define SCREEN_VERTICAL_1		0
#define SCREEN_HORIZONTAL_1		1
#define SCREEN_VERTICAL_2		2
#define SCREEN_HORIZONTAL_2		3
#define BURST_MAX_SIZE 	500

#define BLACK       0x0000      
#define NAVY        0x000F      
#define DARKGREEN   0x03E0      
#define DARKCYAN    0x03EF      
#define MAROON      0x7800      
#define PURPLE      0x780F      
#define OLIVE       0x7BE0      
#define LIGHTGREY   0xC618      
#define DARKGREY    0x7BEF      
#define BLUE        0x001F      
#define GREEN       0x07E0      
#define CYAN        0x07FF      
#define RED         0xF800     
#define MAGENTA     0xF81F      
#define YELLOW      0xFFE0      
#define WHITE       0xFFFF      
#define ORANGE      0xFD20      
#define GREENYELLOW 0xAFE5     
#define PINK        0xF81F

void ILI9341_SPI_Init(char * spidevice, uint8_t mode, uint32_t speed);
void gpio_init();
void set_gpio(struct gpiod_line *line, int value);
void ili9341_init();
void ILI9341_SPI_Send(unsigned char data);
void ILI9341_Write_Command(uint8_t Command);
void ILI9341_Write_Data(uint8_t Data);
void ILI9341_Reset(void);
void ILI9341_Enable(void);
void ILI9341_Set_Rotation(uint8_t Rotation);
void ILI9341_Init(void);
void ILI9341_Set_Address(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2);
void ILI9341_Draw_Colour_Burst(uint16_t Colour, uint32_t Size);
void ILI9341_Fill_Screen(uint16_t Colour);