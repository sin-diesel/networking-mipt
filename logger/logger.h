

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <time.h>


enum {
	LOG_INFO = 0,
	LOG_ERR,
};

int log_init(char* path);

int pr_log_level(int log_level, char* fmt, ...);

#define pr_err(fmt, ...) pr_log_level(LOG_ERR, "%s:%d " fmt, __FILE__, __LINE__, ##__VA_ARGS__);
