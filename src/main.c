#include "mgos.h"
#include "mqtt.h"
#include "rpc.h"
#include "mgos_prometheus_metrics.h"

static uint32_t s_things=0;

static void counter_timer(void *user_data) {
  s_things++;
}

static void prometheus_metrics_fn(struct mg_connection *nc, void *user_data) {
  mgos_prometheus_metrics_printf(nc, COUNTER,
    "things_total", "Total things counted",
    "%u", s_things);
  (void) user_data;
}

enum mgos_app_init_result mgos_app_init(void) {
  // Initialize MQTT
  mqtt_init();

  // Initialize RPCs
  rpc_init();

  // Install a repeating timer at 250ms that calls 'counter_timer'
  mgos_set_timer(250, true, counter_timer, NULL);

  // Install a metrics handler that calls 'prometheus_metrics_fn'
  mgos_prometheus_metrics_add_handler(prometheus_metrics_fn, NULL);

  return MGOS_APP_INIT_SUCCESS;
}
