#ifndef SOCKET_H
#define SOCKET_H

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <resolv.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define SOCK_ASSERT(a) (a < 0)
#define SOCK_ERROR -1

typedef int socket_t;

typedef struct conn_t {
  char* client_host;
  uint16_t client_port;
} conn_t;

socket_t socket_create(conn_t* con);
BOOL sock_write(socket_t sock, const char* data);
void sock_pseudorecv(socket_t sock);
static void socket_cleanup(struct addrinfo* addr, socket_t sock);
static void socket_hint_init(struct addrinfo* addr);

#endif // SOCKET_H
