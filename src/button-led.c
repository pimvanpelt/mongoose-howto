#include "mgos.h"
#include "mqtt.h"
#include "mgos_gpio.h"
#include "mgos_config.h"

void button_handler(int gpio, void *user_data) {
  if (user_data)
    mqtt_publish_stat("button", "RPC pressed the button");
  else
    mqtt_publish_stat("button", "User pressed the button");

  mgos_gpio_toggle(mgos_sys_config_get_app_led());
  (void) user_data;
}

bool led_getstate(void) {
  return mgos_gpio_read_out(mgos_sys_config_get_app_led());
}

void button_led_init() {
  // Set up LED
  mgos_gpio_set_mode(mgos_sys_config_get_app_led(), MGOS_GPIO_MODE_OUTPUT);
  mgos_gpio_write(mgos_sys_config_get_app_led(), 0);

  // Set up button
  mgos_gpio_set_mode(mgos_sys_config_get_app_button(), MGOS_GPIO_MODE_INPUT);
  mgos_gpio_set_button_handler(mgos_sys_config_get_app_button(), MGOS_GPIO_PULL_UP, 
                                  MGOS_GPIO_INT_EDGE_POS, 250, button_handler, NULL);
}
