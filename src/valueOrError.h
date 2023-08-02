#ifndef VALUE_OR_ERROR
#define VALUE_OR_ERROR

#include <stdlib.h>

typedef enum ErrorCode {
  E_OK = 0,
  E_COULD_NOT_GET_SYSTEM_TIME, // 1
} ErrorCode;

typedef struct ValueOrError {
  ErrorCode err;
  void *value;
} ValueOrError;

#endif // !VALUE_OR_ERROR
