#include "log.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void t_log(LOG_LEVEL level, const char *func, ...) {
  char *system_log_level = getenv("TRACKME_LOG_LEVEL");
  if ((!system_log_level) || atoi(system_log_level) < (int)level) {
    return;
  }
  va_list args;
  va_start(args, func);
  char *message = va_arg(args, char *);

  char *format;
  switch (level) {
  case INFO:
    format = ANSI_COLOR_YELLOW "[%s] Info: " ANSI_COLOR_RESET "%s\n";
    break;
  case ERROR:
    format = ANSI_COLOR_RED "[%s] Error: " ANSI_COLOR_RESET "%s\n";
    break;
  case SUCCESS:
    format = ANSI_COLOR_GREEN "[%s] Success: " ANSI_COLOR_RESET "%s\n";
    break;
  }

  int length = strlen(format) + strlen(func) + strlen(message);
  char *log_format = malloc(length);
  snprintf(log_format, length, format, func, message);
  vprintf(log_format, args);
  va_end(args);
  free(log_format);
}
