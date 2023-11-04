set(SOURCES "mqtt_cfg.c")
set(pri_set wifi_pro)

idf_component_register(SRCS ${SOURCES}
                    INCLUDE_DIRS "."
                    REQUIRES ${pri_set}
                    REQUIRES nvs_flash
                    REQUIRES server_cfg
                    REQUIRES wifi_pro
                    REQUIRES mqtt
                    REQUIRES dht11
)
