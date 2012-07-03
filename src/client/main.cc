#include <iostream>
#include <string.h>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "common/die.h"
#include "common/defaults.h"

int main(void)
{
  struct sockaddr_in sad;
  int fd;

  std::cout << "Setting up udp socket on port " << SERVER_PORT << std::endl;

  if (0 > (fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)))
    die("socket");

  memset(&sad, 0, sizeof(sad));
  sad.sin_family = AF_INET;
  sad.sin_port = htons(SERVER_PORT);
  if (!inet_aton(SERVER_ADDR, &sad.sin_addr))
    die("inet_aton");

  connect(fd, reinterpret_cast<struct sockaddr*>(&sad), sizeof(sad));
  send(fd, "HI", 3, 0);

  close(fd);

  return 0;
}
