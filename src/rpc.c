#include "rpc.h"
#include "mgos_config.h"
#include "button-led.h"

static void rpc_log(struct mg_rpc_request_info *ri, struct mg_str args) {
  LOG(LL_INFO, ("tag=%.*s src=%.*s method=%.*s args='%.*s'",
                ri->tag.len, ri->tag.p, ri->src.len, ri->src.p,
                ri->method.len, ri->method.p, args.len, args.p));
}


static void rpc_math_add(struct mg_rpc_request_info *ri, void *cb_arg, struct mg_rpc_frame_info *fi, struct mg_str args) {
  int32_t a, b;
  int32_t sum;

  rpc_log(ri, args);

  if (json_scanf(args.p, args.len, ri->args_fmt, &a, &b) != 2) {
    mg_rpc_send_errorf(ri, 400, "argument 'a' and 'b' are required");
    ri = NULL;
    return;
  }
  sum=a+b;
  mg_rpc_send_responsef(ri, "{result: %d}", sum);
  ri = NULL;

  (void) ri;
  (void) cb_arg;
  (void) fi;
  (void) args;
}

static void rpc_led_getstate(struct mg_rpc_request_info *ri, void *cb_arg, struct mg_rpc_frame_info *fi, struct mg_str args) {
  rpc_log(ri, args);

  mg_rpc_send_responsef(ri, "{state: %d}", led_getstate());
  ri = NULL;

  (void) ri;
  (void) cb_arg;
  (void) fi;
  (void) args;
}

static void rpc_button_push(struct mg_rpc_request_info *ri, void *cb_arg, struct mg_rpc_frame_info *fi, struct mg_str args) {
  rpc_log(ri, args);

  button_handler(mgos_sys_config_get_app_button(), (void*)1);
  mg_rpc_send_responsef(ri, "{status: OK}");
  ri = NULL;

  (void) ri;
  (void) cb_arg;
  (void) fi;
  (void) args;
}

void rpc_init() {
  struct mg_rpc *c = mgos_rpc_get_global();
  mg_rpc_add_handler(c, "Math.Add", "{a: %d, b: %d}", rpc_math_add, NULL);
  mg_rpc_add_handler(c, "LED.GetState", "", rpc_led_getstate, NULL);
  mg_rpc_add_handler(c, "Button.Push", "", rpc_button_push, NULL);
}
