#include <stdlib.h>
#ifdef RUN_TESTS
#include "tests.h"
int main() {
  ez_run_tests();
  return 0;
}
#endif
#ifndef RUN_TESTS
#include "error/logs.h"
#include "error/state.h"
#include "generator.h"
#include "io/cmd.h"
#include "io/dir.h"
#include "io/file.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
  char logo[999] = {};
  ez_file_to_string("assets/logo.txt", logo);
  printf("%s", logo);

  ez_CompilationConfig cConfig = {.compiler = "gcc"};

  for (int i = 0; i < argc; i++) {
    // printf("Argument %d: %s\n", i, argv[i]);
    if (i != argc - 1) {
      SDEBUG("argv[i]: %s\n argv[i+1]: %s\n", argv[i], argv[i + 1]);

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
          SWARN("Requested include directory: \"%s\" does not exist.",
                 requestedInclude);
        };
      }
      // --------------------------------------------- Sources
      //
      else if (strcmp("--source-dir", argv[i]) == 0) {
        char requestedDir[strlen(argv[i + 1])];
        strcpy(requestedDir, argv[i + 1]);
        if (ez_dir_item_exists_strip(
                requestedDir)) { // should check whether it is a file or a dir.
          // so the source does exist. Now we recursively
          ez_DirItem *dirItemsRecursive = NULL;
          ez_DirCount dirCountRecursive =
              ez_dir_get_items_recursive(requestedDir, &dirItemsRecursive);

          // printf("Found %d total items in this source directory.\n",
          //        dirCountRecursive.items);
          for (int item = 0; item < dirCountRecursive.items; item++) {
            char itemName[strlen(dirItemsRecursive[item].name) +
                          1]; // i mustnt forget the null terminator!
            strcpy(itemName, dirItemsRecursive[item].name);
            itemName[strlen(dirItemsRecursive[item].name)] = '\0';
            if (dirItemsRecursive[item].type == EZ_DIR_ITEM_TYPE_FILE) {
              // printf("Found source file: %s\n", itemName);
              strcpy(cConfig.sourceFiles[cConfig.sourceFileCount], itemName);
              cConfig.sourceFiles[cConfig.sourceFileCount][strlen(itemName)] =
                  '\0';
              cConfig.sourceFileCount++;

            } else if (dirItemsRecursive[item].type ==
                       EZ_DIR_ITEM_TYPE_SUBDIRECTORY) {
              // printf("Found subdirectory: %s\n", itemName);
              strcpy(cConfig.sourceFolders[cConfig.sourceFoldersCount],
                     itemName);
              cConfig.sourceFolders[cConfig.sourceFileCount][strlen(itemName)] =
                  '\0';
              cConfig.sourceFoldersCount++;
            }
          }
        } else {
          SWARN("Requested source directory: \"%s\" does not exist.",
                 requestedDir);
        };
      } else if (strcmp("--source-file", argv[i]) == 0) {
        char requestedSource[strlen(argv[i + 1])];
        strcpy(requestedSource, argv[i + 1]);
        if (ez_dir_item_exists_strip(
                requestedSource)) { // should check whether it is a file or a
                                    // dir.
          strcpy(cConfig.sourceFiles[cConfig.sourceFileCount], requestedSource);
          cConfig
              .sourceFiles[cConfig.sourceFileCount][strlen(requestedSource)] =
              '\0';
          cConfig.sourceFileCount++;
        } else {
          SWARN("Requested source file: \"%s\" does not exist.",
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
          strcpy(cConfig.excludedSourceFiles[cConfig.excludedSourceFileCount],
                 excludedFile);
          cConfig.excludedSourceFiles[cConfig.excludedSourceFileCount]
                                     [strlen(excludedFile)] = '\0';
          cConfig.excludedSourceFileCount++;

          SINFO("Excluding file: \"%s\"\n", excludedFile);
        } else {
          SWARN("Excluded file \"%s\" may not exist.\n", excludedFile);
        };
      }

      // -------------------------------------------- Defines
      //
      //
      else if (strcmp("--define", argv[i]) == 0 ||
               strcmp("--d", argv[i]) == 0) {
        char toDefine[strlen(argv[i + 1]) + 1];
        strcpy(toDefine, argv[i + 1]);
        toDefine[strlen(argv[i + 1])] = '\0';
        SINFO("Defining: \"%s\"", toDefine);
        strcpy(cConfig.defines[cConfig.definesCount], toDefine);
        cConfig.definesCount++;
      } else if (strcmp("--out", argv[i]) == 0 || strcmp("--o", argv[i]) == 0) {
        // printf("OUTPUT\n");

        char outputFileName[strlen(argv[i + 1])];
        strcpy(outputFileName, argv[i + 1]);
        // printf("outputFileName: %s\n", outputFileName);
        // printf("Before copying: \"%s\"\n", cConfig.output);

        strcpy(cConfig.output, outputFileName);
        cConfig.output[strlen(argv[i + 1])] = '\0';
        SINFO("Writing to: \"%s\"\n", cConfig.output);
      } else {
        // bruh
      }
    }
  }

  if (cConfig.includeCount == 1) {
    SINFO("Include directory: ");
  } else {
    SINFO("Include directories: ");
  }
  for (int i = 0; i < cConfig.includeCount; i++) {

    SINFO("\"%s\"", cConfig.includeDirectories[i]);
    if (i != cConfig.includeCount - 1) {
      printf(", ");
    }
  }
  printf("\n");

  if (cConfig.sourceFileCount == 1) {
    SINFO("Source file: ");
  } else {
    SINFO("Source files: ");
  }
  for (int i = 0; i < cConfig.sourceFileCount; i++) {

    if (i != cConfig.sourceFileCount - 1) {
        SINFO("\"%s\", ", cConfig.sourceFiles[i]);
    }else{
        SINFO("\"%s\"", cConfig.sourceFiles[i]);

    }
  }
  printf("\n");

  if (cConfig.sourceFoldersCount == 1) {
    SINFO("Source folder: ");
  } else {
    SINFO("Source folders: ");
  }
  for (int i = 0; i < cConfig.sourceFoldersCount; i++) {

    if (i != cConfig.sourceFoldersCount - 1) {
         SINFO("\"%s\", ", cConfig.sourceFolders[i]);
    }else{
        SINFO("\"%s\"", cConfig.sourceFolders[i]);

    }
  }
  printf("\n");

  char *cCommand = ez_generator_from_compile_config(&cConfig);
  SINFO("Running: \"%s\"\n", cCommand);

  int res = ez_spawn_child(cCommand);

  if (!res) {
    SSUCCESS("Compilation was successful! All done now :3\n");


    return res;

  } else {
    SERROR("Unexpected exit code: %d\n", res);
    return res;
  }
  return 0;
}
#endif
