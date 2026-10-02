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
  char **argsFiltered = argv;
  int argcFiltered = argc;

  if (argc == 1) {
    SINFO("No arguments provided, looking for \"ezConfig.txt\"");
    char ezConfigString[999];
    long bytes = ez_file_to_string("ezConfig.txt", ezConfigString);

    

    argcFiltered = ez_file_config_to_argv(ezConfigString, &argsFiltered);
    SINFO("Config argc: %d", argcFiltered);
    for (int i = 0; i < argcFiltered; i++) {
      STRACE("config_argv[%d] = %s", i, argsFiltered[i]);
    }
  }

  for (int i = 0; i < argcFiltered; i++) {
    // printf("Argument %d: %s\n", i, argsFiltered[i])kj;
    if (i != argcFiltered - 1) {
      // SDEBUG("argsFiltered[i]: %s\n argsFiltered[i+1]: %s\n",
      // argsFiltered[i],
      //        argsFiltered[i + 1]);

      // --------------------------------------------- Include directory
      if (strcmp("--include", argsFiltered[i]) == 0) {
        char requestedInclude[strlen(argsFiltered[i + 1])];
        strcpy(requestedInclude, argsFiltered[i + 1]);
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
      else if (strcmp("--source-dir", argsFiltered[i]) == 0) {
        char requestedDir[strlen(argsFiltered[i + 1])];
        strcpy(requestedDir, argsFiltered[i + 1]);
        if (ez_dir_item_exists_strip(requestedDir)) { // should check whether it
                                                      // is a file or a dir.
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
      } else if (strcmp("--source-file", argsFiltered[i]) == 0) {
        char requestedSource[strlen(argsFiltered[i + 1])];
        strcpy(requestedSource, argsFiltered[i + 1]);
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
      else if (strcmp("--exclude-file", argsFiltered[i]) == 0) {
        char excludedFile[strlen(argsFiltered[i + 1])];
        strcpy(excludedFile, argsFiltered[i + 1]);
        if (ez_dir_item_exists_strip(excludedFile)) {
          strcpy(cConfig.excludedSourceFiles[cConfig.excludedSourceFileCount],
                 excludedFile);
          cConfig.excludedSourceFiles[cConfig.excludedSourceFileCount]
                                     [strlen(excludedFile)] = '\0';
          cConfig.excludedSourceFileCount++;

          SINFO("Excluding file: \"%s\"", excludedFile);
        } else {
          SWARN("Excluded file \"%s\" may not exist.", excludedFile);
        };
      }

      // -------------------------------------------- Defines
      //
      //
      else if (strcmp("--define", argsFiltered[i]) == 0 ||
               strcmp("--d", argsFiltered[i]) == 0) {
        char toDefine[strlen(argsFiltered[i + 1]) + 1];
        strcpy(toDefine, argsFiltered[i + 1]);
        toDefine[strlen(argsFiltered[i + 1])] = '\0';
        SINFO("Defining: \"%s\"", toDefine);
        strcpy(cConfig.defines[cConfig.definesCount], toDefine);
        cConfig.definesCount++;
      } else if (strcmp("--out", argsFiltered[i]) == 0 ||
                 strcmp("--o", argsFiltered[i]) == 0) {
        // printf("OUTPUT\n");

        char outputFileName[strlen(argsFiltered[i + 1])];
        strcpy(outputFileName, argsFiltered[i + 1]);
        // printf("outputFileName: %s\n", outputFileName);
        // printf("Before copying: \"%s\"\n", cConfig.output);

        strcpy(cConfig.output, outputFileName);
        cConfig.output[strlen(argsFiltered[i + 1])] = '\0';
        SINFO("Writing to: \"%s\"", cConfig.output);
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
    } else {
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
    } else {
      SINFO("\"%s\"", cConfig.sourceFolders[i]);
    }
  }
  printf("\n");

  char *cCommand = ez_generator_from_compile_config(&cConfig);
  SINFO("Running: \"%s\"", cCommand);

  SINFO("Command length: %zu", strlen(cCommand));

 

  int res = ez_spawn_child(cCommand);

  if (!res) {
    SSUCCESS("Compilation was successful! All done now :3\n");
    SINFO("Generating compile commands...");

    char *ccJson = ez_generator_compile_commands(&cConfig);
    FILE *jsonPtr = fopen("compile_commands.json", "w");
    if (!jsonPtr) {
      SERROR("Couldn't open compile_commands.json");
      return 0;
    }

    size_t jsonSize = strlen(ccJson);

    fputs(ccJson, jsonPtr);

    fclose(jsonPtr);

    SSUCCESS("Generated compile_commands.json!");

    return res;

  } else {
    SERROR("Unexpected exit code: %d", res);
    return res;
  }
  return 0;
}
#endif
