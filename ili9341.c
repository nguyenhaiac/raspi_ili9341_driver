#include "gpiod.h"
#include "stdio.h"
#include "fcntl.h"
#include "sys/ioctl.h"
#include "linux/spi/spidev.h"
#include "unistd.h"
#include "stdint.h"
#include "ili9341.h"

static struct gpiod_chip *gpiochip;
static struct gpiod_line * cs, *reset, *dc;
static int fd;
volatile uint16_t LCD_HEIGHT = ILI9341_SCREEN_HEIGHT;
volatile uint16_t LCD_WIDTH	 = ILI9341_SCREEN_WIDTH;

void ILI9341_SPI_Init(char * spidevice, uint8_t mode, uint32_t speed){
    fd = open(spidevice, O_RDWR);
    if(fd < 0){
        perror("Cannot open spi device");
        return;
    }
    ioctl(fd, SPI_IOC_WR_MODE, &mode);
    ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
}

void gpio_init(){
    gpiochip = gpiod_chip_open_by_name(CHIP_NAME);
    if(!gpiochip){
        perror("Cannot open gpiochip");
        return;
    }
    cs = gpiod_chip_get_line(gpiochip, CS_PIN);
    dc = gpiod_chip_get_line(gpiochip, DC_PIN);
    reset = gpiod_chip_get_line(gpiochip, RESET_PIN);

    if(!cs || !dc || !reset){
        perror("Cannot get line");
        return;
    }
    
    if(gpiod_line_request_output(cs, "ili9341", 0) < 0){
        perror("Cannot get cs line");
        return;
    }

    if(gpiod_line_request_output(dc, "ili9341", 0) < 0){
        perror("Cannot get dc line");
        return;
    }

    if(gpiod_line_request_output(reset, "ili9341", 1) < 0){
        perror("Cannot get cs line");
        return;
    }
}

void set_gpio(struct gpiod_line *line, int value) {
    if (gpiod_line_set_value(line, value) < 0) {
        perror("gpiod_line_set_value");
        exit(1);
    }
}




void ILI9341_SPI_Send(unsigned char data){
    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long)&data,
        .rx_buf = (unsigned long)NULL,
        .len = 1,
        .speed_hz = SPEED,
        .delay_usecs = 0,
        .bits_per_word = 8,
    };
    if(ioctl(fd, SPI_IOC_MESSAGE(1), &tr)<1){
        perror("SPI transer failed");
        exit(1);
    }
}   
    

void ILI9341_Write_Command(uint8_t Command){
    set_gpio(cs, 0);
    set_gpio(dc, 0);
    ILI9341_SPI_Send(Command);
    set_gpio(cs, 1);
}


void ILI9341_Write_Data(uint8_t Data)
{
    set_gpio(dc, 1);
    set_gpio(cs, 0);
    ILI9341_SPI_Send(Data);	
    set_gpio(cs, 1);
}


void ILI9341_Reset(void)
{
    set_gpio(reset, 0);
    usleep(200000);
    set_gpio(cs, 0);
    usleep(200000);
    set_gpio(reset, 1);
}


void ILI9341_Enable(void)
{
    set_gpio(reset, 1);
}


void ILI9341_Set_Rotation(uint8_t Rotation) 
{
	
uint8_t screen_rotation = Rotation;

ILI9341_Write_Command(0x36);
sleep(1);
	
switch(screen_rotation) 
	{
		case SCREEN_VERTICAL_1:
			ILI9341_Write_Data(0x40|0x08);
			LCD_WIDTH = 240;
			LCD_HEIGHT = 320;
			break;
		case SCREEN_HORIZONTAL_1:
			ILI9341_Write_Data(0x20|0x08);
			LCD_WIDTH  = 320;
			LCD_HEIGHT = 240;
			break;
		case SCREEN_VERTICAL_2:
			ILI9341_Write_Data(0x80|0x08);
			LCD_WIDTH  = 240;
			LCD_HEIGHT = 320;
			break;
		case SCREEN_HORIZONTAL_2:
			ILI9341_Write_Data(0x40|0x80|0x20|0x08);
			LCD_WIDTH  = 320;
			LCD_HEIGHT = 240;
			break;
		default:
			//EXIT IF SCREEN ROTATION NOT VALID!
			break;
	}
}


void ILI9341_Init(void)
{

ILI9341_Enable();
ILI9341_SPI_Init(SPI0, SPI_MODE_0, SPEED);
ILI9341_Reset();

//SOFTWARE RESET
ILI9341_Write_Command(0x01);
sleep(1);
	
//POWER CONTROL A
ILI9341_Write_Command(0xCB);
ILI9341_Write_Data(0x39);
ILI9341_Write_Data(0x2C);
ILI9341_Write_Data(0x00);
ILI9341_Write_Data(0x34);
ILI9341_Write_Data(0x02);

//POWER CONTROL B
ILI9341_Write_Command(0xCF);
ILI9341_Write_Data(0x00);
ILI9341_Write_Data(0xC1);
ILI9341_Write_Data(0x30);

//DRIVER TIMING CONTROL A
ILI9341_Write_Command(0xE8);
ILI9341_Write_Data(0x85);
ILI9341_Write_Data(0x00);
ILI9341_Write_Data(0x78);

//DRIVER TIMING CONTROL B
ILI9341_Write_Command(0xEA);
ILI9341_Write_Data(0x00);
ILI9341_Write_Data(0x00);

//POWER ON SEQUENCE CONTROL
ILI9341_Write_Command(0xED);
ILI9341_Write_Data(0x64);
ILI9341_Write_Data(0x03);
ILI9341_Write_Data(0x12);
ILI9341_Write_Data(0x81);

//PUMP RATIO CONTROL
ILI9341_Write_Command(0xF7);
ILI9341_Write_Data(0x20);

//POWER CONTROL,VRH[5:0]
ILI9341_Write_Command(0xC0);
ILI9341_Write_Data(0x23);

//POWER CONTROL,SAP[2:0];BT[3:0]
ILI9341_Write_Command(0xC1);
ILI9341_Write_Data(0x10);

//VCM CONTROL
ILI9341_Write_Command(0xC5);
ILI9341_Write_Data(0x3E);
ILI9341_Write_Data(0x28);

//VCM CONTROL 2
ILI9341_Write_Command(0xC7);
ILI9341_Write_Data(0x86);

//MEMORY ACCESS CONTROL
ILI9341_Write_Command(0x36);
ILI9341_Write_Data(0x48);

//PIXEL FORMAT
ILI9341_Write_Command(0x3A);
ILI9341_Write_Data(0x55);

//FRAME RATIO CONTROL, STANDARD RGB COLOR
ILI9341_Write_Command(0xB1);
ILI9341_Write_Data(0x00);
ILI9341_Write_Data(0x18);

//DISPLAY FUNCTION CONTROL
ILI9341_Write_Command(0xB6);
ILI9341_Write_Data(0x08);
ILI9341_Write_Data(0x82);
ILI9341_Write_Data(0x27);

//3GAMMA FUNCTION DISABLE
ILI9341_Write_Command(0xF2);
ILI9341_Write_Data(0x00);

//GAMMA CURVE SELECTED
ILI9341_Write_Command(0x26);
ILI9341_Write_Data(0x01);

//POSITIVE GAMMA CORRECTION
ILI9341_Write_Command(0xE0);
ILI9341_Write_Data(0x0F);
ILI9341_Write_Data(0x31);
ILI9341_Write_Data(0x2B);
ILI9341_Write_Data(0x0C);
ILI9341_Write_Data(0x0E);
ILI9341_Write_Data(0x08);
ILI9341_Write_Data(0x4E);
ILI9341_Write_Data(0xF1);
ILI9341_Write_Data(0x37);
ILI9341_Write_Data(0x07);
ILI9341_Write_Data(0x10);
ILI9341_Write_Data(0x03);
ILI9341_Write_Data(0x0E);
ILI9341_Write_Data(0x09);
ILI9341_Write_Data(0x00);

//NEGATIVE GAMMA CORRECTION
ILI9341_Write_Command(0xE1);
ILI9341_Write_Data(0x00);
ILI9341_Write_Data(0x0E);
ILI9341_Write_Data(0x14);
ILI9341_Write_Data(0x03);
ILI9341_Write_Data(0x11);
ILI9341_Write_Data(0x07);
ILI9341_Write_Data(0x31);
ILI9341_Write_Data(0xC1);
ILI9341_Write_Data(0x48);
ILI9341_Write_Data(0x08);
ILI9341_Write_Data(0x0F);
ILI9341_Write_Data(0x0C);
ILI9341_Write_Data(0x31);
ILI9341_Write_Data(0x36);
ILI9341_Write_Data(0x0F);

//EXIT SLEEP
ILI9341_Write_Command(0x11);
usleep(120000);

//TURN ON DISPLAY
ILI9341_Write_Command(0x29);

//STARTING ROTATION
ILI9341_Set_Rotation(SCREEN_VERTICAL_1);
}


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


void ILI9341_Set_Address(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2)
{
ILI9341_Write_Command(0x2A);
ILI9341_Write_Data(X1>>8);
ILI9341_Write_Data(X1);
ILI9341_Write_Data(X2>>8);
ILI9341_Write_Data(X2);

ILI9341_Write_Command(0x2B);
ILI9341_Write_Data(Y1>>8);
ILI9341_Write_Data(Y1);
ILI9341_Write_Data(Y2>>8);
ILI9341_Write_Data(Y2);

ILI9341_Write_Command(0x2C);
}

void ILI9341_Draw_Colour_Burst(uint16_t Colour, uint32_t Size)
{
//SENDS COLOUR
uint32_t Buffer_Size = 0;
if((Size*2) < BURST_MAX_SIZE)
{
	Buffer_Size = Size;
}
else
{
	Buffer_Size = BURST_MAX_SIZE;
}
	
set_gpio(dc, 1);	
set_gpio(cs, 0);

unsigned char chifted = 	Colour>>8;;
unsigned char burst_buffer[Buffer_Size];
for(uint32_t j = 0; j < Buffer_Size; j+=2)
	{
		burst_buffer[j] = 	chifted;
		burst_buffer[j+1] = Colour;
	}

uint32_t Sending_Size = Size*2;
uint32_t Sending_in_Block = Sending_Size/Buffer_Size;
uint32_t Remainder_from_block = Sending_Size%Buffer_Size;

if(Sending_in_Block != 0)
{
	for(uint32_t j = 0; j < (Sending_in_Block); j++)
		{

        spi_transfer(fd, burst_buffer, NULL, Buffer_Size, SPEED);
		}
}

//REMAINDER!
spi_transfer(fd, burst_buffer, NULL, Remainder_from_block, SPEED);

set_gpio(cs, 1);
}

void ILI9341_Fill_Screen(uint16_t Colour)
{
ILI9341_Set_Address(0,0,LCD_WIDTH,LCD_HEIGHT);	
ILI9341_Draw_Colour_Burst(Colour, LCD_WIDTH*LCD_HEIGHT);	
}
