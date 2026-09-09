#ifdef RUN_TESTS
#include "tests.h"
int main() {
  ez_run_tests();
  return 0;
}
#endif
#ifndef RUN_TESTS

#include "error/state.h"
#include "io/dir.h"
#include "io/file.h"
#include <stdio.h>
#include <string.h>

typedef struct ez_CompilationConfig {
  char *include; // single include for now
  ez_DirItem *sourceFiles;
  size_t sourceFileCount;
} ez_CompilationConfig;

int main(int argc, char *argv[]) {
  char logo[999] = {};
  ez_file_to_string("assets/logo.txt", logo);
  printf("%s", logo);
  ez_dir_item_exists("C:\\dev\\ez\\src\\tests.c");


  ez_CompilationConfig cConfig = {};
  for (int i = 0; i < argc; i++) {
    //printf("Argument %d: %s\n", i, argv[i]);
    if (i != argc - 1) {
      if (strcmp("--include", argv[i]) == 0) {
        printf("Include directory: %s\n", argv[i + 1]);
      }
    }
  }

  return 0;
}
#endif
