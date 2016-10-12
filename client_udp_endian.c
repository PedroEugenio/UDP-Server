#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
//---------------------------------------------------------
void error(char *);
//---------------------------------------------------------
int main(int argc, char *argv[])
{
   int sock, length, n, n_util;
   struct sockaddr_in server, from;
   struct hostent *hp;
   char buffer[256];

   if (argc != 3) { printf("Usage: server_udp port\n");
                    exit(1);
   }
   //creates datagram socket (udp)
   sock= socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0) error("socket");

   server.sin_family = AF_INET;
   hp = gethostbyname(argv[1]);
   if (hp==0) error("Unknown host");

   bcopy((char *)hp->h_addr,
        (char *)&server.sin_addr,
         hp->h_length);
   server.sin_port = htons(atoi(argv[2]));
   length=sizeof(struct sockaddr_in);

   //cria mensagem inicial (name)
   printf("Please enter your username: ");
   bzero(buffer,256);
   fgets(buffer,255,stdin);

   //envia mensagem (datagrama) inicial
   n=sendto(sock,buffer,strlen(buffer),0,&server,length);
   if (n < 0) error("Error in sendto");

   //recebe e mostra texto (mensagem)
   bzero(buffer,256);
   n = recvfrom(sock,buffer,256,0,&from, &length);
   if (n < 0) error("Error in recvfrom (text)");
   //write(1,"Got an ack: ",12);
   printf("\nText received from server: %s\n", buffer);
   //write(1,buffer,n);

   n = recvfrom(sock,&n_util,sizeof(n_util),0,&from, &length);
   if (n < 0) error("Error in recvfrom (text)");
   //write(1,"Got an ack: ",12);
   //n_util=ntohl(n_util);
   printf("\nNumber received from server: %d\n", n_util);
   //write(1,buffer,n);
}
//---------------------------------------------------------
// trata mensagens de erro
void error(char *msg) {
    perror(msg);
    exit(0);
}
//---------------------------------------------------------
//---------------------------------------------------------
