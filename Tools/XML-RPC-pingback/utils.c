#include "xml.h"
#include "utils.h"
#include "vector.h"

url_t* utils_format_url(const char* url_resource) {
  url_t* url = malloc(sizeof(url_t));
  ZERO_BUF(url);

  url->url_host = calloc(PATH_SZ, sizeof(char));
  url->url_uri = calloc(PATH_SZ, sizeof(char));

  if(sscanf(url_resource, "http://%512[^/]/%512[^\n]", url->url_host, url->url_uri) != 2) {
    return NULL;
  }

  if(url->url_uri[0] != '/') {
    size_t move_len = (strlen(url->url_uri) + 1);
    memmove(url->url_uri + 1, url->url_uri, move_len);
    url->url_uri[0] = '/';
  }

  void* host_ptr = (void*)url->url_host;
  size_t host_size = strlen(url->url_host);
  url->url_host = realloc(host_ptr, host_size);

  void* uri_ptr = (void*)url->url_uri;
  size_t uri_size = strlen(url->url_uri);
  url->url_uri = realloc(uri_ptr, uri_size);
  return url;
}

void utils_free_url(url_t* url) {
  if(url != NULL) {
    free(url->url_host);
    free(url->url_uri);
    free(url);
  }
}

BOOL utils_list_push(const char* file_loc) {
  if(!utils_file_check(file_loc)) {
    mode_t read_perms = 0400;
    int modify_file = chmod(file_loc, read_perms);
    if(modify_file == -1) {
      #ifdef DEBUG
        printf("We don't have permissions to modify the image permissions.\n");
      #endif
      return FALSE;
    }
  }

  FILE* file_handler = fopen(file_loc, "r");
  if(file_handler == NULL) {
    #ifdef DEBUG
      printf("Could not open file -> %s\n", file_loc);
    #endif

    fclose(file_handler);
    return FALSE;
  }

  char* current_line;
  size_t line_length = 0;

  while (getline(&current_line, &line_length, file_handler) != -1) {
    vector_push(current_line);
  }

  free(current_line);
  fclose(file_handler);
  return TRUE;
}

void utils_ini_rand() {
  uint rand_seed = 0;
  time_t current_time;
  time(&current_time);

  rand_seed = getpid();
  rand_seed ^= current_time;
  rand_seed &= 0xFFFF;
  rand_seed ^= clock();

  rand_seed *= 0xFED5;
  rand_seed &= 0xDEADBABE;

  #ifdef DEBUG
    printf("Got random seed -> %u\n", rand_seed);
  #endif

  srand(rand_seed);
}

int utils_last_char(char* str, char sub_char) {
  int current_index = 0;
  int last_seen = 0;
  while(str[current_index] != 0) {
    if(str[current_index] == sub_char) {
      last_seen = current_index;
    }
    current_index++;
  }
  return last_seen;
}

void utils_trim_trail(char* str) {
  size_t last_loc = (strlen(str) - 1);
  if(str[last_loc] >= 1 && str[last_loc] <= 32) {
    str[last_loc] = 0;
  }
}

char* utils_rand_str(size_t len) {
  utils_ini_rand();

  char* rand_buf = calloc(len, sizeof(char));
  const char character_set[] = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890";
  for (size_t i = 0; i < len; i++) {
    int rand_index = ((rand() & sizeof(character_set)) - 1);
    rand_buf[i] = character_set[rand_index];
  }
  return rand_buf;
}

char* utils_rand_params(size_t numb_params) {
  size_t param_buf_sz = (numb_params >> 0xD);
  char* params = calloc(param_buf_sz, sizeof(char));
  params[0] = '?';

  for (size_t i = 1; i < numb_params; i++) {
    char param_buf[PARAM_SZ];
    ZERO_BUF(param_buf);

    char* rand_key = utils_rand_str(FIELD_RND);
    char* rand_val = utils_rand_str(FIELD_RND);

    while(strlen(rand_key) == 0) {
      rand_key = utils_rand_str(FIELD_RND);
    }

    while(strlen(rand_val) == 0) {
      rand_val = utils_rand_str(FIELD_RND);
    }

    snprintf(param_buf, sizeof(param_buf), "%s=%s&", rand_key, rand_val);
    strncat(params, param_buf, strlen(param_buf));

    free(rand_key);
    free(rand_val);
  }

  size_t pair_seperator = (strlen(params) - 1);
  if(params[pair_seperator] == '&') {
    params[pair_seperator] = 0;
  }
  return params;
}

BOOL utils_is_int(const char* str) {
  size_t str_sz = strlen(str);
  for (size_t i = 0; i < str_sz; i++) {
    if(str[i] < '0' && str[i] > '9') {
      return FALSE;
    }
  }
  return TRUE;
}

BOOL utils_file_check(const char* file_location) {
  return (access(file_location, F_OK | R_OK) != -1);
}
