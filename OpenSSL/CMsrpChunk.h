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
#define MAXBODYSZIE 10240
#define MAXHEADERSIZE 1000
class CMsrpChunk
{
private:
	char *p;
    string strTransactionID;    
    string strFromPath;
    string strToPath;
    string strMethod;
    string strMsgID;
    string strSuccReport;
    string strFailReport;
    string strConType;
    string strByteRange;
    string strStartLine;
    string strEndLine;
    char chunkbuf[MAXBODYSZIE + MAXHEADERSIZE];
    void buildStrByteRange(int filestart, int filesize);
    int m_bodylength;

public:
	CMsrpChunk();
	CMsrpChunk(unsigned int transNo,string& transid, string& method, string& to, 
		string& from, string& msgid, string& succrepot, string& failreport, string& contype,
		int filestart, int filesize, int bodylengh, char* pBody,string& end);
    int getChunkTotalLength();
    char* getChunkMsg();

}; 
