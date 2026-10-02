#include "io/file.h"
#include "error/logs.h"
#include "error/state.h"
#include <ctype.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
long ez_file_to_string(const char *path, char *pDestination) {
  FILE *fptr = fopen(path, "r");
  if (!fptr) {
    atomic_store(&ez_errorGlobal, EZ_ERROR_FILE_PERSMISSION_DENIED);
    SERROR("Failed to open file: %s", path);
  }
  fseek(fptr, 0, SEEK_END);

  long byteCount = ftell(fptr);
  fseek(fptr, 0, SEEK_SET);

  STRACE("File byte count: %lu\n", byteCount);

  for (long i = 0; i < byteCount; i++) {
    // printf("%02X ", (unsigned char)pDestination[i]);
  }
  fread(pDestination, sizeof(char), byteCount, fptr);
  pDestination[byteCount] = '\0';

  fclose(fptr);
  return byteCount;
};

int ez_file_config_to_argv(char *configString, char ***pDestination) {

  int argc = 1; // argv[0] generall is "ez"

  int arg_in = 0; // is there arguments?

  // loop while the current position is still valid
  for (char *p = configString; *p; p++) {
    if (isspace((unsigned char)*p)) {
      arg_in = 0;
    } else if (!arg_in) {
      argc++;
      arg_in = 1;
    }
  }

  char **argv = malloc((argc + 1) * sizeof(*argv));
  if (!argv) {
    return -1;
  }

  argv[0] = "ez";

  int i = 1;
  char *p = configString;

  while (*p) {
    while (*p && isspace((unsigned char)*p)) {
      p++;
    }

    if (!*p) {
      break;
    }

    // the start of the argument
    argv[i++] = p;

    while (*p && !isspace((unsigned char)*p)) {
      p++;
    }

    if (*p) {
      *p++ = '\0';
    }
  }

  argv[i] = NULL;
  *pDestination = argv;
  return i;
};
