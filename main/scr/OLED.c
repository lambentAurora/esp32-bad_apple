#include "oled.h"
#include "Font.h"
#include "driver/i2c_master.h"
#include <driver/i2c.h>
#include "freertos/FreeRTOS.h"
#include "esp_log.h"

const unsigned char OLED_init_cmd[25]=
{
  /*0xae,0X00,0X10,0x40,0X81,0XCF,0xff,0xa1,0xa4,
  0xA6,0xc8,0xa8,0x3F,0xd5,0x80,0xd3,0x00,0XDA,0X12,
  0x8d,0x14,0xdb,0x40,0X20,0X02,0xd9,0xf1,0xAF*/
       0xAE,//关闭显示
       0xD5,//设置时钟分频因子,震荡频率
       0x80,  //[3:0],分频因子;[7:4],震荡频率

       0xA8,//设置驱动路数
       0X3F,//默认0X3F(1/64)
       0xD3,//设置显示偏移
       0X00,//默认为0
       0x40,//设置显示开始行 [5:0],行数.                              
       0x8D,//电荷泵设置
       0x14,//bit2，开启/关闭
       0x20,//设置内存地址模式
       0x02,//[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
       0xA1,//段重定义设置,bit0:0,0->0;1,0->127;
       0xC8,//设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数
       0xDA,//设置COM硬件引脚配置
       0x12,//[5:4]配置            
       0x81,//对比度设置
       0xEF,//1~255;默认0X7F (亮度设置,越大越亮)
       0xD9,//设置预充电周期
       0xf1,//[3:0],PHASE 1;[7:4],PHASE 2;
       0xDB,//设置VCOMH 电压倍率
       0x30,//[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;
       0xA4,//全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
       0xA6,//设置显示方式;bit0:1,反相显示;0,正常显示        
       0xAF,//开启显示     
};

/*
*********************************************************************************************************
*	函 数 名: OLED_init
*	功能说明: 初始化OLED
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void OLED_init(void)
{
	unsigned char i;
	// IIC_Init();		//初始化IIC
    printf("即将进行OLED初始化\r\n");
	for(i=0;i<25;i++)
	{
        printf("OLED_init_cmd[%d]:%x\r\n", i, OLED_init_cmd[i]);
		OLED_send_cmd(OLED_init_cmd[i]);
        vTaskDelay(pdMS_TO_TICKS(1));
	}
}

//写命令
/*
void OLED_send_cmd(i2c_master_dev_handle_t i2c_dev, unsigned char o_command)
{
	// IIC_Start();
    // IIC_Send_Byte(0x78);//OLED地址
    // IIC_Wait_Ack();
    // IIC_Send_Byte(0x00);//寄存器地址
    // IIC_Wait_Ack();
    // IIC_Send_Byte(o_command);
    // IIC_Wait_Ack();
    // IIC_Stop();

    u8 buff[] = {0x00,0x00};
    buff[1] = o_command;
    ESP_ERROR_CHECK(i2c_master_transmit(i2c_dev, buff, sizeof(buff), 1));
}*/
void OLED_send_cmd(unsigned char o_command)
{
	// IIC_Start();
    // IIC_Send_Byte(0x78);//OLED地址
    // IIC_Wait_Ack();
    // IIC_Send_Byte(0x00);//寄存器地址
    // IIC_Wait_Ack();
    // IIC_Send_Byte(o_command);
    // IIC_Wait_Ack();
    // IIC_Stop();

    u8 buff[] = {0x00,0x00};
    buff[1] = o_command;
    // printf("\t准备发送命令%x\r\n", o_command);
    ESP_ERROR_CHECK(i2c_master_transmit(dev_handle, buff, sizeof(buff), I2C_Wait_Time_ms));
    // printf("\t%x命令发送完毕\r\n", o_command);
}
//写数据
/*
void OLED_send_data(i2c_master_dev_handle_t i2c_dev, unsigned char o_data)
{ 
	// IIC_Start();
    // IIC_Send_Byte(0x78);//OLED地址
    // IIC_Wait_Ack();
    // IIC_Send_Byte(0x40);//寄存器地址
    // IIC_Wait_Ack();
    // IIC_Send_Byte(o_data);
    // IIC_Wait_Ack();
    // IIC_Stop();

    u8 buff[] = {0x40,0x00};
    buff[1] = o_data;
    ESP_ERROR_CHECK(i2c_master_transmit(i2c_dev, buff, sizeof(buff), 1));
}*/
void OLED_send_data(unsigned char o_data)
{ 
	// IIC_Start();
    // IIC_Send_Byte(0x78);//OLED地址
    // IIC_Wait_Ack();
    // IIC_Send_Byte(0x40);//寄存器地址
    // IIC_Wait_Ack();
    // IIC_Send_Byte(o_data);
    // IIC_Wait_Ack();
    // IIC_Stop();

    u8 buff[] = {0x40,0x00};
    buff[1] = o_data;
    ESP_ERROR_CHECK(i2c_master_transmit(dev_handle, buff, sizeof(buff), I2C_Wait_Time_ms));
}

void Column_set(unsigned char column)
{
	OLED_send_cmd(0x10|(column>>4));    //设置列地址高位
	OLED_send_cmd(0x00|(column&0x0f));  //设置列地址低位   
}
void Page_set(unsigned char page)
{
	OLED_send_cmd(0xb0+page);
}

/*
*********************************************************************************************************
*	函 数 名: OLED_clear
*	功能说明: 清屏整个OLED的显示
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void OLED_clear(void)
{
	unsigned char page,column;
	for(page=0;page<8;page++)             //page loop
	{ 
		Page_set(page);
		Column_set(0);	  
		for(column=0;column<128;column++)	//column loop
		{
			OLED_send_data(0x00);
		}
	}
}

/*
*********************************************************************************************************
*	函 数 名: OLED_full
*	功能说明: 填充整个OLED的显示
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void OLED_full(void)
{
	unsigned char page,column;
	for(page=0;page<8;page++)             //page loop
	{ 
		Page_set(page);
		Column_set(0);	  
		for(column=0;column<128;column++)	//column loop
		{
			OLED_send_data(0xff);
		}
	}
}

/*
*********************************************************************************************************
*	函 数 名: Picture_display
*	功能说明: 显示图片（128*64）
*	形    参: *ptr_pic：图片数组的头指针
*	返 回 值: 无
*********************************************************************************************************
*/
void Picture_display(const unsigned char *ptr_pic)
{
	unsigned char page,column;
	for(page=0;page<8;page++)        //page loop
	{ 
		Page_set(page);
		Column_set(0);	  
		for(column=0;column<128;column++)	//column loop
		{
			OLED_send_data(*ptr_pic++);
		}
	}
}

/*
*********************************************************************************************************
*	函 数 名: Picture_ReverseDisplay
*	功能说明: 反转的形式显示图片（128*64）
*	形    参: *ptr_pic：图片数组的头指针
*	返 回 值: 无
*********************************************************************************************************
*/
void Picture_ReverseDisplay(const unsigned char *ptr_pic)
{
	unsigned char page,column,oled_data;
	for(page=0;page<(64/8);page++)        //page loop
	{ 
		Page_set(page);
		Column_set(0);	  
		for(column=0;column<128;column++)	//column loop
		{
			oled_data=*ptr_pic++;
			oled_data=~oled_data;
			OLED_send_data(oled_data);
		}
	}
}

//设置光标位置
void OLED_SetPos(unsigned char x, unsigned char y)
{ 
    OLED_send_cmd(0xb0+y);
    OLED_send_cmd(((x&0xf0)>>4)|0x10);
    OLED_send_cmd((x&0x0f)|0x01);
}

/*
*********************************************************************************************************
*	函 数 名: OLED_ShowStr
*	功能说明: 显示字符串（在字库中的字符）
*	形    参: 1. x,y：起始显示坐标		//y=0,2,4,6（TextSize=2）  y=0,1,2,3,4,5,6,7（TextSize=1）
			  2. ch[]：字符串    
			  3. TextSize：字符大小（1:6*8  2:8*16）
*	返 回 值: 无
*********************************************************************************************************
*/
void OLED_ShowStr(unsigned char x, unsigned char y, char ch[], unsigned char TextSize)
{
    unsigned char c = 0,i = 0,j = 0;
    switch(TextSize)
    {
        case 1:
        {
            while(ch[j] != '\0')
            {
                c = ch[j] - 32;
                if(x > 126)
                {
                    x = 0;
                    y++;
                }
                OLED_SetPos(x,y);
                for(i=0;i<6;i++)
                    OLED_send_data(F6x8[c*6 + i]);
                x += 6;
                j++;
            }
        }break;
        case 2:
        {
            while(ch[j] != '\0')
            {
                c = ch[j] - 32;
                if(x > 120)
                {
                    x = 0;
                    y++;
                }
                OLED_SetPos(x,y);
                for(i=0;i<8;i++)
                    OLED_send_data(F8x16[c*16+i]);
                OLED_SetPos(x,y+1);
                for(i=0;i<8;i++)
                    OLED_send_data(F8x16[c*16+i+8]);
                x += 8;
                j++;
            }
        }break;
    }
}
/*
*********************************************************************************************************
*	函 数 名: OLED_AreaClear
*	功能说明: 区域清除（通过显示空格）
*	形    参: 1. x,y：起始显示坐标		//y=0,2,4,6（TextSize=2）  y=0,1,2,3,4,5,6,7（TextSize=1）
			  2. TextSize：字符大小（1:6*8  2:8*16）
              3. len：清屏长度
*	返 回 值: 无
*********************************************************************************************************
*/
void OLED_AreaClera(unsigned char x, unsigned char y, unsigned char TextSize, unsigned char len)
{
    if (TextSize == 1){
        if (TextSize > 21)
            TextSize = 21;
        while (len--)
        {
            OLED_ShowStr(x,y," ",TextSize);
            x+=6;
        } 
    }else if (TextSize == 2){
        if (TextSize > 16)
            TextSize = 16;
        while (len--)
        {
            OLED_ShowStr(x,y," ",TextSize);
            x+=8;
        } 
    }
}

/********************************************************/

//开启OLED显示    
void OLED_Display_On(void)
{
	OLED_send_cmd(0X8D);  //SET DCDC命令
	OLED_send_cmd(0X14);  //DCDC ON
	OLED_send_cmd(0XAF);  //DISPLAY ON
}
//关闭OLED显示     
void OLED_Display_Off(void)
{
	OLED_send_cmd(0X8D);  //SET DCDC命令
	OLED_send_cmd(0X10);  //DCDC OFF
	OLED_send_cmd(0XAE);  //DISPLAY OFF
}
/*
*********************************************************************************************************
*	函 数 名: OLED_ShowCHinese
*	功能说明: 显示中文字符（在字库中的字符）
*	形    参: 1、2. x,y：起始显示坐标		//y=0,2,4,6（TextSize=2）  y=0,1,2,3,4,5,6,7（TextSize=1）
		      3. no：所显示中文字符在字库中的序号
              4. len: 需要连续显示汉字的长度
*	返 回 值: 无
*********************************************************************************************************
*/
void OLED_ShowCHinese(u8 x,u8 y,u8 no,u8 len)
{      			    
	u8 t,adder=0;
    while (len--)
    {
        OLED_SetPos(x,y);
        for(t=0;t<16;t++)
        {
            OLED_send_data(Hzk[2*no][t]);
            adder+=1;
        }
        OLED_SetPos(x,y+1);	
        for(t=0;t<16;t++)
        {	
            OLED_send_data(Hzk[2*no+1][t]);
            adder+=1;
        }no+=1;
        x+=16;
    }			
}
/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
	unsigned int j=0;
	unsigned char x,y;

	if(y1%8==0) y=y1/8;      
	else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_SetPos(x0,y);
		for(x=x0;x<x1;x++)
		{      
			OLED_send_data(BMP[j++]);	    	
		}
	}
}

// void Show_Weather( void *pvParameters )
// {
//     if (CityWeatherData.Weather == String("晴")){
//         if (Weather != 0){
//             Weather = 0;
//             OLED_clear();
//         }
//         OLED_ShowCHinese(0,0,16,3);/*娄底市*/
//         OLED_ShowCHinese(0,2,7,1);/*晴*/
//         OLED_ShowStr(24,2,(char*)CityWeatherData.temperature.c_str(),2);//温度度数
//         OLED_ShowCHinese(40,2,3,1);/*℃*/
//         if (Date.Hour <= 18)
//             OLED_DrawBMP(76,0,121,6,(unsigned char*)qing);
//         else
//             OLED_DrawBMP(76,0,121,6,(unsigned char*)wanqing);
//     }else if (CityWeatherData.Weather == String("多云")){
//         if (Weather != 1){
//             Weather = 1;
//             OLED_clear();
//         }
//         OLED_ShowCHinese(0,0,16,3);/*娄底市*/
//         OLED_ShowCHinese(0,2,8,2);/*多云*/
//         OLED_ShowStr(40,2,(char*)CityWeatherData.temperature.c_str(),2);//温度度数
//         OLED_ShowCHinese(56,2,3,1);/*℃*/
//         if (Date.Hour <= 18)
//             OLED_DrawBMP(76,0,121,6,(unsigned char*)wanduoyun);
//         else
//             OLED_DrawBMP(76,0,121,6,(unsigned char*)duoyun);
//     }else if (CityWeatherData.Weather == String("阴")){
//         if (Weather != 2){
//             Weather = 2;
//             OLED_clear();
//         }
//         OLED_ShowCHinese(0,0,16,3);/*娄底市*/
//         OLED_ShowCHinese(0,2,10,1);/*阴*/
//         OLED_ShowStr(24,2,(char*)CityWeatherData.temperature.c_str(),2);//温度度数
//         OLED_ShowCHinese(40,2,3,1);/*℃*/
//         OLED_DrawBMP(76,0,121,6,(unsigned char*)yin);
//     }else if (CityWeatherData.Weather == String("小雨")){
//         if (Weather != 3){
//             Weather = 3;
//             OLED_clear();
//         }
//         OLED_ShowCHinese(0,0,16,3);/*娄底市*/
//         OLED_ShowCHinese(0,2,11,1);/*小*/
//         OLED_ShowCHinese(16,2,15,1);/*雨*/
//         OLED_ShowStr(40,2,(char*)CityWeatherData.temperature.c_str(),2);//温度度数
//         OLED_ShowCHinese(56,2,3,1);/*℃*/
//         OLED_DrawBMP(76,0,121,6,(unsigned char*)xiaoyu);
//     }else if (CityWeatherData.Weather == String("中雨")){
//         if (Weather != 4){
//             Weather = 4;
//             OLED_clear();
//         }
//         OLED_ShowCHinese(0,0,16,3);/*娄底市*/
//         OLED_ShowCHinese(0,2,12,1);/*中*/
//         OLED_ShowCHinese(16,2,15,1);/*雨*/
//         OLED_ShowStr(40,2,(char*)CityWeatherData.temperature.c_str(),2);//温度度数
//         OLED_ShowCHinese(56,2,3,1);/*℃*/
//         OLED_DrawBMP(76,0,121,6,(unsigned char*)zhongyu);
//     }else if (CityWeatherData.Weather == String("大雨")){
//         if (Weather != 5){
//             Weather = 5;
//             OLED_clear();
//         }
//         OLED_ShowCHinese(0,0,16,3);/*娄底市*/
//         OLED_ShowCHinese(0,2,13,1);/*大*/
//         OLED_ShowCHinese(16,2,15,1);/*雨*/
//         OLED_ShowStr(40,2,(char*)CityWeatherData.temperature.c_str(),2);//温度度数
//         OLED_ShowCHinese(56,2,3,1);/*℃*/
//         OLED_DrawBMP(76,0,121,6,(unsigned char*)dayu);
//     }else if (CityWeatherData.Weather == String("暴雨")){
//         if (Weather != 5){
//             Weather = 5;
//             OLED_clear();
//         }
//         OLED_ShowCHinese(0,0,16,3);/*娄底市*/
//         OLED_ShowCHinese(0,2,14,2);/*暴雨*/
//         OLED_ShowStr(40,2,(char*)CityWeatherData.temperature.c_str(),2);//温度度数
//         OLED_ShowCHinese(56,2,3,1);/*℃*/
//         OLED_DrawBMP(76,0,121,6,(unsigned char*)baoyu);
//     }
// }
