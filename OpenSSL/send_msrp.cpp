#include "CMsrpChunk.h"
using namespace std;


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
{
   ERR_print_errors_fp(stderr);
   exit;
}

printf("Connected with %s encryption\n", SSL_get_cipher(ssl));
// ShowCerts(ssl);
string strFromPath("msrps://10.2.22.150:22581/8V2aLr53kkElox9KMLn76i2SM40q3;tcp");
string strToPath(argv[3]);
string strTransactionID("n14s00i3t0+");
unsigned int startNo = 20000;
stringstream ss1;
ss1 << startNo; 
string strStartNo = ss1.str();
//string strTransactionID = strTranIDwoNO + strStartNo;

string strMsgID("87652491++0");
string send("SEND");
string fail("no");
string succ("no");
string contype("text/plain");
string endch;

fstream file;
file.open("/root/OpenSSL/g711a.pcap");
if(!file)
{
    printf("open file failed!\n");
    return -1;
}
file.seekg(0,ios::end);
unsigned int filesize = file.tellg();
file.seekg(0);
printf("file size is: %d\n",filesize);
char *tmp_filebuf;
   
int divisor = filesize/MAXBODYSZIE;
int dremainder = filesize%MAXBODYSZIE;
unsigned int i=0;
for(i = 0; i < divisor;i++)
{
    printf("i is: %d\n", i);
    tmp_filebuf = new char[MAXBODYSZIE];
    file.read(tmp_filebuf,MAXBODYSZIE);
    file.seekg(i*MAXBODYSZIE + MAXBODYSZIE);
    if(dremainder==0 && i==divisor-1)
    {
        endch = "$";
    }
    else
    {
        endch = "+";
    }
        
    CMsrpChunk objChunk(i+startNo,strTransactionID,send,strToPath, strFromPath, strMsgID, fail, succ,contype,1+i*MAXBODYSZIE,filesize,MAXBODYSZIE,tmp_filebuf, endch);
    len = SSL_write(ssl,objChunk.getChunkMsg(),objChunk.getChunkTotalLength());
    if(len > 0)
    {
        printf("successfully sent:%d, chucklength is:%d\n", len,objChunk.getChunkTotalLength());
    }
    else
    {
        printf("unsuccessfully sent:%d\n", len);
    }
    delete[] tmp_filebuf;
}
if(dremainder != 0)
{
    printf("i is: %d\n", i);
    endch = "$";
    tmp_filebuf = new char[dremainder];
    file.read(tmp_filebuf,dremainder);
    //file.seekg(i*MAXBODYSZIE + MAXBODYSZIE);
    CMsrpChunk objChunk(i+startNo,strTransactionID,send,strToPath, strFromPath, strMsgID, fail, succ,contype,1+i*MAXBODYSZIE,filesize,dremainder,tmp_filebuf, endch);
    len = SSL_write(ssl,objChunk.getChunkMsg(),objChunk.getChunkTotalLength());
    if(len > 0)
    {
        printf("successfully sent:%d, chucklength is:%d\n", len,objChunk.getChunkTotalLength());
    }
    else
    {
        printf("unsuccessfully sent:%d\n", len);
    }
    delete[] tmp_filebuf;
}

file.close();


//sleep(2);
SSL_shutdown(ssl);
SSL_free(ssl);
close(sockfd);
SSL_CTX_free(ctx);
return 0;
}
