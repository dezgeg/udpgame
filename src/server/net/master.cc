#include <iostream>
#include <string.h>
#include <pthread.h>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "common/die.h"
#include "common/defaults.h"

#include "server/net/threadpool.h"
#include "server/net/slave.h"

#define BUFLEN 512
#define MAX_CLIENTS 5

int main(void) {
  struct sockaddr_in si_me, si_other;
  int fd;
  char buf[BUFLEN];
  socklen_t slen = sizeof(si_other);

  ThreadPool thread_pool(MAX_CLIENTS);
  thread_pool.Init();

	std::cout << "Setting up udp socket on port " << SERVER_PORT << std::endl;

  if (0 > (fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)))
    die("socket");

  memset(&si_me, 0, sizeof(si_me));
  si_me.sin_family = AF_INET;
  si_me.sin_port = htons(SERVER_PORT);
  si_me.sin_addr.s_addr = htonl(INADDR_ANY);
  if (0 > bind(fd, reinterpret_cast<const sockaddr*>(&si_me), sizeof(si_me)))
    die("bind");

  for (;;) {
    memset(&buf, 0, sizeof(buf));
    if (0 > recvfrom(fd, buf, BUFLEN, 0, reinterpret_cast<sockaddr*>(&si_other), &slen))
      die("recvfrom");
    std::cout << "Got message from " << inet_ntoa(si_other.sin_addr) << std::endl;

    if (strncmp("HI", buf, 2) == 0) {
      Slave* slave = thread_pool.AssignConnection(&si_other);
      if (slave == nullptr) {
        std::cout << "Server is full, disconnecting client. " << std::endl;
        continue;
      }
      sprintf(buf, "OK %hu\n", slave->getListeningPort());
      sendto(fd, buf, strlen(buf), 0, (const sockaddr*)&si_other, slen);
      continue;
    }
    const char byemsg[] = "BYE";
    sendto(fd, byemsg, strlen(byemsg), 0, (const sockaddr*)&si_other, slen);
  }

  thread_pool.Destroy();
  close(fd);

	return 0;
}
