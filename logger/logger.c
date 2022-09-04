#include "logger.h"
#include <string.h>
#include <assert.h>

bool logger_create(Logger* logger, const char* filename) {
    assert(logger != NULL);
    assert(filename != NULL);
    assert(strlen(filename) < 100);

    if (logger == NULL || filename == NULL) return false;
    if (strlen(filename) >= 100) return false;

    logger->file = fopen(filename, "w");

    bool success = true;
    if (logger->file != NULL) {
        strcpy(logger->filename, filename);
    }
    else {
        logger->filename[0] = '\0';
        success = false;
    }

    return success;
}

void logger_close(Logger* logger) {
    assert(logger != NULL);

    if (logger != NULL && logger->file != NULL) {
        int close = fclose(logger->file);
        assert(close == 0);
        logger->filename[0] = '\0';
    }
}