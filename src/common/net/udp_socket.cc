#include <assert.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>

#include "common/net/udp_socket.h"

UDPSocket::UDPSocket(uint16_t port):
  m_port(port),
  m_fd(-1)
{
  m_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  assert(0 <= m_fd);
  memset(&m_sa_me, 0, sizeof(m_sa_me));
  m_sa_me.sin_family = AF_INET;
  m_sa_me.sin_port = htons(m_port);
  m_sa_me.sin_addr.s_addr = htonl(INADDR_ANY);
  assert(0 ==
    bind(m_fd,
      reinterpret_cast<const sockaddr*>(&m_sa_me), sizeof(m_sa_me)));
  set_nonblocking();
}

UDPSocket::~UDPSocket()
{
  close(m_fd);
}

void UDPSocket::set_nonblocking()
{
  fcntl(m_fd, F_SETFL, fcntl(m_fd, F_GETFL) | O_NONBLOCK);
}

int UDPSocket::fd() const
{
  return m_fd;
}

