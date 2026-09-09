#include "tests.h"
#include <stdio.h>
#include "error/state.h"
#include <string.h>
#include "io/dir.h"
#include "io/file.h"


void ez_run_tests(){

    printf("Running tests...");
    ez_DirCount dirCount = ez_dir_count_items("src");

    printf("%d files and %d subdirectories found!\n", dirCount.files,
           dirCount.subdirectories);

    ez_DirItem *dirItems;

    ez_dir_get_items("src", &dirItems);

    for (int i = 0; i < dirCount.items; i++) {
      printf("FOUND: \"%s\"\n", dirItems[i].name);
    }

    ez_DirItem *dirItemsRecursive = NULL;
    ez_DirCount dirCountRecursive =
        ez_dir_get_items_recursive("src", &dirItemsRecursive);

    for (int i = 0; i < dirCountRecursive.items; i++) {
      printf("Recursive Search yielded: \"%s\"\n", dirItemsRecursive[i].name);
    }


}
