#ifndef LOG_H
#define LOG_H

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_RESET "\x1b[0m"

typedef enum { SUCCESS = 1, ERROR = 2, INFO = 3 } LOG_LEVEL;

void t_log(LOG_LEVEL, const char *func, ...);

#endif // !LOG_H
