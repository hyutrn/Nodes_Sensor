#ifndef DHT11_H_
#define DHT11_H_

#include "esp_timer.h"
#include "driver/gpio.h"
#include "rom/ets_sys.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "cJSON.h"

#include "wifi_pro.h"
#include "mqtt_cfg.h"

enum dht11_status {
    DHT11_CRC_ERROR = -2,
    DHT11_TIMEOUT_ERROR,
    DHT11_OK
};

struct dht11_reading {
    int status;
    int temperature;
    int humidity;
};

struct moisture_reading 
{
    int soil_moisture;
};

struct light_reading {
    int light_intensity;
}

//hoàn thiện cho cả moisture sensor và lighting sensor 

void dht11_init(gpio_num_t);
void moisture_init(gpio_num_t);
void light_init(gpio_num_t);
struct dht11_reading dht11_read();

#endif