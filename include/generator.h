#pragma once
#include <stdint.h>


// compiler
#define EZ_COMPILATION_CONFIG_COMPILER_MAX_NAME_LEN 20

// includes
#define EZ_COMPILATION_CONFIG_INCLUDE_DIRECTORIES_MAX 10
#define EZ_COMPILATION_CONFIG_INCLUDE_DIRECTORIES_MAX_NAME_LEN 40
// source files
#define EZ_COMPILATION_CONFIG_SOURCE_FILE_MAX 20
#define EZ_COMPILATION_CONFIG_SOURCE_FILE_MAX_NAME_LEN 40

// source folder
#define EZ_COMPILATION_CONFIG_SOURCE_FOLDERS_MAX 20
#define EZ_COMPILATION_CONFIG_SOURCE_FOLDERS_MAX_NAME_LEN 40

// output
#define EZ_COMPILATION_CONFIG_OUTPUT_FILE_MAX_NAME_LEN 40

typedef struct ez_CompilationConfig {
  char compiler[EZ_COMPILATION_CONFIG_COMPILER_MAX_NAME_LEN];

  char includeDirectories
      [EZ_COMPILATION_CONFIG_INCLUDE_DIRECTORIES_MAX]
      [EZ_COMPILATION_CONFIG_INCLUDE_DIRECTORIES_MAX_NAME_LEN];
  size_t includeCount;

  // unique, specified by the user ()
  char sourceFiles[EZ_COMPILATION_CONFIG_SOURCE_FILE_MAX]
                  [EZ_COMPILATION_CONFIG_SOURCE_FILE_MAX_NAME_LEN];
  size_t sourceFilesCount;

  char outputFile[EZ_COMPILATION_CONFIG_OUTPUT_FILE_MAX_NAME_LEN];

  char sourceFolders[EZ_COMPILATION_CONFIG_SOURCE_FOLDERS_MAX]
                    [EZ_COMPILATION_CONFIG_SOURCE_FOLDERS_MAX_NAME_LEN];
  size_t sourceFoldersCount;

  char output[EZ_COMPILATION_CONFIG_OUTPUT_FILE_MAX_NAME_LEN];

} ez_CompilationConfig;

char *ez_generator_from_compile_config(ez_CompilationConfig *cConfig);
