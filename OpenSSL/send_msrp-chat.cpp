#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <sstream>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <openssl/err.h>
#include <openssl/ssl.h> 

using namespace std;
#define MAXBUF 20480
#define CHUNKSIZE 10240
void ShowCerts(SSL * ssl)
{
    X509 *cert;
    char *line;

    cert = SSL_get_peer_certificate(ssl);
    if (cert != NULL) {
        printf("cert info:\n");
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        printf("cert: %s\n", line);
        free(line);
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        printf("by: %s\n", line);
        free(line);
        X509_free(cert);
    } else
        printf("no cert info\n");
}

int main(int argc, char **argv)
{

int sockfd, len;
struct sockaddr_in dest;
char buffer[MAXBUF + 1];
SSL_CTX *ctx;
SSL *ssl;
SSL_library_init();
OpenSSL_add_all_algorithms();
SSL_load_error_strings();
ctx = SSL_CTX_new(SSLv23_client_method());
if (ctx == NULL)
{
  ERR_print_errors_fp(stdout);
  exit(1);
}
if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
{
	perror("Socket");
	exit(errno);
}
printf("socket created\n");
bzero(&dest, sizeof(dest));
dest.sin_family = AF_INET;
dest.sin_port = htons(atoi(argv[2]));
if (inet_aton(argv[1], (struct in_addr *) &dest.sin_addr.s_addr) == 0)
{
        perror(argv[1]);
        exit(errno);
}
printf("address created\n");


if (connect(sockfd, (struct sockaddr *) &dest, sizeof(dest)) != 0) 
{
    perror("Connect ");
    exit(errno);
}

printf("server connected\n");

ssl = SSL_new(ctx);
SSL_set_fd(ssl, sockfd);
if (SSL_connect(ssl) == -1)
   ERR_print_errors_fp(stderr);
else 
{
    printf("Connected with %s encryption\n", SSL_get_cipher(ssl));
   // ShowCerts(ssl);
    bzero(buffer, MAXBUF + 1);



    fstream file;
    file.open("/root/OpenSSL/ping.cap");
    if(!file)
    {
        printf("open file failed!\n");
    }
    file.seekg(0,ios::end);
    unsigned int n = file.tellg();
    file.seekg(0);
    printf("file siz is: %d",n);
    char *tmp_filebuf;
    if(n < CHUNKSIZE)
    {
        printf("file is smaller than CHUNKSIZE\n");
        tmp_filebuf = new char[n+1];
        file.read(tmp_filebuf,n);
        tmp_filebuf[n] = '\0';
    }
    file.close();

    string strFromPath("From-Path: msrps://10.2.22.150:22581/8V2aLr53kkElox9KMLn76i2SM40q3;tcp\r\n");
    string strToPath(argv[3]);
    strToPath="To-Path: " + strToPath;
   // string strSeparateline("dfaffefaff");
    string strTranIDwoNO("n14s00i3t0+");
    unsigned int startNo = 20000;
    stringstream ss1;
    ss1 << startNo; 
    string strStartNo = ss1.str();
    string strTransactionID = strTranIDwoNO + strStartNo;

    string strStartline = "MSRP " + strTransactionID + " SEND\r\n";       

    string strSuccReport = "Success-Report: no\r\n";
    string strFailReport = "Failure-Report: no\r\n";
    string strConType = "Content-Type: text/plain\r\n\r\n";
    string strBody(tmp_filebuf);// = "Hey Bobi, are you there?\r\n";
    string strEndLine = "\r\n-------" + strTransactionID + "$\r\n";

    stringstream ss2;
    ss2 << n;
    string strByteNo = ss2.str();
    string strByteRange = "Byte-Range: 1-*/" + strByteNo + "\r\n";
    string strMsgID = "Message-ID: 87652491\r\n";

    string str_tmpChunkMsg = strStartline + strToPath + strFromPath + strMsgID + strSuccReport + strFailReport + strByteRange + strConType;// + strBody + strEndLine;

    char *p = buffer;
    memcpy(p,str_tmpChunkMsg.c_str(),str_tmpChunkMsg.size());
    p+=str_tmpChunkMsg.size();
    memcpy(p,tmp_filebuf,n);
    p+=n;
    memcpy(p,strEndLine.c_str(),strEndLine.size());
    p+=strEndLine.size();

    cout << strEndLine <<endl;
    int sum = str_tmpChunkMsg.size() + strEndLine.size(); 
    cout << sum <<endl;
    //cout <<endl;
    //cout << strChunkMsg <<endl;
    //strcpy(buffer,strChunkMsg.c_str());
  //    len = write(sockfd, buffer, strlen(buffer));
    for(int i=0;i<1;i++)
    {
        len = SSL_write(ssl, buffer, p-buffer);
        if(len >0)
        {
            printf("successfully sent:%d\n", len);
        }
        else
        {
            printf("unsuccessfully sent:%d\n", len);
        }
   }
   
}
sleep(2);
SSL_shutdown(ssl);
SSL_free(ssl);
close(sockfd);
SSL_CTX_free(ctx);
return 0;
}
