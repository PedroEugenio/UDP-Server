#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
//---------------------------------------------------------
// trata mensagens de erro
void error(char *msg) {
    perror(msg);
    exit(0);
}
//---------------------------------------------------------
int main(int argc, char *argv[]) {

   int sock, length, fromlen, n;
   struct sockaddr_in server;
   struct sockaddr_in from;
   char buf[1024];
   char text_msg[256];// message (text) to send
   char frase[256]; //frase recebida pelo utilizador
   int n_util;

   if (argc < 2) {
      fprintf(stderr, "ERROR, no port provided\n");
      exit(0);
   }

   // cria socket datagram (UDP)
   sock=socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0) error("Error in opening socket");
   length = sizeof(server);
   bzero(&server,length);
   server.sin_family=AF_INET;
   server.sin_addr.s_addr=INADDR_ANY;
   server.sin_port=htons(atoi(argv[1]));
   if (bind(sock,(struct sockaddr *)&server,length)<0)
       error("Error in binding");
   fromlen = sizeof(struct sockaddr_in);

   //Pede um numero inteiro e uma frase ao utilizador
   printf("Enter a message: ");
   bzero(frase,256);
   fgets(frase,255,stdin);
   printf("\nEnter a number: ");
   scanf("%d", &n_util);
   n_util=htonl(n_util);

   //forever cicle for clients
   while (1) {
       //receives datagrama that identifies client
       n = recvfrom(sock,buf,1024,0,(struct sockaddr *)&from,&fromlen);
       if (n < 0) error("Error in recvfrom");
       write(1,"Received a datagram: ",21);
       write(1,buf,n);

       //sends response text
       n = sendto(sock, frase, strlen(frase), 0, (struct sockaddr *)&from, fromlen);
       if (n  < 0) error("Error in sendto");

       n = sendto(sock, &n_util, sizeof(n_util), 0, (struct sockaddr *)&from, fromlen);
       if (n  < 0) error("Error in sendto");
   }//while
 }
//---------------------------------------------------------
//---------------------------------------------------------
