#include <stdatomic.h>
#include <stdio.h>
#include <tchar.h>
// #ifndef Windows11
#include "error/state.h"
#include "io/dir.h"
#include <direct.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define MAX_CHARS 999
#define DIR_NAME_TEST "y7894tu0jwgringrbiohut34968yu04tjorig"

// me when i steal code from stackoverflow
ez_DirCount ez_dir_count_items(const char *dirName) {

  if (strcmp(dirName, DIR_NAME_TEST) == 0) {
    atomic_store(&ez_errorGlobal, EZ_ERROR_FIND_FIRST_FILE_FAILED);
  }

  ez_DirCount res = {};

  TCHAR pathBuffer[MAX_CHARS] = {};
  WIN32_FIND_DATA findData;
  HANDLE hFind = INVALID_HANDLE_VALUE;

  snprintf(pathBuffer, MAX_CHARS, ("%s\\*"), dirName);
  // printf("pathBuffer: %s", pathBuffer);

  // Initialize the directory search
  hFind = FindFirstFile(pathBuffer, &findData);

  if (hFind == INVALID_HANDLE_VALUE) {
    // Paths might be restricted by permissions
    // printf("Access denied or path not found: %s\n", dirName);
    res.items = -1;
    res.files = -1;
    res.subdirectories = -1;
    atomic_store(&ez_errorGlobal, EZ_ERROR_FILE_PERSMISSION_DENIED);

    return res;
  }
  do {
    // Skip the current (.) and parent (..) directory anchors to avoid infinite
    // loops
    if (strcmp(findData.cFileName, ".") == 0 ||
        strcmp(findData.cFileName, "..") == 0) {
      continue;
    }

    // Construct the full path of the current item
    snprintf(pathBuffer, MAX_PATH, "%s\\%s", dirName, findData.cFileName);

    // Check if the item is a subdirectory
    if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
      // printf("[DIR]  %s\n", pathBuffer);
      res.items++;
      res.subdirectories++;
      // Recurse down into the subdirectory
      // WalkDirectory(pathBuffer);
    } else {
      // It's a file
      // printf("[FILE] %s\n", pathBuffer);
      res.items++;
      res.files++;
    }

  } while (FindNextFile(hFind, &findData)); // Move to the next item

  FindClose(hFind); // Always, Always, clean things up!
  return res;
};

void ez_dir_count_items_recursive(const char *dirName,
                                  ez_DirCount *pTotalDirCount) {

  // printf("iteration has %d total subdirectories found\n",
  //        pTotalDirCount->subdirectories);

  if (strcmp(dirName, DIR_NAME_TEST) == 0) {
    atomic_store(&ez_errorGlobal, EZ_ERROR_FIND_FIRST_FILE_FAILED);
  }

  ez_DirCount *res = pTotalDirCount;

  TCHAR pathBuffer[MAX_CHARS] = {};
  WIN32_FIND_DATA findData;
  HANDLE hFind = INVALID_HANDLE_VALUE;

  snprintf(pathBuffer, MAX_CHARS, ("%s\\*"), dirName);
  // printf("pathBuffer: %s", pathBuffer);

  // Initialize the directory search
  hFind = FindFirstFile(pathBuffer, &findData);

  if (hFind == INVALID_HANDLE_VALUE) {
    // Paths might be restricted by permissions
    // printf("Access denied or path not found: %s\n", dirName);

    atomic_store(&ez_errorGlobal, EZ_ERROR_FILE_PERSMISSION_DENIED);

    return;
  }
  do {
    // Skip the current (.) and parent (..) directory anchors to avoid infinite
    // loops
    if (strcmp(findData.cFileName, ".") == 0 ||
        strcmp(findData.cFileName, "..") == 0) {
      continue;
    }

    // Construct the full path of the current item
    snprintf(pathBuffer, MAX_PATH, "%s\\%s", dirName, findData.cFileName);

    // Check if the item is a subdirectory
    if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
      // printf("[DIR]  %s\n", pathBuffer);
      res->items++;
      res->subdirectories++;
      // Recurse down into the subdirectory
      ez_dir_count_items_recursive(pathBuffer, res);
    } else {
      // It's a file
      // printf("[FILE] %s\n", pathBuffer);
      res->items++;
      res->files++;
    }

  } while (FindNextFile(hFind, &findData)); // Move to the next item

  FindClose(hFind); // Always, Always, clean things up!
};

void ez_dir_get_items(const char *dirName, ez_DirItem **ppItems) {
  ez_DirCount dirCount = ez_dir_count_items(dirName);
  *ppItems = (ez_DirItem *)malloc(sizeof(ez_DirItem) * dirCount.items);

  // help me
  ez_DirItem *res = *ppItems;

  TCHAR pathBuffer[MAX_CHARS] = {};
  WIN32_FIND_DATA findData;
  HANDLE hFind = INVALID_HANDLE_VALUE;

  snprintf(pathBuffer, MAX_CHARS, ("%s\\*"), dirName);
  // printf("pathBuffer: %s", pathBuffer);

  // Initialize the directory search
  hFind = FindFirstFile(pathBuffer, &findData);

  if (hFind == INVALID_HANDLE_VALUE) {
    // Paths might be restricted by permissions
    // printf("Access denied or path not found: %s\n", dirName);
    atomic_store(&ez_errorGlobal, EZ_ERROR_FILE_PERSMISSION_DENIED);
    return;
  }

  size_t itemCount = 0;
  do {
    // Skip the current (.) and parent (..) directory anchors to avoid infinite
    // loops
    if (strcmp(findData.cFileName, ".") == 0 ||
        strcmp(findData.cFileName, "..") == 0) {
      continue;
    }

    // Construct the full path of the current item
    snprintf(pathBuffer, MAX_PATH, "%s\\%s", dirName, findData.cFileName);

    // Check if the item is a subdirectory
    if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
      // printf("[DIR]  %s\n", pathBuffer);
      //  omg i forgot to malloc the strings
      res[itemCount].name = malloc(strlen(pathBuffer) * sizeof(char) + 1);
      res[itemCount].name[strlen(pathBuffer)] =
          '\0'; // not sure if this helps anything
      strcpy(res[itemCount].name, pathBuffer);
      // printf("res[itemCount].name: %s", res[itemCount].name);
      // printf("successfully copied dir name \n");

    } else {
      // It's a file
      // printf("[FILE] %s\n", pathBuffer);
      res[itemCount].name = malloc(strlen(pathBuffer) * sizeof(char) + 1);
      res[itemCount].name[strlen(pathBuffer)] = '\0';
      strcpy(res[itemCount].name, pathBuffer);
    }

    itemCount++;

  } while (FindNextFile(hFind, &findData)); // Move to the next item

  FindClose(hFind); // Always, Always, clean things up!
  return;
};

// internal
void ez_dir_i_get_items_recursive(const char *dirName, ez_DirItem *pItems,
                                  size_t *offset) {
  ez_DirCount dirCount = ez_dir_count_items(dirName);

  // help me
  ez_DirItem *res = pItems;

  TCHAR pathBuffer[MAX_CHARS] = {};
  WIN32_FIND_DATA findData;
  HANDLE hFind = INVALID_HANDLE_VALUE;

  snprintf(pathBuffer, MAX_CHARS, ("%s\\*"), dirName);
  // printf("pathBuffer: %s", pathBuffer);

  // Initialize the directory search
  hFind = FindFirstFile(pathBuffer, &findData);

  if (hFind == INVALID_HANDLE_VALUE) {
    // Paths might be restricted by permissions
    // printf("Access denied or path not found: %s\n", dirName);
    atomic_store(&ez_errorGlobal, EZ_ERROR_FILE_PERSMISSION_DENIED);
    return;
  }

  size_t itemCount = *offset;
  do {
    // Skip the current (.) and parent (..) directory anchors to avoid infinite
    // loops
    if (strcmp(findData.cFileName, ".") == 0 ||
        strcmp(findData.cFileName, "..") == 0) {
      continue;
    }

    // Construct the full path of the current item
    snprintf(pathBuffer, MAX_PATH, "%s\\%s", dirName, findData.cFileName);

    // Check if the item is a subdirectory
    if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
      // printf("[DIR]  %s\n", pathBuffer);
      //  omg i forgot to malloc the strings
      res[itemCount].name = malloc(strlen(pathBuffer) * sizeof(char) + 1);
      res[itemCount].name[strlen(pathBuffer)] =
          '\0'; // not sure if this helps anything
      strcpy(res[itemCount].name, pathBuffer);
      res[itemCount].type = EZ_DIR_ITEM_TYPE_SUBDIRECTORY;

      itemCount++;

      // printf("res[itemCount].name: %s", res[itemCount].name);
      // printf("successfully copied dir name \n");
      ez_dir_i_get_items_recursive(pathBuffer, pItems, &itemCount);

    } else {
      // It's a file
      // printf("[FILE] %s\n", pathBuffer);
      res[itemCount].name = malloc(strlen(pathBuffer) * sizeof(char) + 1);
      res[itemCount].name[strlen(pathBuffer)] = '\0';
      res[itemCount].type = EZ_DIR_ITEM_TYPE_FILE;
      strcpy(res[itemCount].name, pathBuffer);
      itemCount++;
    }

  } while (FindNextFile(hFind, &findData)); // Move to the next item
  *offset = itemCount;

  FindClose(hFind); // Always, Always, clean things up!
  return;
};

// returns the item count
ez_DirCount ez_dir_get_items_recursive(const char *dirName,
                                       ez_DirItem **pItems) {
  ez_DirCount dirCountRecursive = {};
  ez_dir_count_items_recursive(dirName, &dirCountRecursive);

  // allocate enough space for all the items
  *pItems = (ez_DirItem *)malloc(sizeof(ez_DirItem) * dirCountRecursive.items);
  size_t offset = 0;

  ez_dir_i_get_items_recursive(dirName, *pItems, &offset);
  return dirCountRecursive;
};

int ez_dir_item_exists(const char *itemName) {
  // recursively look through all files in the working environment to see if the
  // file specified exists.
  char cwd[1024];

  if (getcwd(cwd, sizeof(cwd)) != NULL) {
    // printf("current working directory: %s\n", cwd);
    ez_DirItem *dirItemsRecursive = NULL;
    ez_DirCount dirCountRecursive =
        ez_dir_get_items_recursive(cwd, &dirItemsRecursive);

    for (int i = 0; i < dirCountRecursive.items; i++) {
      // printf("Recursive Search yielded: \"%s\"\n",
      // dirItemsRecursive[i].name);
      if (strcmp(dirItemsRecursive[i].name, itemName) == 0) {
        // printf("Found required item! %s\n", itemName);
        return 1;
      }
    }

  } else {
    atomic_store(&ez_errorGlobal, EZ_ERROR_WORKING_DIRECTORY_NOT_FOUND);
    return -1; // failure
  }

  // printf("Found no match for %s\n", itemName);
  return 0; // looked through the whole tree and item doesnt exist
};

int ez_dir_item_exists_strip(const char *itemName) {
  // recursively look through all files in the working environment to see if the
  // file specified exists.
  char cwd[1024];

  if (getcwd(cwd, sizeof(cwd)) != NULL) {
    // printf("current working directory: %s\n", cwd);
    ez_DirItem *dirItemsRecursive = NULL;
    ez_DirCount dirCountRecursive =
        ez_dir_get_items_recursive(cwd, &dirItemsRecursive);

    for (int i = 0; i < dirCountRecursive.items; i++) {
      size_t toStrip = strlen(cwd) + 1;
      size_t originalNameLen = strlen(dirItemsRecursive[i].name);

      char nameStripped[originalNameLen - toStrip + 1];

      strncpy(nameStripped, dirItemsRecursive[i].name + toStrip,
              originalNameLen - toStrip);
      nameStripped[originalNameLen - toStrip] =
          '\0'; // me when i forget to null-terminate

      // printf("Recursive Search yielded: \"%s\"\n", nameStripped);
      if (strcmp(nameStripped, itemName) == 0) {
        // printf("Found required item! %s\n", itemName);
        return 1;
      }
    }

  } else {
    atomic_store(&ez_errorGlobal, EZ_ERROR_WORKING_DIRECTORY_NOT_FOUND);
    return -1; // failure
  }

  // printf("Found no match for %s\n", itemName);
  return 0; // looked through the whole tree and item doesnt exist
};

int ez_spawn_child(const char *command) {
  printf("ez_spawn_child called!\n");

  STARTUPINFOA si = {0};
  PROCESS_INFORMATION pi = {0};

  si.cb = sizeof(si);

  // CreateProcess may modify this buffer,
  // so it must be writable.
  // printf("1\n");

  char *cmd = strdup(command);

  // printf("2\n");
  if (!cmd) {
    return -1;
  }

  BOOL ok = CreateProcessA(NULL,  // application name
                           cmd,   // command line
                           NULL,  // process security
                           NULL,  // thread security
                           FALSE, // inherit handles
                           0,     // creation flags
                           NULL,  // environment
                           NULL,  // working directory
                           &si, &pi);

  free(cmd);

  if (!ok) {
    fprintf(stderr, "CreateProcess failed: %lu\n", GetLastError());
    return -1;
  }

  // Wait for GCC to finish.
  WaitForSingleObject(pi.hProcess, INFINITE);

  DWORD exit_code;
  GetExitCodeProcess(pi.hProcess, &exit_code);

  CloseHandle(pi.hThread);
  CloseHandle(pi.hProcess);

  return (int)exit_code;
};

// #endif
