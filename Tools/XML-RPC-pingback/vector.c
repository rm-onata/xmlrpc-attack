#include "xml.h"
#include "vector.h"
#include "utils.h"

struct node* node_start = NULL;

void vector_push(char* node_path) {
  if(node_path != NULL) {
    url_t* url_parts = utils_format_url(node_path);
    if(url_parts != NULL) {
      struct node* node_link = malloc(sizeof(struct node));
      ZERO_BUF(node_link);

      node_link->host = url_parts->url_host;
      node_link->path = url_parts->url_uri;
      node_link->next_ptr = node_start;

      node_start = node_link;
    }
    free(url_parts);
  }
}

static void vector_destory_element(struct node* element) {
  free(element->host);
  free(element->path);
  free(element);
}

void vector_destroy() {
  struct node* node_tmp;
  FOR_EACH(node_start, node_tmp) {
    if(node_tmp == NULL) {
      break;
    }
    vector_destory_element(node_tmp);
  }
  node_start = NULL;
}

int vector_size() {
  int node_elements = 0;
  struct node* node_tmp;
  FOR_EACH(node_start, node_tmp) {
    node_elements++;
  }
  return node_elements;
}

struct node* vector_rand() {
  uint vector_index = (rand() & vector_size());
  uint current_index = 0;
  struct node* node_tmp;
  FOR_EACH(node_start, node_tmp) {
    if(current_index == vector_index) {
      return node_tmp;
    }
    ++current_index;
  }
  return node_start;
}
