#include <stdio.h>

#include "driver/gpio.h"
//#include "driver/can.h"
#include "driver/twai.h"

//#include "sdkconfig.h"

static const char *TAG = "can_example";

uint32_t my_alerts = 0;

esp_err_t recv() {
    //Wait for message to be received
    can_message_t message; FIXME this is not a TWAI structure, is it?
    if (can_receive(&message, pdMS_TO_TICKS(10000)) == ESP_OK) {
        printf("Message received\n");
    } else {
        printf("Failed to receive message\n");
        return;
    }

    //Process received message
    if (message.flags & CAN_MSG_FLAG_EXTD) {
        printf("Message is in Extended Format\n");
    } else {
        printf("Message is in Standard Format\n");
    }
    printf("ID is %d\n", message.identifier);
    if (!(message.flags & CAN_MSG_FLAG_RTR)) {
        for (int i = 0; i < message.data_length_code; i++) {
            printf("Data byte %d = %d\n", i, message.data[i]);
        }
    }
}

void app_main()
{
    //Initialize configuration structures using macro initializers
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(GPIO_NUM_5, GPIO_NUM_35, TWAI_MODE_NORMAL);
    twai_reconfigure_alerts(TWAI_ALERT_ALL || TWAI_ALERT_AND_LOG, NULL, &my_alerts);
    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_250KBITS();
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

    //Install CAN driver
    if (twai_driver_install(&g_config, &t_config, &f_config) == ESP_OK) {
        printf("Driver installed\n");
    } else {
        printf("Failed to install driver\n");
        return;
    }

    //Start CAN driver
    if (twai_start() == ESP_OK) {
        printf("Driver started\n");
    } else {
        printf("Failed to start driver\n");
        return;
    }

    while(true) {
        recv();
        if (twai_read_alerts(&my_alerts, 10) == ESP_OK) {
            printf("ESP_OK. alerts:%x\n", my_alerts);
        } else {
            printf("ESP_FAIL\n");
        }
    };
    

}
