#pragma once
#include <stdio.h>

typedef void (*test_fn_type)(int* total, int*passed);


void ez_run_tests();
