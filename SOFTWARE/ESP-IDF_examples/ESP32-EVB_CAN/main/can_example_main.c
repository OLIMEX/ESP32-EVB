// Code based on example at
// https://docs.espressif.com/projects/esp-idf/en/release-v3.3/api-reference/peripherals/can.html
// which is Copyright 2015-2021 Espressif Systems (Shanghai) PTE LTD
//
// All other example code Copyright 2022 Brian Alano and 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.#include <stdio.h>

#include "driver/gpio.h"
// #include "driver/can.h"
#include "driver/twai.h"

//#include "sdkconfig.h"

static const char *TAG = "can_example";

esp_err_t recv() {
    //Wait for message to be received
    twai_message_t message; 
    if (twai_receive(&message, pdMS_TO_TICKS(10000)) == ESP_OK) {
        printf("Message received\n");
    } else {
        printf("Failed to receive message\n");
        return;
    }

    //Process received message
    if (message.flags & TWAI_MSG_FLAG_EXTD) {
        printf("Message is in Extended Format\n");
    } else {
        printf("Message is in Standard Format\n");
    }
    printf("ID:%X, data:", message.identifier);
    if (!(message.flags & TWAI_MSG_FLAG_RTR)) {
        for (int i = 0; i < message.data_length_code; i++) {
            printf("%X ", message.data[i]);
        }
        printf("\n");
    }
}

esp_err_t transmit(twai_message_t *message) {
    //Queue message for transmission
    if (twai_transmit(message, pdMS_TO_TICKS(1000)) == ESP_OK) {
        printf("Message queued for transmission\n");
        return ESP_OK;
    } else {
        printf("Failed to queue message for transmission\n");
        return ESP_FAIL;
    }
}

void app_main()
{
    uint32_t my_alerts = 0;
    esp_err_t ret; 
    twai_message_t message;
    message.identifier = 0x18FF1720;
    message.flags = TWAI_MSG_FLAG_EXTD;
    message.data_length_code = 8;
    for (int i = 0; i < 8; i++) {
        message.data[i] = 0xff;
    }
    uint32_t cnt = 0;

    //Initialize configuration structures using macro initializers
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(GPIO_NUM_5, GPIO_NUM_35, TWAI_MODE_NORMAL);
    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_250KBITS();
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

    ret = twai_reconfigure_alerts(TWAI_ALERT_ALL || TWAI_ALERT_AND_LOG, &my_alerts);
    if (ret == ESP_OK) {
        printf("Alerts reconfigured\n");
    } else {
        printf("Failed to reconfigure alerts. Err:%d", ret);
    }
    
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
        message.data[0] = (uint8_t)(cnt >>24);
        message.data[1] = (uint8_t)(cnt >>16);
        message.data[2] = (uint8_t)(cnt >>8);
        message.data[3] = (uint8_t)(cnt);
        transmit(&message);
        ret = twai_read_alerts(&my_alerts, 10);
        if ( ret == ESP_OK) {
            printf("ESP_OK. alerts:%x\n", my_alerts);
        } else if (ret == ESP_ERR_TIMEOUT) {
            printf("ESP_ERR_TIMEOUT\n");
        } else {
            printf("unexpected twai alert: %d\n", ret);
        }
        cnt++;
    };
    

}
