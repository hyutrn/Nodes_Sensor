#include "mqtt_cfg.h" 

void log_error(const char *message, int error_code) {
    if(error_code != 0) {
        printf("Last error %s: 0x%x\n", message, error_code);
    }
}

void mqtt_handler(void *handler_argr, esp_event_base_t base, int32_t event_id, void *event_data) {
    printf("Event base = %s, event_id =%d\n", base, event_id);
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    switch ((esp_mqtt_event_id_t)event_id) {
    case MQTT_EVENT_CONNECTED:
        //xử lý khi kết nối thành công 
        printf("MQTT_EVENT_CONNECTED\n");
        dht11_init(GPIO_NUM_4);
        while(1) {
            //Xử lý Read Sensor Data tại đây 
            //....
            int air_temperature, air_humidity, status;
            status = dht11_read().status;
            air_temperature = dht11_read().temperature;
            air_humidity = dht11_read().humidity;
            printf("Temperature is %d \n", air_temperature);
            printf("Humidity is %d\n", air_humidity);
            printf("Status code is %d\n", status);
            //create jSON 
            cJSON *root = cJSON_CreateObject();
            if(root == NULL) {
                printf("Failed to create json\n");
            }

            cJSON_AddNumberToObject(root, "air_temperature", air_temperature);
            cJSON_AddNumberToObject(root, "air_humidity", air_humidity);

            char *jsonString = cJSON_PrintUnformatted(root);

            if (jsonString != NULL) {
                printf("JSON:\n%s\n", jsonString);
                //Gửi dữ liệu từ node tới gateway 
                //Cần xử lý ${id_node}
                msg_id = esp_mqtt_client_publish(client, "/node/sensors/", jsonString, 0, 1, 0);
                free(jsonString);
            }
            free(jsonString);
            cJSON_Delete(root);
            vTaskDelay(1000/ portTICK_PERIOD_MS); //delay 1s
        }
        break;
    case MQTT_EVENT_DISCONNECTED:
        //Xử lý khi mất kết nối
        printf("MQTT_EVENT_DISCONNECTED\n");
        break;

    case MQTT_EVENT_SUBSCRIBED:
        //xử lý khi subcribed topic thành công 
        printf("MQTT_EVENT_SUBSCRIBED, msg_id=%d\n", event->msg_id);
        msg_id = esp_mqtt_client_publish(client, "/topic/qos0", "data", 0, 0, 0);
        printf("sent publish successful, msg_id=%d\n", msg_id);
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        //Xử lý khi unsubcribed thành công
        printf("MQTT_EVENT_UNSUBSCRIBED, msg_id=%d\n", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        //Xử lý khi published dữ liệu thành công 
        printf("MQTT_EVENT_PUBLISHED, msg_id=%d\n", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
        //Xử lý khi nhận được dữ liệu từ broker 
        printf("MQTT_EVENT_DATA\n");
        printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
        printf("DATA=%.*s\r\n", event->data_len, event->data);
        break;
        case MQTT_EVENT_ERROR:
        //Xử lý khi xảy ra lỗi 
        printf("MQTT_EVENT_ERROR\n");
        if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
            log_error("reported from esp-tls", event->error_handle->esp_tls_last_esp_err);
            log_error("reported from tls stack", event->error_handle->esp_tls_stack_err);
            log_error("captured as transport's socket errno",  event->error_handle->esp_transport_sock_errno);
            printf("Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));

        }
        break;
    default:
        printf("Other event id:%d\n", event->event_id);
        break;
    }
}
void mqtt_setup(void) {
    //xử lý thêm phần url 
    //Cho phép thay đổi url qua tham số truyền vào (hoặc Kconfig)
    esp_mqtt_client_config_t mqtt_cfg = {
        .uri = "mqtt://mqtt.eclipseprojects.io",
        .port = 1883,
        .username = "esp32",
        .password = NULL
    };
    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_handler, NULL);
    esp_mqtt_client_start(client);
}

void mqtt_start() {
    //CONFIG_BROKER_URL = url 
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    mqtt_setup();
}