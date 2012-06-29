#include <iostream>
#include <string.h>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "common/die.h"

#define BUFLEN 512
#define PORT 4500

int main(void)
{
  struct sockaddr_in si_me, si_other;
  int fd;
  char buf[BUFLEN];
  socklen_t slen = sizeof(si_other);

	std::cout << "Setting up udp socket on port " << PORT << std::endl;

  if (0 > (fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)))
    die("socket");

  memset(&si_me, 0, sizeof(si_me));
  si_me.sin_family = AF_INET;
  si_me.sin_port = htons(PORT);
  si_me.sin_addr.s_addr = htonl(INADDR_ANY);
  if (0 > bind(fd, reinterpret_cast<const sockaddr*>(&si_me), sizeof(si_me)))
    die("bind");

  for (int i = 0; i < 10; ++i)
  {
    if (0 > recvfrom(fd, buf, BUFLEN, 0, reinterpret_cast<sockaddr*>(&si_other), &slen))
      die("recvfrom");
    std::cout << "received packet from " << inet_ntoa(si_other.sin_addr) << std::endl;
  }

  close(fd);

	return 0;
}
