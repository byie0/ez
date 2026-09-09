#pragma once

typedef struct ez_DirCount {
  int items;
  int files;
  int subdirectories;
} ez_DirCount;

typedef enum ez_DirItemType {
  DIR_ITEM_TYPE_FILE,
  DIR_ITEM_TYPE_SUBDIRECTORY
} ez_DirItemType;

typedef struct ez_DirItem {
  ez_DirItemType type;
  char *name;
} ez_DirItem;

ez_DirCount ez_dir_count_items(const char *dirName);
// needs pTotalDirCount to be initialized to {0}
void ez_dir_count_items_recursive(const char *dirName,
                                  ez_DirCount *pTotalDirCount);

// pItems gets allocated BY the function, user can just define it.
void ez_dir_get_items(const char *dirName, ez_DirItem **ppItems);

// pItems gets allocated BY the function, user can just define it.
// returns the item count
ez_DirCount ez_dir_get_items_recursive(const char *dirName,
                                       ez_DirItem **pItems);

int ez_dir_item_exists(const char *dirName);
