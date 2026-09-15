#include "generator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
// Count how many characters are in the include folders
// + spaces
// + flag prefixes
//
char *ez_generator_from_compile_config(ez_CompilationConfig *cConfig) {

  // printf("Using compiler: %s\n", cConfig->compiler);
  size_t includeCharacterCount = 0;
  for (size_t i = 0; i < cConfig->includeCount; i++) {
    includeCharacterCount += strlen(cConfig->includeDirectories[i]);
  }

  // printf("Counted: %llu characters in the include directory names\n",
  //        includeCharacterCount);

  size_t sourceCharacterCount = 0;
  for (size_t i = 0; i < cConfig->sourceFilesCount; i++) {
    sourceCharacterCount += strlen(cConfig->sourceFiles[i]);
  }

  // printf("Counted: %llu characters in the source file names\n",
  //        sourceCharacterCount);

  // assuming gcc for now
  char gccCommand[] = "gcc";
  char gccIncludeFlag[] = "-I";
  char gccOutputFlag[] = "-o";
  size_t totalCharCount = strlen(gccCommand) +
                          (sourceCharacterCount +       // Source file names
                           cConfig->sourceFilesCount) + // spaces between them
                          +(strlen(gccIncludeFlag) *    // include prefix
                            cConfig->includeCount)      // * include directories
                          + cConfig->includeCount       // spaces between them
                          + strlen(gccOutputFlag)       // self-explanatory
                          + strlen(cConfig->output);

  // printf("totalCharCount: %llu\n", totalCharCount);
  char *compileCommandBuffer = malloc(totalCharCount);
  size_t bufferPos = 0;

  strcpy(compileCommandBuffer, gccCommand);
  bufferPos += strlen(gccCommand);
  compileCommandBuffer[bufferPos] = ' ';
  bufferPos++;

  for (size_t j = 0; j < cConfig->sourceFilesCount; j++) {
    strcpy(compileCommandBuffer + bufferPos, cConfig->sourceFiles[j]);
    bufferPos += strlen(cConfig->sourceFiles[j]);
    compileCommandBuffer[bufferPos] = ' ';
    bufferPos++;
  }

  for (size_t j = 0; j < cConfig->includeCount; j++) {
    strcpy(compileCommandBuffer + bufferPos, gccIncludeFlag);
    bufferPos += strlen(gccIncludeFlag);
    strcpy(compileCommandBuffer + bufferPos, cConfig->includeDirectories[j]);
    bufferPos += strlen(cConfig->includeDirectories[j]);
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

  // printf("Generated compile command (%llu chars long): %s\n",
  //        strlen(compileCommandBuffer), compileCommandBuffer);

  return compileCommandBuffer;

};
