/*
ESP32-EVB_clock - demo app to test whether the ESP32-EVB crystal is accurate enough or if we need an external RTC

TEST: how well it keeps time over the weekend, and how many sync events it took to keep time. (currently lagging 0-1 s)
TODO: keep time w/o syncing and compare to time.gov
*/
#include <stdio.h>
#include <string.h>
#include <driver/gpio.h>
#include <time.h>
#include <sys/time.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/event_groups.h>
#include <hd44780.h>
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_attr.h"
#include "esp_sleep.h"
#include "nvs_flash.h"
#include "esp_sntp.h"
#include "protocol_examples_common.h"

static const char* TAG = "clock"; // logging system tag

#ifndef INET6_ADDRSTRLEN
#define INET6_ADDRSTRLEN 48
#endif
#ifndef CONFIG_SNTP_TIME_SERVER
#define CONFIG_SNTP_TIME_SERVER "pool.ntp.org"
#endif

//#define NR_OF_IP_ADDRESSES_TO_WAIT_FOR 1

/* Variable holding number of times ESP32 restarted since first boot.
 * It is placed into RTC memory using RTC_DATA_ATTR and
 * maintains its value when ESP32 wakes from deep sleep.
 */
RTC_DATA_ATTR static int boot_count = 0;
RTC_DATA_ATTR static int sync_count = 0;

static void obtain_time(void);
static void initialize_sntp(void);
struct tm timeinfo;
time_t now;

#ifdef CONFIG_SNTP_TIME_SYNC_METHOD_CUSTOM
void sntp_sync_time(struct timeval *tv)
{
   settimeofday(tv, NULL);
   ESP_LOGI(TAG, "Time is synchronized from custom code");
   sntp_set_sync_status(SNTP_SYNC_STATUS_COMPLETED);
}
#endif

hd44780_t lcd = {
    .write_cb = NULL,
    .font = HD44780_FONT_5X8,
    .lines = 4, //TEST
    .pins = {
        .rs = GPIO_NUM_13,
        .e  = GPIO_NUM_12,
        .d4 = GPIO_NUM_14,
        .d5 = GPIO_NUM_27,
        .d6 = GPIO_NUM_26,
        .d7 = GPIO_NUM_25,
        .bl = HD44780_NOT_USED
    }
};

void spinner(uint8_t col, uint8_t row, uint8_t* ptr) {
    const char spinchars[] = {'-', (char)221, '/', (char)201,  '|', (char)205, (char)164};
    hd44780_gotoxy(&lcd, col, row);
    hd44780_putc(&lcd, spinchars[*ptr]);
    *ptr = (*ptr + 1) % sizeof(spinchars);
}

void time_sync_notification_cb(struct timeval *tv)
{
    static uint8_t sync_ptr = 0;
    ESP_LOGI(TAG, "Notification of a time synchronization event");
    spinner(18, 2, &sync_ptr); // TEST/FIXME
    sync_count++;
}

// len includes delmiinter and null terminator
void puti(const hd44780_t *plcd, const uint8_t col, const uint8_t row, const uint8_t len, const char delim, int val) {
    char str[8];
    snprintf(str, len, "%02u%c", val, delim);
    // str[len - 1] = delim;
    str[len] = 0; // null-terminate string
    hd44780_gotoxy(plcd, col, row);
    hd44780_puts(plcd, str);
}

static uint32_t get_time_sec()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec;
}

void lcd_test(void *pvParameters)
{
 
    ESP_LOGI(TAG, "enter lcd_test");
    ESP_ERROR_CHECK(hd44780_init(&lcd));
    hd44780_gotoxy(&lcd, 0,0);
    hd44780_puts(&lcd, "reboots:");
    hd44780_gotoxy(&lcd, 0,2); // TEST
    hd44780_puts(&lcd, "syncs:");
    uint8_t time_spinner = 0;
    while (1)
    {
        time(&now);
        localtime_r(&now, &timeinfo);
        puti(&lcd, 0, 1, 6, '-', timeinfo.tm_year+1900); 
        puti(&lcd, 5, 1, 4, '-', timeinfo.tm_mon+1);
        puti(&lcd, 8, 1, 4, 'T', timeinfo.tm_mday+1);
        puti(&lcd, 11, 1, 4, ':', timeinfo.tm_hour);
        puti(&lcd, 14, 1, 4, ':', timeinfo.tm_min);
        puti(&lcd, 17, 1, 4, '.', timeinfo.tm_sec);

        spinner(19,0, &time_spinner); 

        vTaskDelay(pdMS_TO_TICKS(1000));
        ESP_LOGI(TAG, "puts(time)");
    }
}

void app_main()
{
    
    ++boot_count;
    ESP_LOGI(TAG, "Boot count: %d", boot_count);

    time(&now); // get system time in epoch clock format
    localtime_r(&now, &timeinfo); // push time to timeinfo in local format

    xTaskCreate(lcd_test, "lcd_test", configMINIMAL_STACK_SIZE * 3, NULL, 5, NULL);

    // Is time set? If not, tm_year will be (1970 - 1900).
    if (timeinfo.tm_year < (2016 - 1900)) {
        ESP_LOGI(TAG, "Time is not set yet. Connecting to WiFi and getting time over NTP.");
        obtain_time();
        // update 'now' variable with current time
        time(&now);
        vTaskDelay(pdMS_TO_TICKS(10));
     } else {
        ESP_LOGI(TAG, "Time is set.");
     }

#ifdef CONFIG_SNTP_TIME_SYNC_METHOD_SMOOTH
    else {
        // add 500 ms error to the current system time.
        // Only to demonstrate a work of adjusting method!
        {
            ESP_LOGI(TAG, "Add a error for test adjtime");
            struct timeval tv_now;
            gettimeofday(&tv_now, NULL);
            int64_t cpu_time = (int64_t)tv_now.tv_sec * 1000000L + (int64_t)tv_now.tv_usec;
            int64_t error_time = cpu_time + 500 * 1000L;
            struct timeval tv_error = { .tv_sec = error_time / 1000000L, .tv_usec = error_time % 1000000L };
            settimeofday(&tv_error, NULL);
        }

        ESP_LOGI(TAG, "Time was set, now just adjusting it. Use SMOOTH SYNC method.");
        obtain_time();
        // update 'now' variable with current time
        time(&now);
    }
#endif

    char strftime_buf[64];

    // Set timezone to Eastern Standard Time and print local time
    setenv("TZ", "EST5EDT,M3.2.0/2,M11.1.0", 1);
    tzset();
    localtime_r(&now, &timeinfo);
    strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
    ESP_LOGI(TAG, "current New York time: %s", strftime_buf);

    // show boots
    char bootc[6];

    if (sntp_get_sync_mode() == SNTP_SYNC_MODE_SMOOTH) {
        struct timeval outdelta;
        while (sntp_get_sync_status() == SNTP_SYNC_STATUS_IN_PROGRESS) {
            adjtime(NULL, &outdelta);
            ESP_LOGI(TAG, "Waiting for adjusting time ... outdelta = %jd sec: %li ms: %li us",
                        (intmax_t)outdelta.tv_sec,
                        outdelta.tv_usec/1000,
                        outdelta.tv_usec%1000);
            vTaskDelay(2000 / portTICK_PERIOD_MS);
        }
    }

    vTaskDelay(4000 / portTICK_PERIOD_MS);
    
    // snprintf(bootc, 5, "%u", boot_count);
    // bootc[sizeof(bootc) - 1] = 0; // null-terminate string?

    // hd44780_gotoxy(&lcd, 6,0);
    // hd44780_puts(&lcd, bootc);
    puti(&lcd, 8, 0, 8, ' ', boot_count); 
    vTaskDelay(10 / portTICK_PERIOD_MS); // TEST
    puti(&lcd, 8, 2, 8, ' ', sync_count); // TEST
    const int deep_sleep_sec = 4;
    ESP_LOGI(TAG, "Enter deep sleep for %d s", deep_sleep_sec);
 //   hd44780_control(&lcd, false, false, false);
    esp_deep_sleep(1000000LL * deep_sleep_sec);
}

static void obtain_time(void)
{
    ESP_ERROR_CHECK( nvs_flash_init() );
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK( esp_event_loop_create_default() );

    /**
     * NTP server address could be aquired via DHCP,
     * see following menuconfig options:
     * 'LWIP_DHCP_GET_NTP_SRV' - enable STNP over DHCP
     * 'LWIP_SNTP_DEBUG' - enable debugging messages
     *
     * NOTE: This call should be made BEFORE esp aquires IP address from DHCP,
     * otherwise NTP option would be rejected by default.
     */
#ifdef LWIP_DHCP_GET_NTP_SRV
    sntp_servermode_dhcp(1);      // accept NTP offers from DHCP server, if any
#endif

    /* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
     * Read "Establishing Wi-Fi or Ethernet Connection" section in
     * examples/protocols/README.md for more information about this function.
     */
    ESP_ERROR_CHECK(example_connect());

    initialize_sntp();

    // wait for time to be set
    time_t now = 0;
    struct tm timeinfo = { 0 };
    int retry = 0;
    const int retry_count = 15;
    while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET && ++retry < retry_count) {
        ESP_LOGI(TAG, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
    time(&now);
    localtime_r(&now, &timeinfo);

    ESP_ERROR_CHECK( example_disconnect() );
}

static void initialize_sntp(void)
{
    ESP_LOGI(TAG, "Initializing SNTP");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);

/*
 * If 'NTP over DHCP' is enabled, we set dynamic pool address
 * as a 'secondary' server. It will act as a fallback server in case that address
 * provided via NTP over DHCP is not accessible
 */
#if LWIP_DHCP_GET_NTP_SRV && SNTP_MAX_SERVERS > 1
    sntp_setservername(1, "pool.ntp.org");

#if LWIP_IPV6 && SNTP_MAX_SERVERS > 2          // statically assigned IPv6 address is also possible
    ip_addr_t ip6;
    if (ipaddr_aton("2a01:3f7::1", &ip6)) {    // ipv6 ntp source "ntp.netnod.se"
        sntp_setserver(2, &ip6);
    }
#endif  /* LWIP_IPV6 */

#else   /* LWIP_DHCP_GET_NTP_SRV && (SNTP_MAX_SERVERS > 1) */
    // otherwise, use DNS address from a pool
    sntp_setservername(0, CONFIG_SNTP_TIME_SERVER);
#endif

    sntp_set_time_sync_notification_cb(time_sync_notification_cb);
#ifdef CONFIG_SNTP_TIME_SYNC_METHOD_SMOOTH
    sntp_set_sync_mode(SNTP_SYNC_MODE_SMOOTH);
#endif
    sntp_init();

    ESP_LOGI(TAG, "List of configured NTP servers:");

    for (uint8_t i = 0; i < SNTP_MAX_SERVERS; ++i){
        if (sntp_getservername(i)){
            ESP_LOGI(TAG, "server %d: %s", i, sntp_getservername(i));
        } else {
            // we have either IPv4 or IPv6 address, let's print it
            char buff[INET6_ADDRSTRLEN];
            ip_addr_t const *ip = sntp_getserver(i);
            if (ipaddr_ntoa_r(ip, buff, INET6_ADDRSTRLEN) != NULL)
                ESP_LOGI(TAG, "server %d: %s", i, buff);
        }
    }
}
