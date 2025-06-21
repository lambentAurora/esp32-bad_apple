#ifndef __OLED_H__
#define __OLED_H__

#include "freertos/FreeRTOS.h"
#include "driver/i2c_master.h"
#include <driver/i2c.h>

#define u8 unsigned char

#define I2C_Wait_Time_ms   20  // I2C等待时间，单位毫秒

extern const unsigned char OLED_init_cmd[25];
extern i2c_master_dev_handle_t dev_handle;

extern void OLED_send_cmd(unsigned char o_command);
extern void OLED_send_data(unsigned char o_data);
extern void Column_set(unsigned char column);
extern void Page_set(unsigned char page);
extern void OLED_clear(void);
extern void OLED_full(void);
extern void OLED_init(void);
extern void Picture_display(const unsigned char *ptr_pic);
extern void Picture_ReverseDisplay(const unsigned char *ptr_pic);
extern void OLED_SetPos(unsigned char x, unsigned char y);
extern void OLED_ShowStr(unsigned char x, unsigned char y, char ch[], unsigned char TextSize);
extern void OLED_AreaClera(unsigned char x, unsigned char y, unsigned char TextSize, unsigned char len);

/****************************/

extern void OLED_Display_On(void);
extern void OLED_Display_Off(void);	   							   		    
// extern void OLED_DrawPoint(u8 x,u8 y,u8 t);
// extern void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);	 
extern void OLED_ShowCHinese(u8 x,u8 y,u8 no,u8 len);
extern void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
// extern void fill_picture(unsigned char fill_Data);

// extern void Show_Weather( void *pvParameters );

#endif
