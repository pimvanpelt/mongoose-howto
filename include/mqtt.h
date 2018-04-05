#pragma once

#include "mgos.h"
#include "mgos_mqtt.h"
#include "mgos_config.h"
#include "mgos_ro_vars.h"

#define MQTT_TOPIC_PREFIX         ""
#define MQTT_TOPIC_BROADCAST_CMD  "/mongoose/broadcast"
#define MQTT_TOPIC_BROADCAST_STAT "/mongoose/broadcast/stat"

void mqtt_init();
void mqtt_publish_stat(const char *stat, const char *msg);
