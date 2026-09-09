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


int main(int argc, char *argv[]) {
    

  for (int i = 0; i < argc; i++) {
    printf("Argument %d: %s\n", i, argv[i]);
  }

  char logo[999] = {};
  ez_file_to_string("assets/logo.txt", logo);

  printf("%s", logo);

  return 0;
}
#endif
