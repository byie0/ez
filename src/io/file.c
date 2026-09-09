#include "io/file.h"
#include <stdatomic.h>
#include "error/state.h"
#include <stdio.h>

void ez_file_to_string(const char *path, char *pDestination) {
  FILE *fptr = fopen(path, "r");
  if (!fptr) {
    atomic_store(&ez_errorGlobal, EZ_ERROR_FILE_PERSMISSION_DENIED);
  }
  fseek(fptr, 0, SEEK_END);

  long byteCount = ftell(fptr);
  fseek(fptr, 0, SEEK_SET);

  //printf("File byte count: %lu\n", byteCount);

  for (long i = 0; i < byteCount; i++) {
      //printf("%02X ", (unsigned char)pDestination[i]);
  }
  fread(pDestination, sizeof(char), byteCount, fptr);
  pDestination[byteCount] = '\0';

  fclose(fptr);
};
