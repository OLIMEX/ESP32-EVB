/*
 * my_olimexikus_CAN.c
 *
 *  Created on: 14.06.2017
 *      Author: rudi ;-)
 *
 *      Olimex EVB REV B
 *      with CAN
 *      CAN Rx = GPIO 35
 *      CAN Tx = GPIO  5
 *      Node   = 197 ( you can change all in Kconfig )
 *
 *      done ;-)
 *      ....
 *      code comes later to olimex github
 *      after PR visit it here
 *      https://github.com/OLIMEX/ESP32-EVB/tree/master/SOFTWARE
 *      have phun
 *      best wishes
 *      rudi ;-)
 */

#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"

#include "CAN.h"
#include "CAN_config.h"

/* brief  : rudi
 * content: Set the CAN Speed over Kconfig
 * you can use menuconfig for this
 * and you can expand like you need
 * you can also use your own
 * cfg - look in the main/cfg folder
 * you have then change by self
 */
 #ifndef CONFIG_ESPCAN
 #error for this demo you must enable and configure ESPCan in menuconfig
 #endif

#ifdef CONFIG_CAN_SPEED_100KBPS
#define CONFIG_SELECTED_CAN_SPEED CAN_SPEED_100KBPS
#endif

#ifdef CONFIG_CAN_SPEED_125KBPS
#define CONFIG_SELECTED_CAN_SPEED CAN_SPEED_125KBPS
#endif

#ifdef CONFIG_CAN_SPEED_250KBPS
#define CONFIG_SELECTED_CAN_SPEED CAN_SPEED_250KBPS
#endif

#ifdef CONFIG_CAN_SPEED_500KBPS
#define CONFIG_SELECTED_CAN_SPEED CAN_SPEED_500KBPS
#endif

#ifdef CONFIG_CAN_SPEED_800KBPS
#define CONFIG_SELECTED_CAN_SPEED CAN_SPEED_800KBPS
#endif

#ifdef CONFIG_CAN_SPEED_1000KBPS
#define CONFIG_SELECTED_CAN_SPEED CAN_SPEED_1000KBPS
#endif

#ifdef CONFIG_CAN_SPEED_USER_KBPS
#define CONFIG_SELECTED_CAN_SPEED CONFIG_CAN_SPEED_USER_KBPS_VAL /* per menuconfig */
#endif

/* brief  : rudi
 * content: config CAN Speed, Tx, Rx Pins taken from Kconfig
 * over menuconfig you can set the cfg
 * defines are used in the head of code then
 * if you change to user cfg
 * you can change here too by your self
 * how you need this.
*/
CAN_device_t CAN_cfg = {
	.speed		= CONFIG_SELECTED_CAN_SPEED,	// CAN Node baudrade
	.tx_pin_id 	= CONFIG_ESP_CAN_TXD_PIN_NUM,	// CAN TX pin example menuconfig GPIO_NUM_5
	.rx_pin_id 	= CONFIG_ESP_CAN_RXD_PIN_NUM,	// CAN RX pin example menuconfig GPIO_NUM_35 ( Olimex )
	.rx_queue	= NULL,							// FreeRTOS queue for RX frames
};

esp_err_t event_handler(void *ctx, system_event_t *event)
{
    return ESP_OK;
}

void task_CAN( void *pvParameters ){
    (void)pvParameters;

    //frame buffer
    CAN_frame_t __RX_frame;

    //create CAN RX Queue
    CAN_cfg.rx_queue = xQueueCreate(10,sizeof(CAN_frame_t));

    //start CAN Module
    CAN_init();
    printf("Can Init done - wait now..\n");
    while (1){
        //receive next CAN frame from queue
        if(xQueueReceive(CAN_cfg.rx_queue,&__RX_frame, 3*portTICK_PERIOD_MS)==pdTRUE){

        	//do stuff!
        	// printf("New Frame from 0x%08x, DLC %d, dataL: 0x%08x, dataH: 0x%08x \r\n",__RX_frame.MsgID,  __RX_frame.FIR.B.DLC, __RX_frame.data.u32[0],__RX_frame.data.u32[1]);

			printf("Frame from : 0x%08x, DLC %d \n", __RX_frame.MsgID, __RX_frame.FIR.B.DLC);
			printf("D0: 0x%02x, ", __RX_frame.data.u8[0]);
			printf("D1: 0x%02x, ", __RX_frame.data.u8[1]);
			printf("D2: 0x%02x, ", __RX_frame.data.u8[2]);
			printf("D3: 0x%02x, ", __RX_frame.data.u8[3]);
			printf("D4: 0x%02x, ", __RX_frame.data.u8[4]);
			printf("D5: 0x%02x, ", __RX_frame.data.u8[5]);
			printf("D6: 0x%02x, ", __RX_frame.data.u8[6]);
			printf("D7: 0x%02x\n", __RX_frame.data.u8[7]);
			printf("==============================================================================\n");

        	//loop back frame
        	// CAN_write_frame(&__RX_frame);
        }
    }
}


void task_CAN_TX(void* pvParameters) {

   CAN_frame_t __TX_frame;
   uint32_t counter = 0;

      __TX_frame.MsgID = CONFIG_ESP_CAN_NODE_ITSELF;
      __TX_frame.FIR.B.DLC   =  8;
      __TX_frame.data.u8[0]  = 'E';
      __TX_frame.data.u8[1]  = 'S';
      __TX_frame.data.u8[2]  = 'P';
      __TX_frame.data.u8[3]  = '-';
      __TX_frame.data.u8[4]  = 'C';
      __TX_frame.data.u8[5]  = 'A';
      __TX_frame.data.u8[6]  = 'N';
      __TX_frame.data.u8[7]  = counter;

while(1) {
      __TX_frame.data.u8[7] = counter;
      CAN_write_frame(&__TX_frame);
      printf("frame send [%3d]\r", counter);
      fflush(stdout);
      vTaskDelay( 1000 / portTICK_PERIOD_MS);  // to see ( printf on receiver side ) what happend..
      counter++;
      if (counter >= 256) counter = 0;
  }
}

void app_main(void)
{
	// wait for all print Infos
	vTaskDelay( 1000 / portTICK_PERIOD_MS);
    // Create CAN receive task
    // xTaskCreate(&task_CAN, "CAN", 2048, NULL, 5, NULL);

	/*brief: rudi
	* if you have "activate ESPCan"
	* then the code here runs..
	*
	*/
	#ifdef CONFIG_ESPCAN
	 printf("**********   CAN TESTINGS **********\n");
	 printf("Olimex EVB Rev B Board \n");
	 printf("ESPCan configured by this Data:\n");
	 printf("Node           : 0x%03x\n", CONFIG_ESP_CAN_NODE_ITSELF);
	 printf("CAN RXD PIN NUM: %d\n", CONFIG_ESP_CAN_RXD_PIN_NUM);
	 printf("CAN TXD PIN NUM: %d\n", CONFIG_ESP_CAN_TXD_PIN_NUM);
	 printf("CAN SPEED      : %d KBit/s\n", CONFIG_SELECTED_CAN_SPEED);

	 #ifdef CONFIG_CAN_SPEED_USER_KBPS
	  printf("kBit/s setting was done by User\n");
	 #endif

	//Create CAN receive task
      xTaskCreate(&task_CAN, "CAN", 2048, NULL, 5, NULL);

    /* check if we have set a test frame to send in Kconfig */
	 #ifdef CONFIG_CAN_TEST_SENDING_ENABLED
      printf("Test Frame sending is enabled\n");
	  vTaskDelay( 1000 / portTICK_PERIOD_MS);
	  xTaskCreate(&task_CAN_TX, "task_CAN_TX", 2048, NULL, 5, NULL);
	 #endif

	#endif
}

