#include "tests.h"
#include "error/state.h"
#include "error/logs.h"

#include "io/dir.h"
#include "io/file.h"
#include <stdio.h>
#include <string.h>

void ez_run_tests() {

  SINFO("Running tests...");
  ez_DirCount dirCount = ez_dir_count_items("src");

  SINFO("%d files and %d subdirectories found!\n", dirCount.files,
         dirCount.subdirectories);

  ez_DirItem *dirItems;

  ez_dir_get_items("src", &dirItems);

  for (int i = 0; i < dirCount.items; i++) {
    SINFO("FOUND: \"%s\"\n", dirItems[i].name);
  }

  ez_DirItem *dirItemsRecursive = NULL;
  ez_DirCount dirCountRecursive =
      ez_dir_get_items_recursive("src", &dirItemsRecursive);

  for (int i = 0; i < dirCountRecursive.items; i++) {
    SINFO("Recursive Search yielded: \"%s\"\n", dirItemsRecursive[i].name);
  }
}
