#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "driver/i2c_master.h"
#include "sdkconfig.h"
#include <driver/i2c.h>
#include "./main/inc/oled.h"
#include "esp_partition.h"

i2c_master_bus_handle_t bus_handle;
i2c_master_dev_handle_t dev_handle;
extern const unsigned char qing[];

void app_main(void)
{
    unsigned char oled_buffer[1024] = {0};  // Buffer for OLED display

    i2c_master_bus_config_t bus_conf = {
        .i2c_port = I2C_NUM_0,
        .sda_io_num = GPIO_NUM_32,
        .scl_io_num = GPIO_NUM_33,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags = {
            .allow_pd = 0,
            .enable_internal_pullup = 0,  // Enable internal pull-ups
        },
    };

    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = 0x3c,  // OLED I2C address
        .flags = {
            .disable_ack_check = 0,  // Enable ACK check
        },
        .scl_speed_hz = 400000,  // 400 kHz
        .scl_wait_us = 5,  // 20 ms wait time
    };
   
    ESP_ERROR_CHECK(i2c_new_master_bus(&bus_conf, &bus_handle));  // 创建总线
    ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &dev_cfg, &dev_handle));  // 添加设备
    // i2c_master_transmit(dev_handle, OLED_init_cmd, sizeof(OLED_init_cmd), 1);

    // Initialize OLED display
    OLED_init();
    OLED_clear();
    ESP_LOGI("main_task", "I2C initialized successfully");
    const esp_partition_t *badapple_partition =  esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, "badapple");
    ESP_LOGI("main_task", "Found partition: %s", badapple_partition->label);
    if (badapple_partition == NULL) {
        ESP_LOGE("main_task", "Partition not found");
        return;
    }
    
    vTaskPrioritySet(NULL, 24);  // Set task priority to 5

    uint16_t oled_buffer_size = sizeof(oled_buffer);
    // ESP_ERROR_CHECK(esp_partition_read(badapple_partition, 100*1024, oled_buffer, sizeof(oled_buffer)));
    // ESP_LOGI("main_task", "oled_buffer size:%d", oled_buffer_size);

    // Picture_display(oled_buffer);  // Display the picture on OLED
    // ESP_LOGI("main_task", "Picture displayed on OLED");
    
    for(uint16_t i = 0;i<2284;i++) {
        esp_partition_read(badapple_partition, 
            i * (oled_buffer_size), 
            oled_buffer, 
            sizeof(oled_buffer));

        Picture_display(oled_buffer);
        
        // vTaskDelay(pdMS_TO_TICKS(1));  // Delay for 1 second
        // if (i > 2284){
        //     for (;;)
        //     {
        //         printf("播放完毕...\n");
        //         vTaskDelay(pdMS_TO_TICKS(1000));  // Delay for 1 second
        //     }
        // }else {
        //     i++;
        // }
        // printf("app_main running...\ti:%d\n", i);
        // vTaskDelay(pdMS_TO_TICKS(1000));
    }

    OLED_DrawBMP(41, 9, 86, 54, qing);
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
        printf("播放完毕\r\n");
    }
    
}
