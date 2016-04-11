#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>  

#define MAXSIZE 1024
#define SERV_PORT 5060

int main(int argc,char *argv[])
{
    if(argc != 3)
    {
      printf("pls specify the ip address and port of peer\n");
      return 0;
    }
 
    struct sockaddr_in servaddr, clientaddr;
    char buf[MAXSIZE];
    int sockfd, client_socketfd;
    
    int len;
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    bzero(&clientaddr, sizeof(servaddr));  
    servaddr.sin_family = AF_INET;
 //inet_pton(AF_INET, "10.2.19.99", &servaddr.sin_addr);
    int port = atoi(argv[2]);
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);  
    
    int ret = bind(sockfd,(struct sockaddr*)&servaddr,sizeof(struct sockaddr));
    listen(sockfd, 10);

    unsigned int sin_size=sizeof(struct sockaddr_in); 

    client_socketfd = accept(sockfd,(struct sockaddr *)&clientaddr,&sin_size);
  //  connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    printf("accept client %s\n",inet_ntoa(clientaddr.sin_addr)); 

    while((len=recv(client_socketfd,buf,MAXSIZE,0))>0)
    {  
       printf("the received bytes number is: %d\n", len);
       buf[len]='\0';  
       //printf("%s\n",buf);  
    } 


    close(client_socketfd);
    close(sockfd);
    return 0;
}
