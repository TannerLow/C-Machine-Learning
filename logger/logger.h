#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdbool.h>

typedef struct {
    char filename[100];
    FILE* file;
} Logger;

bool logger_create(Logger* logger, const char* filename);
void logger_close(Logger* logger);

extern Logger global_logger;

#endif // LOGGER_H