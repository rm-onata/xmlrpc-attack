#include "xml.h"
#include "socket.h"

static void socket_hint_init(struct addrinfo* addr) {
  memset(addr, 0, sizeof(struct addrinfo));
  addr->ai_family = AF_UNSPEC;
  addr->ai_socktype = SOCK_STREAM;
  addr->ai_protocol = IPPROTO_TCP;
}

static BOOL sock_connect(socket_t sock, struct addrinfo* addr_res) {
  socklen_t sock_opt;
  int error_flag = 1;
  fd_set desc_set;
  struct timeval time_conf;

  time_conf.tv_sec = 5;
  time_conf.tv_usec = 0;
  FD_ZERO(&desc_set);
  FD_SET(sock, &desc_set);

  if(SOCK_ASSERT(connect(sock, addr_res->ai_addr, addr_res->ai_addrlen))) {
    if(select(sock + 1, NULL, &desc_set, NULL, &time_conf) > 0) {
      int ret_sock = getsockopt(sock, SOL_SOCKET, SO_ERROR, &error_flag, &sock_opt);
      if(ret_sock || sock_opt != 0) {
       close(sock);
       return FALSE;
      }
    }
  }
  return TRUE;
}

BOOL sock_write(socket_t sock, const char* data) {
  return (send(sock, data, strlen(data), MSG_DONTWAIT) > 0);
}

void sock_pseudorecv(socket_t sock) {
  char buf[8];
  recv(sock, buf, sizeof(buf), MSG_DONTWAIT);
}

socket_t socket_create(conn_t* con) {
  socket_t sock_desc;
  struct addrinfo addr_hints;
  struct addrinfo *addr_res;
  int enable_flag = 1;

  socket_hint_init(&addr_hints);

  char port_format[8];
  snprintf(port_format, sizeof(port_format), "%d", con->client_port);

  if(SOCK_ASSERT(getaddrinfo(con->client_host, port_format, &addr_hints, &addr_res) != 0)) {
    close(sock_desc);
    return SOCK_ERROR;
  }

  sock_desc = socket(addr_res->ai_family, addr_res->ai_socktype, addr_res->ai_protocol);
  if(SOCK_ASSERT(sock_desc)) {
    close(sock_desc);
    return SOCK_ERROR;
  }

  if(SOCK_ASSERT(fcntl(sock_desc, F_SETFL,
                              fcntl(sock_desc, F_GETFL, NULL) | O_NONBLOCK))) {
      freeaddrinfo(addr_res);
      close(sock_desc);
      return SOCK_ERROR;
    }

  if(SOCK_ASSERT(setsockopt(sock_desc, IPPROTO_TCP, TCP_NODELAY, &enable_flag,
                                                              sizeof(int)))) {
    freeaddrinfo(addr_res);
    close(sock_desc);
    return SOCK_ERROR;
  }

  if(!sock_connect(sock_desc, addr_res)) {
    freeaddrinfo(addr_res);
    close(sock_desc);
    return SOCK_ERROR;
  }

  freeaddrinfo(addr_res);
  return sock_desc;
}
