#ifndef APP_H
#define APP_H

#include "arg_parser.h"

typedef enum {
    APP_STATUS_SUCCESS=0,
    APP_STATUS_ERROR,
    APP_STATUS_UNDEFINED,
  } app_status_t;

app_status_t app(arg_t args);

#endif //APP_H
