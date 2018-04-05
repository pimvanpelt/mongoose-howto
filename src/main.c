#include "mgos.h"
#include "mqtt.h"

enum mgos_app_init_result mgos_app_init(void) {
  // Initialize MQTT
  mqtt_init();

  return MGOS_APP_INIT_SUCCESS;
}
