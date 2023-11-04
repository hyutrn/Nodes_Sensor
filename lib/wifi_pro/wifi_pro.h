#ifndef _WIFI_PRO_H_
#define _WIFI_PRO_H_

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "nvs_flash.h"
#include "string.h"
#include <sys/param.h>
#include "esp_wifi.h"
#include <stdbool.h>

#include "lwip/err.h"
#include "lwip/sys.h"
#include "cJSON.h"
#include "server_cfg.h"
#include "project_system_files.h"
#include "mqtt_cfg.h"

//ssid và password cho kết nối STA 
char sta_ssid[20];
char sta_password[20];
int sta_flag;
void scann();
esp_err_t wifi_sta_mode(void);
void sta_connect_handler(void* arg, esp_event_base_t event_base,
                               int32_t event_id, void* event_data);
void ap_connect_handler(void* arg, esp_event_base_t event_base,
                               int32_t event_id, void* event_data);
void initialise_wifi(void);

#endif 
