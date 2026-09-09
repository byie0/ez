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

// includes
#define EZ_COMPILATION_CONFIG_INCLUDE_DIRECTORIES_MAX 10
#define EZ_COMPILATION_CONFIG_INCLUDE_DIRECTORIES_MAX_NAME_LEN 40
// source files
#define EZ_COMPILATION_CONFIG_SOURCE_FILE_MAX 10
#define EZ_COMPILATION_CONFIG_SOURCE_FILE_MAX_NAME_LEN 40

typedef struct ez_CompilationConfig {
  char includeDirectories
      [EZ_COMPILATION_CONFIG_INCLUDE_DIRECTORIES_MAX]
      [EZ_COMPILATION_CONFIG_INCLUDE_DIRECTORIES_MAX_NAME_LEN];
  size_t includeCount;

  // unique, specified by the user ()
  char sourceFiles[EZ_COMPILATION_CONFIG_SOURCE_FILE_MAX]
                  [EZ_COMPILATION_CONFIG_SOURCE_FILE_MAX_NAME_LEN];
  size_t sourceFilesCount;

  

} ez_CompilationConfig;

int main(int argc, char *argv[]) {
  char logo[999] = {};
  ez_file_to_string("assets/logo.txt", logo);
  printf("%s", logo);

  ez_CompilationConfig cConfig = {};

  for (int i = 0; i < argc; i++) {
    // printf("Argument %d: %s\n", i, argv[i]);
    if (i != argc - 1) {

      // --------------------------------------------- Include directory
      if (strcmp("--include", argv[i]) == 0) {
        char requestedInclude[strlen(argv[i + 1])];
        strcpy(requestedInclude, argv[i + 1]);
        if (ez_dir_item_exists_strip(requestedInclude)) {
          strcpy(cConfig.includeDirectories[cConfig.includeCount],
                 requestedInclude);

          cConfig.includeDirectories[cConfig.includeCount]
                                    [strlen(requestedInclude)] = '\0';

          cConfig.includeCount++;
        } else {
          printf("Requested include directory: \"%s\" does not exist.",
                 requestedInclude);
        };
      }
      // --------------------------------------------- Include directory
      //
      // recursive (everything in the folder)
      else if (strcmp("--source-file", argv[i]) == 0) {
        char requestedSource[strlen(argv[i + 1])];
        strcpy(requestedSource, argv[i + 1]);
        if (ez_dir_item_exists_strip(requestedSource)) { // should check whether it is a file or a dir.
          strcpy(cConfig.sourceFiles[cConfig.sourceFilesCount],
                 requestedSource);
          cConfig
              .sourceFiles[cConfig.sourceFilesCount][strlen(requestedSource)] =
              '\0';
          cConfig.sourceFilesCount++;
        } else {
          printf("Requested source directory: \"%s\" does not exist.",
                 requestedSource);
        };
      }
      // -------------------------------------------- Exclude
      // maybe add --exclude-files [file1.c, file2.c, etc.]
      //
      // exclude file
      else if (strcmp("--exclude-file", argv[i]) == 0) {
        char excludedFile[strlen(argv[i + 1])];
        strcpy(excludedFile, argv[i + 1]);
        if (ez_dir_item_exists_strip(excludedFile)) {
          printf("Excluding file: \"%s\"\n", excludedFile);
        } else {
          printf("Excluded file \"%s\" may not exist.", excludedFile);
        };
      }

      // -------------------------------------------- Defines
      //
      //
      else if (strcmp("--define", argv[i]) == 0 ||
               strcmp("--d", argv[i]) == 0) {
        char toDefine[strlen(argv[i + 1])];
        strcpy(toDefine, argv[i + 1]);
        printf("Defining: \"%s\"\n", toDefine);
      }
    }
  }

  if (cConfig.includeCount == 1) {
    printf("Include directory: ");
  } else {
    printf("Include directories: ");
  }
  for (int i = 0; i < cConfig.includeCount; i++) {

    printf("\"%s\"", cConfig.includeDirectories[i]);
    if (i != cConfig.includeCount - 1) {
      printf(", ");
    }
  }
  printf("\n");

  if (cConfig.sourceFilesCount == 1) {
    printf("Source file: ");
  } else {
    printf("Source files: ");
  }
  for (int i = 0; i < cConfig.sourceFilesCount; i++) {

    printf("\"%s\"", cConfig.sourceFiles[i]);
    if (i != cConfig.sourceFilesCount - 1) {
      printf(", ");
    }
  }
  printf("\n");

  return 0;
}
#endif
