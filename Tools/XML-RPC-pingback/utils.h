#ifndef UTILS_H
#define UTILS_H

#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define PATH_SZ 512
#define PARAM_SZ 16
#define LF 0x0A
#define FIELD_RND (rand() & 8 + 1)

typedef struct url_t {
  char* url_host;
  char* url_uri;
} url_t;

url_t* utils_format_url(const char* url_resource);
char* utils_rand_str(size_t len);
char* utils_rand_params(size_t numb_params);
void utils_ini_rand();
int utils_last_char(char* str, char sub_char);
BOOL utils_list_push(const char* file_loc);
BOOL utils_is_int(const char* str);
BOOL utils_file_check(const char* file_location);

#endif // UTILS_H
