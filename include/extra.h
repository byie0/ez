#pragma once
#include "error/logs.h"
#include "error/state.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#define FLOPPY_SIZE 1474560
void extra_floppy(const char in[], const char out[]) {
  FILE *fptr = fopen(in, "rb");
  SINFO("Flopping %s", in);
  if (!fptr) {
    atomic_store(&ez_errorGlobal, EZ_ERROR_FILE_PERSMISSION_DENIED);
    SERROR("Failed to open file: %s", in);
  }
  fseek(fptr, 0, SEEK_END);

  long byteCount = ftell(fptr);
  char *buf = malloc(byteCount);
  fseek(fptr, 0, SEEK_SET);

  size_t bytesRead = fread(buf, 1, (size_t)byteCount, fptr);
  fclose(fptr);

  int ch;
  uint32_t p = 0;
  while ((ch = fgetc(fptr)) != EOF) {
    buf[p++] = ch;
    // for (int i = 7; i >= 0; i--) {
    //   int bit = ((unsigned char)ch >> i) & 1;
    //   printf("%d", bit);
    // }
  }
  buf[p] = '\0';
  fclose(fptr);

  FILE *fout = fopen(out, "wb");
  if (!fout) {
    SERROR("Failed to open output floppy!");
  }
  size_t bytesWritten = fwrite(buf, 1, bytesRead, fout);

  for (long i = byteCount; i < FLOPPY_SIZE; i++) {
    if (fputc(0, fout) == EOF) {
      SERROR("Failed to pad floppy image");
      fclose(fout);
      free(buf);
      return;
    }
  }

  if (fclose(fout) != 0) {
    SERROR("Failed to finalize output file");
  }

  SINFO("Finished padding floppy");
  free(buf);

  fclose(fout);
}
