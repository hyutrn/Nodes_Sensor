#ifndef _MQTT_CFG_H_
#define _MQTT_CFG_H_

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
//#include "protocol_examples_common.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "esp_log.h"
#include "mqtt_client.h"

#include "lwip/err.h"
#include "lwip/sys.h"
#include "wifi_pro.h"
#include "server_cfg.h"
#include "dht11.h"

void log_error(const char *message, int error_code);
void mqtt_handler(void *handler_argr, esp_event_base_t base, int32_t event_id, void *event_data);
void mqtt_setup(void);
void mqtt_start();
#endif 