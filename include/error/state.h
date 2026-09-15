#pragma once

#include <stdatomic.h>

typedef enum ez_ErrorCodes {
  EZ_ERROR_FIND_FIRST_FILE_FAILED,
  EZ_ERROR_FILE_PERSMISSION_DENIED,
  EZ_ERROR_WORKING_DIRECTORY_NOT_FOUND,
} ez_ErrorCodes;

#define ez_error_check()                                                       \
  switch (ez_errorGlobal) {                                                    \
  case EZ_ERROR_FIND_FIRST_FILE_FAILED: {                                      \
    SERROR("Error finding the first file of the requested subdirectory");      \
  }                                                                            \
  }

extern _Atomic ez_ErrorCodes ez_errorGlobal;
