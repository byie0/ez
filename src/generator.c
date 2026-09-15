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

  printf("Using compiler: %s\n", cConfig->compiler);
  size_t includeCharacterCount = 0;
  for (size_t i = 0; i < cConfig->includeCount; i++) {
    includeCharacterCount += strlen(cConfig->includeDirectories[i]);
  }

  printf("Counted: %llu characters in the include directory names\n",
         includeCharacterCount);

  size_t sourceCharacterCount = 0;
  for (size_t i = 0; i < cConfig->sourceFileCount; i++) {
    sourceCharacterCount += strlen(cConfig->sourceFiles[i]);
  }
  printf("Counted: %llu characters in the source file names\n",
         sourceCharacterCount);

  size_t defineCharacterCount = 0;
  for (size_t i = 0; i < cConfig->definesCount; i++) {
    defineCharacterCount += strlen(cConfig->defines[i]);
  }
  printf("Counted: %llu characters in the define names\n",
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
      + cConfig->includeCount + cConfig->sourceFileCount + cConfig->definesCount;

  printf("totalCharCount: %llu\n", totalCharCount);
  char *compileCommandBuffer = malloc(totalCharCount);
  size_t bufferPos = 0;

  strcpy(compileCommandBuffer, gccCommand);
  bufferPos += strlen(gccCommand);
  compileCommandBuffer[bufferPos] = ' ';
  bufferPos++;

  for (size_t j = 0; j < cConfig->sourceFileCount; j++) {
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

  printf("Generated compile command (%llu chars long): %s\n",
         strlen(compileCommandBuffer), compileCommandBuffer);

  return compileCommandBuffer;
};
