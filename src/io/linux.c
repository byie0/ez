#ifdef Linux
#include "error/logs.h"
#include "error/state.h"
#include "io/dir.h"
#include <dirent.h>
#include <limits.h>
#include <malloc.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "io/cmd.h"
// todo: make this constant a define somewhere else
#define MAX_PATH 50
#define MAX_CHARS 999

char *ez_dir_get_working_directory() {
  size_t bufferSize = 2048;
  char *cwd = malloc(bufferSize);

  if (getcwd(cwd, bufferSize)) {
    return cwd;
  } else {
    SERROR("Failed to get GWD!"); // todo: error state
    return NULL;
  }
};

ez_DirCount ez_dir_count_items(const char *dirName) {

  ez_DirCount res = {};

  DIR *dir;
  struct dirent *entry;
  int count = 0;

  dir = opendir(dirName);
  if (dir == NULL) {
    SERROR("Failed to open \"%s\"", dirName);
    res.items = -1;
    res.files = -1;
    res.subdirectories = -1;
    atomic_store(&ez_errorGlobal, EZ_ERROR_FILE_PERSMISSION_DENIED);

    return res;
  }

  while ((entry = readdir(dir)) != NULL) {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
      continue;
    }
    if (entry->d_type == DT_REG) {
      res.files++;
    } else if (entry->d_type == DT_DIR) {
      res.subdirectories++;
    }
    count++;
  }
  closedir(dir);
  res.items = count;
  return res;
};
void ez_dir_count_items_recursive(const char *dirName,
                                  ez_DirCount *pTotalDirCount) {
  DIR *dir;
  struct dirent *entry;

  dir = opendir(dirName);
  if (dir == NULL) {
    SERROR("Failed to open \"%s\"", dirName);
    pTotalDirCount->items = -1;
    pTotalDirCount->files = -1;
    pTotalDirCount->subdirectories = -1;
    atomic_store(&ez_errorGlobal, EZ_ERROR_FILE_PERSMISSION_DENIED);
    return;
  }

  while ((entry = readdir(dir)) != NULL) {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
      continue;
    }
    if (entry->d_type == DT_REG) {
      pTotalDirCount->files++;
    } else if (entry->d_type == DT_DIR) {
      pTotalDirCount->subdirectories++;
      char childDir[2048];
      strcpy(childDir, dirName);
      strcat(childDir, "/");
      strcat(childDir, entry->d_name);
      ez_dir_count_items_recursive(childDir, pTotalDirCount);
    }
    pTotalDirCount->items++;
  }
  closedir(dir);
};

void ez_dir_get_items(const char *dirName, ez_DirItem **ppItems) {
  ez_DirCount dirCount = ez_dir_count_items(dirName);
  *ppItems = (ez_DirItem *)malloc(sizeof(ez_DirItem) * dirCount.items);

  // help me
  ez_DirItem *res = *ppItems;
  char pathBuffer[MAX_CHARS] = {};

  DIR *dir;
  struct dirent *entry;

  dir = opendir(dirName);
  if (dir == NULL) {
    SERROR("Failed to open \"%s\"", dirName);
    atomic_store(&ez_errorGlobal, EZ_ERROR_FILE_PERSMISSION_DENIED);
    return;
  }

  size_t itemPos = 0;

  while ((entry = readdir(dir)) != NULL) {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
      continue;
    }

    snprintf(pathBuffer, MAX_PATH, "%s/%s", dirName, entry->d_name);
    STRACE("Found item with full path: %s", pathBuffer);

    res[itemPos].name = strdup(pathBuffer);
    if (entry->d_type == DT_REG) {
      res[itemPos].type = EZ_DIR_ITEM_TYPE_FILE;
    } else if (entry->d_type == DT_DIR) {
      res[itemPos].type = EZ_DIR_ITEM_TYPE_SUBDIRECTORY;
    }
    itemPos++;
  }
  closedir(dir);
};

static void recursive_fn(const char *dirName, ez_DirItem *res,
                         size_t *pItemPos) {

  char pathBuffer[MAX_CHARS];

  DIR *dir;
  struct dirent *entry;

  dir = opendir(dirName);
  if (dir == NULL) {
    SERROR("Failed to open \"%s\"", dirName);
    atomic_store(&ez_errorGlobal, EZ_ERROR_FILE_PERSMISSION_DENIED);
    return;
  }

  while ((entry = readdir(dir)) != NULL) {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
      continue;
    }
    size_t localPos = *pItemPos;
    (*pItemPos)++;

    snprintf(pathBuffer, MAX_PATH, "%s/%s", dirName, entry->d_name);
    STRACE("Found item with full path: %s", pathBuffer);

    res[localPos].name = strdup(pathBuffer);
    if (entry->d_type == DT_REG) {
      res[localPos].type = EZ_DIR_ITEM_TYPE_FILE;
    } else if (entry->d_type == DT_DIR) {
      res[localPos].type = EZ_DIR_ITEM_TYPE_SUBDIRECTORY;
      recursive_fn(pathBuffer, res, pItemPos);
    }
  }
  closedir(dir);
}

ez_DirCount ez_dir_get_items_recursive(const char *dirName,
                                       ez_DirItem **ppItems) {
  ez_DirCount totalCount = {};
  ez_dir_count_items_recursive(dirName, &totalCount);

  if (totalCount.items <= 0) {
    *ppItems = NULL;
    return totalCount;
  }
  *ppItems = (ez_DirItem *)malloc(sizeof(ez_DirItem) * totalCount.items);

  // help me
  size_t current = 0;
  recursive_fn(dirName, *ppItems, &current);

  return totalCount;
};


int ez_dir_item_exists(const char *itemName) {
  // recursively look through all files in the working environment to see if
  // the file specified exists.
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
  // recursively look through all files in the working environment to see if
  // the file specified exists.
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
  return system(command);
}
#endif
