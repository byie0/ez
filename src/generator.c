#include "generator.h"
#include "io/dir.h"
#include "error/logs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
// Count how many characters are in the include folders
// + spaces
// + flag prefixes
//
char *ez_generator_from_compile_config(ez_CompilationConfig *cConfig) {

  size_t excludedSourceFilesVerified = 0;
  for (int i = 0; i < cConfig->excludedSourceFileCount; i++) {
    for (int j = 0; j < cConfig->sourceFileCount; j++) {
      if (strcmp(cConfig->excludedSourceFiles[i], cConfig->sourceFiles[j]) ==
          0) {
        SINFO("Detected file: \"%s\" to be excluded. Removing from "
              "compilation...\n",
              cConfig->excludedSourceFiles[i]);
        cConfig->sourceFiles[j][0] = '\0';
        excludedSourceFilesVerified++;
      }
    }
  }
  SINFO("%llu source file(s) have been excluded.\n",
        excludedSourceFilesVerified);

  size_t excludedIndicesPos = 0;
  size_t excludedIndices[excludedSourceFilesVerified];
  for (int i = 0; i < cConfig->excludedSourceFileCount; i++) {
    for (int j = 0; j < cConfig->sourceFileCount; j++) {
      if (strcmp(cConfig->excludedSourceFiles[i], cConfig->sourceFiles[j]) ==
          0) {

        excludedIndices[excludedIndicesPos] = j;
        excludedIndicesPos++;
      }
    }
  }

  SINFO("Using compiler: %s\n", cConfig->compiler);

  size_t includeCharacterCount = 0;
  for (size_t i = 0; i < cConfig->includeCount; i++) {
    includeCharacterCount += strlen(cConfig->includeDirectories[i]);
  }

  SDEBUG("Counted: %llu characters in the include directory names\n",
         includeCharacterCount);

  size_t sourceCharacterCount = 0;
  for (size_t i = 0; i < cConfig->sourceFileCount; i++) {
    sourceCharacterCount += strlen(cConfig->sourceFiles[i]);
  }
  SDEBUG("Counted: %llu characters in the source file names\n",
         sourceCharacterCount);

  size_t defineCharacterCount = 0;
  for (size_t i = 0; i < cConfig->definesCount; i++) {
    defineCharacterCount += strlen(cConfig->defines[i]);
  }
  SDEBUG("Counted: %llu characters in the define names\n",
         defineCharacterCount);

  // assuming gcc for now
  char gccCommand[] = "gcc";
  char gccIncludeFlag[] = "-I";
  char gccDefineFlag[] = "-D";

  char gccOutputFlag[] = "-o";
  size_t totalCharCount =
      // Variable-length
      sourceCharacterCount + defineCharacterCount + includeCharacterCount +
      strlen(cConfig->output)
      // constant
      + strlen(gccCommand) +
      strlen(gccOutputFlag)

      // prefixes (variable length)
      + (strlen(gccIncludeFlag) * cConfig->includeCount) +
      (strlen(gccDefineFlag) * cConfig->definesCount)

      // spaces
      + 1 // gcc command
      + 1 // output command
      + cConfig->includeCount +
      (cConfig->sourceFileCount - excludedSourceFilesVerified) +
      cConfig->definesCount;

  SDEBUG("totalCharCount: %llu\n", totalCharCount);
  char *compileCommandBuffer = malloc(totalCharCount);
  size_t bufferPos = 0;

  strcpy(compileCommandBuffer, gccCommand);
  bufferPos += strlen(gccCommand);
  compileCommandBuffer[bufferPos] = ' ';
  bufferPos++;

  for (size_t j = 0; j < cConfig->sourceFileCount; j++) {
    strcpy(compileCommandBuffer + bufferPos, cConfig->sourceFiles[j]);
    bufferPos += strlen(cConfig->sourceFiles[j]);
    if (cConfig->sourceFiles[j][0] == '\0') {

      // this file is excluded
    } else {

      compileCommandBuffer[bufferPos] =
          ' '; // if this file hasn't been excluded
      bufferPos++;
    }
  }

  for (size_t j = 0; j < cConfig->includeCount; j++) {
    strcpy(compileCommandBuffer + bufferPos, gccIncludeFlag);
    bufferPos += strlen(gccIncludeFlag);
    strcpy(compileCommandBuffer + bufferPos, cConfig->includeDirectories[j]);
    bufferPos += strlen(cConfig->includeDirectories[j]);
    compileCommandBuffer[bufferPos] = ' ';
    bufferPos++;
  }

  for (size_t j = 0; j < cConfig->definesCount; j++) {
    strcpy(compileCommandBuffer + bufferPos, gccDefineFlag);
    bufferPos += strlen(gccDefineFlag);
    strcpy(compileCommandBuffer + bufferPos, cConfig->defines[j]);
    bufferPos += strlen(cConfig->defines[j]);
    compileCommandBuffer[bufferPos] = ' ';
    bufferPos++;
  }

  strcpy(compileCommandBuffer + bufferPos, gccOutputFlag);
  bufferPos += strlen(gccOutputFlag);
  compileCommandBuffer[bufferPos] = ' ';
  bufferPos++;

  strcpy(compileCommandBuffer + bufferPos, cConfig->output);
  bufferPos += strlen(cConfig->output);
  compileCommandBuffer[bufferPos] = '\0'; // and we are done!
  bufferPos++;

  SDEBUG("Generated compile command (%llu chars long): %s\n",
         strlen(compileCommandBuffer), compileCommandBuffer);

  return compileCommandBuffer;
};

// internal helper just for this file
static int is_file_excluded(ez_CompilationConfig *cConfig,
                            const char *filename) {
  if (filename == NULL || filename[0] == '\0')
    return 1;

  for (int i = 0; i < cConfig->excludedSourceFileCount; i++) {
    if (strcmp(cConfig->excludedSourceFiles[i], filename) == 0) {
      return 1;
    }
  }
  return 0;
}

char *ez_generator_compile_commands(ez_CompilationConfig *cConfig) {
  const char *compiler =
      (cConfig->compiler != NULL) ? cConfig->compiler : "gcc";

  // assuming gcc for now
  char gccCommand[] = "gcc";
  char gccIncludeFlag[] = "-I";
  char gccDefineFlag[] = "-D";

  char gccOutputFlag[] = "-o";
  // i think i should move these to a sort of compiler template. TODO

  size_t commonFlagsLen = strlen(compiler) + 1; // "compiler "

  for (size_t i = 0; i < cConfig->includeCount; i++) {
    commonFlagsLen +=
        strlen(gccIncludeFlag) + strlen(cConfig->includeDirectories[i]) + 1; // "-I" + dir + " "
  }
  for (size_t i = 0; i < cConfig->definesCount; i++) {
    commonFlagsLen += strlen(gccDefineFlag) + strlen(cConfig->defines[i]) + 1; // "-D" + def + " "
  }

  char *commonFlags = malloc(commonFlagsLen + 1);
  commonFlags[0] = '\0';
  strcat(commonFlags, compiler);
  strcat(commonFlags, " ");

  for (size_t i = 0; i < cConfig->includeCount; i++) {
    strcat(commonFlags, gccIncludeFlag);
    strcat(commonFlags, cConfig->includeDirectories[i]);
    strcat(commonFlags, " ");
  }
  for (size_t i = 0; i < cConfig->definesCount; i++) {
    strcat(commonFlags, gccDefineFlag);
    strcat(commonFlags, cConfig->defines[i]);
    strcat(commonFlags, " ");
  }

  const char *working_dir = ez_dir_get_working_directory(); // fixed

  size_t total_size = 3; // "[\n" + '\0' terminator
  int valid_files = 0;
  for (size_t i = 0; i < cConfig->sourceFileCount; i++) {
    if (is_file_excluded(cConfig, cConfig->sourceFiles[i]))
      continue;
    valid_files++;

    // snprintf with NULL buffer calculates the length it WOULD write
    int entry_len =
        snprintf(NULL, 0,
                 "  {\n    \"directory\": \"%s\",\n    \"command\": \"%s-c "
                 "%s\",\n    \"file\": \"%s\"\n  }",
                 working_dir, commonFlags, cConfig->sourceFiles[i],
                 cConfig->sourceFiles[i]);

    total_size += entry_len; //very handy trick! i did learn this from gemini, so credit where its due
    // man this wouldve made the generator_from_compile_config so much simpler
  }

  if (valid_files > 0) {
    total_size += (valid_files - 1) * 2; // ",\n" between elements
  }
  total_size += 2; // "\n]"

  char *jsonBuffer = malloc(total_size);
  char *bufferPos = jsonBuffer; //

  bufferPos += sprintf(bufferPos, "[\n");

  int processed = 0;
  for (size_t i = 0; i < cConfig->sourceFileCount; i++) {
    if (is_file_excluded(cConfig, cConfig->sourceFiles[i]))
      continue;

    bufferPos +=
        sprintf(bufferPos,
                "  {\n    \"directory\": \"%s\",\n    \"command\": \"%s-c "
                "%s\",\n    \"file\": \"%s\"\n  }",
                working_dir, commonFlags, cConfig->sourceFiles[i],
                cConfig->sourceFiles[i]);

    processed++;
    if (processed < valid_files) {
      bufferPos += sprintf(bufferPos, ",\n"); // comma for all but the last item
    } else {
      bufferPos += sprintf(bufferPos, "\n"); // no comma for the last item
    }
  }
  sprintf(bufferPos, "]"); // End JSON

  free(commonFlags);

  for (size_t i = 0; i < strlen(jsonBuffer); i++) {
    if (jsonBuffer[i] == '\\') {
      jsonBuffer[i] = '/';
    }
  }

  SDEBUG("Compile commands json generated (%llu chars long)\n",
         (unsigned long long)strlen(jsonBuffer));

  return jsonBuffer;
};
