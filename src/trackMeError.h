#ifndef VALUE_OR_ERROR
#define VALUE_OR_ERROR

#include <stdlib.h>

typedef enum ErrorCode {
  E_OK = 0,
  E_COULD_NOT_GET_SYSTEM_TIME, // 1
  E_START_TIME_IN_FUTURE, // 2
  E_TIMER_NOT_STARTED, // 3
  E_COULD_NOT_ALLOCATE_ENOUGH_MEMORY, // 4
} ErrorCode;

void error(ErrorCode ec);

#endif // !VALUE_OR_ERROR
