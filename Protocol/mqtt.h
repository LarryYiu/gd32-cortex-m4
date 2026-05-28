#ifndef __MQTT_H__
#define __MQTT_H__

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
