#include "mqtt.h"

static void mqtt_publish_broadcast_stat(const char *stat, const char *msg) {
  char topic[80];
  snprintf(topic, sizeof(topic)-1, "%s/%s", MQTT_TOPIC_BROADCAST_STAT, stat);
  mgos_mqtt_pub((char*)topic, (char*)msg, strlen(msg), 0, false);
  LOG(LL_INFO, ("Sent topic='%s' msg='%s'", topic, msg));
}

static void mqtt_broadcast_cmd_id() {
  char resp[300];
  struct mgos_net_ip_info ip_info;
  char sta_ip[16], ap_ip[16];

  memset(sta_ip, 0, sizeof(sta_ip));
  memset(ap_ip, 0, sizeof(ap_ip));

  if (mgos_net_get_ip_info(MGOS_NET_IF_TYPE_WIFI, MGOS_NET_IF_WIFI_STA, &ip_info))
    mgos_net_ip_to_str(&ip_info.ip, sta_ip);
  if (mgos_net_get_ip_info(MGOS_NET_IF_TYPE_WIFI, MGOS_NET_IF_WIFI_AP, &ip_info))
    mgos_net_ip_to_str(&ip_info.ip, ap_ip);

  snprintf(resp, sizeof(resp)-1, "{\"deviceid\": \"%s\", \"macaddress\": \"%s\", \"sta_ip\": \"%s\", \"ap_ip\": \"%s\", \"app\": \"%s\", \"arch\": \"%s\", \"uptime\": %lu}",
    mgos_sys_config_get_device_id(),
    mgos_sys_ro_vars_get_mac_address(),
    sta_ip,
    ap_ip,
    MGOS_APP,
    mgos_sys_ro_vars_get_arch(), (unsigned long) mgos_uptime());

  mqtt_publish_broadcast_stat("id", resp);
}

static void mqtt_cb(struct mg_connection *nc, const char *topic, int topic_len, const char *msg, int msg_len, void *ud) {
  LOG(LL_INFO, ("Received topic='%.*s' msg='%.*s'", topic_len, topic, msg_len, msg));
  mqtt_broadcast_cmd_id();
  (void) nc;
  (void) ud;
}

static void mqtt_ev(struct mg_connection *nc, int ev, void *ev_data, void *user_data) {
  switch (ev) {
    case MG_EV_MQTT_CONNACK:
      mqtt_broadcast_cmd_id();
    break;
  }
  (void) nc;
  (void) ev_data;
  (void) user_data;
}


void mqtt_publish_stat(const char *stat, const char *msg) {
  char topic[80];
  snprintf(topic, sizeof(topic)-1, "%s%s/stat/%s", MQTT_TOPIC_PREFIX, mgos_sys_config_get_device_id(), stat);
  mgos_mqtt_pub((char*)topic, (char*)msg, strlen(msg), 0, false);
  LOG(LL_INFO, ("Sent topic='%s' msg='%s'", topic, msg));
}

void mqtt_init() {
  char topic[100];

  mgos_mqtt_sub(MQTT_TOPIC_BROADCAST_CMD, mqtt_cb, NULL);
  snprintf(topic, sizeof(topic)-1, "%s/%s", MQTT_TOPIC_BROADCAST_CMD, MGOS_APP);
  mgos_mqtt_sub(topic, mqtt_cb, NULL);
  mgos_mqtt_add_global_handler(mqtt_ev, NULL);

  return;
}
