#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define DEFAULT_PORT 8824

int main(int argc, char *argv[])
{
  int sockfd, commfd;
  unsigned int clilen;
  struct sockaddr_in s_addr, c_addr;

  int portno = DEFAULT_PORT;

  unsigned int i;
  uint16_t spillcount;
  int boardid = 0;

  char header[16];
  char footer[16];

  printf("Port: %d\n", portno);

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd < 0) {
    perror("Error opening socket");
    exit(1);
  }

  bzero((char *) &s_addr, sizeof(s_addr));

  s_addr.sin_family = AF_INET;
  s_addr.sin_addr.s_addr = INADDR_ANY;
  s_addr.sin_port = htons(portno);

  if (bind(sockfd, (struct sockaddr *) &s_addr, sizeof(s_addr)) < 0) {
    perror("Error binding socket");
    exit(1);
  }

  listen(sockfd, 5);
  clilen = sizeof(c_addr);
  
  commfd = accept(sockfd, (struct sockaddr *) &c_addr, &clilen);

  if (commfd < 0) {
    perror("Error reading from socket");
    exit(1);
  }

  *((uint64_t *) header)     = 0x127056341200B0ABUL;
  *((uint32_t *) &header[8]) = 0x00705634U;

  *((uint64_t *) footer)     = 0xAAAAAA00EE0F0000UL;
  *((uint32_t *) &footer[8]) = 0xAAAAAAAAU;
  *((uint8_t *) &footer[12]) = 0xAAU;

  for (i=0; ; i++) {
    spillcount = i & 0xffff;
    header[10] &= 0xf0;
    header[10] |= boardid & 0x0f;
    *((uint8_t *) &header[11]) = (spillcount >> 8) & 0xff;
    *((uint8_t *) &header[12]) = spillcount & 0xff;
    write(commfd, header, 13);
    write(commfd, footer, 13);
  }

  return 0;
}
