#pragma once

long ez_file_to_string(const char *path, char *pDestination);

int ez_file_config_to_argv(char *configString, char ***pDestination);
