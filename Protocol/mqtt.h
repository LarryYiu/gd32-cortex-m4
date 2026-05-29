#ifndef __MQTT_H__
#define __MQTT_H__

#define MQTT_HOST "mqtts.heclouds.com"
#define MQTT_PORT "1883"
#define MQTT_DEVICE_NAME "GD32Board"
#define MQTT_PRODUCT_ID "4m3RoDJR8n"
#define MQTT_DEVICE_TOKEN                                                                               \
    "version=2018-10-31&res=products%2F4m3RoDJR8n%2Fdevices%2FGD32Board&et=1830268800&method=md5&sign=" \
    "qIQ8T1heqVLOZ23gunAFjg%3D%3D"

#define MQTT_USERCFG_CMD(keepAlive, qos, retain)                                                                     \
    "AT+MQTTUSERCFG=0," keepAlive ",\"" MQTT_DEVICE_NAME "\",\"" MQTT_PRODUCT_ID "\",\"" MQTT_DEVICE_TOKEN "\"," qos \
    "," retain ",\"\"\r\n"

// #define MQTT_CONN_CMD(reconnect) "AT+MQTTCONN=0,\"mqtts.heclouds.com\",1883,1\r\n"
#define MQTT_CONN_CMD(reconnect) "AT+MQTTCONN=0,\"" MQTT_HOST "\"," MQTT_PORT "," reconnect "\r\n"
typedef struct MQTT_Client MQTT_Client_t;
typedef struct MQTT_Request MQTT_Request_t;
typedef struct MQTT_Response MQTT_Response_t;

struct MQTT_Client
{
    char host[32];
    char port[8];
    char device_id[32];
    char product_id[32];
    char token[128];
};

struct MQTT_Request
{
    char topic[128];
    char payload[256];
};

struct MQTT_Response
{
    char id[16];
    char version[16];
    char content[256];
};

#endif // __MQTT_H__
