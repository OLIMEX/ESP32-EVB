
#include <driver/i2c.h>

#define LOG_LOCAL_LEVEL ESP_LOG_DEBUG //FIXME this isn't working
#include "esp_log.h"
#include "esp_err.h"
#include "esp_check.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdio.h>
#include <SparkFunSX1509.hpp> //Click here for the library: http://librarymanager/All#SparkFun_SX1509

#ifndef APP_CPU_NUM
#define APP_CPU_NUM PRO_CPU_NUM
#endif

#define SDA_PIN GPIO_NUM_3
#define SCL_PIN GPIO_NUM_16
#define RESET_PIN GPIO_NUM_17
#define SX1509_ADDR 0x3E
// SX1509 Pin definition:
const uint8_t SX1509_LED_PIN = 15; // LED to SX1509's pin 15
const uint8_t SX1508_BTN_PIN = 0;

static const char *TAG = "sx1509_example";

SX1509 io;

void task(void *ignore)
{
    esp_log_level_set("sx1509", ESP_LOG_DEBUG);        // set sx1509 to debug. This works, but I'd rather use the LOG_LOCAL_LEVEL macro
    esp_err_t ret = io.begin(SX1509_ADDR, SDA_PIN, SCL_PIN, RESET_PIN);
    ESP_LOGD(TAG, "io.begin() returned %i", ret);
 	
    while( ret != ESP_OK ) 
    {
        {
            ESP_LOGE( TAG, "Failed to communicate. Check wiring and address of SX1509. Retrying...");
            ret = io.init();
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
       
    // Set up the SX1509's clock to use the internal 2MHz
    // oscillator. The second parameter divides the oscillator
    // clock to generate a slower LED clock. 4 divides the 2MHz
    // clock by 2 ^ (4-1) (8, ie. 250kHz). The divider parameter
    // can be anywhere between 1-7.
    // i.e. 4: ClkX = 2E6 Hz/(2^(4-1)) = 2E6 Hz/8 = 250 KHz
    // 2: ClkX = 2E6 Hz/2 = 1E6 Hz = 1 MHz
    ret = io.clock(INTERNAL_CLOCK_2MHZ, 1);
    if (ret != ESP_OK) 
    {
        ESP_LOGE(TAG, "io.clock(INTERNAL_CLOCK_2MHZ, 4) returned %i", ret); 
    }
    
    ret = io.pinMode(SX1509_LED_PIN, OUTPUT); // Set LED pin to OUTPUT
    if (ret != ESP_OK) ESP_LOGE(TAG, "io.pinMode(%i, OUTPUT) returned %i", SX1509_LED_PIN, ret); 
    //    ESP_LOGD(TAG, "io.pinMode(%i, OUTPUT) returned %i", SX1509_LED_PIN, ret); 
    
    // Blink the LED pin -- ~1000 ms LOW, ~500 ms HIGH:
    // Blink time resolution is 5 bit
    ret = io.blink(SX1509_LED_PIN, 900, 100);
    if (ret != ESP_OK) ESP_LOGE(TAG, "io.blink(%i, 1000, 500) returned %i", SX1509_LED_PIN, ret); 
    // The timing parameters are in milliseconds, but they
    // aren't 100% exact. The library will estimate to try to
    // get them as close as possible. Play with the clock
    // divider to maybe get more accurate timing.
    vTaskDelay(pdMS_TO_TICKS(3000));

    // TEST now test the button
    // first turn off blinking
    ret = io.digitalWrite(SX1509_LED_PIN, LOW); // TEST
    while(true) vTaskDelay(pdMS_TO_TICKS(10));
    
    if (ret != ESP_OK) ESP_LOGE(TAG, "io.digitalWrite(SX1509_LED_PIN, LOW) returned %i", ret); 
    // set up the button as input
    ret = io.pinMode(SX1508_BTN_PIN, INPUT_PULLUP); // TEST
    if (ret != ESP_OK) ESP_LOGE(TAG, "io.pinMode(SX1508_BTN_PIN, INPUT_PULLUP) returned %i", ret); 
     
    while( true ) 
    {
        if (io.digitalRead(SX1508_BTN_PIN) == LOW) {
            io.digitalWrite(SX1509_LED_PIN, HIGH);
        } else {
            io.digitalWrite(SX1509_LED_PIN, LOW);
        }
        vTaskDelay(pdMS_TO_TICKS(2));
    }
       
        
}

extern "C" void app_main()
{
    ESP_LOGI(TAG, "SX1509 Example");
    // Start task
    xTaskCreatePinnedToCore(task, TAG, configMINIMAL_STACK_SIZE * 8, NULL, 5, NULL, APP_CPU_NUM);


}
