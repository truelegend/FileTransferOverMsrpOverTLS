#include "CMsrpChunk.h"

CMsrpChunk::CMsrpChunk()
{
	// To do
}

CMsrpChunk::CMsrpChunk(unsigned int transNo,string& transid, string& method, string& to, 
		string& from, string& msgid, string& succrepot, string& failreport, string& contype,
		int filestart, int filesize, int bodylength, char* pBody,string& end)
{  
    bzero(chunkbuf,MAXBODYSZIE + MAXHEADERSIZE);
    stringstream ss1;
    ss1 << transNo;
    strTransactionID = transid+ss1.str();
    strStartLine = "MSRP " + strTransactionID + " " + method + "\r\n";
    strFromPath = "From-Path: " + from + "\r\n";
    strToPath = "To-Path: " + to;// + "\r\n";
    strMsgID = "Message-ID: " + msgid + "\r\n";
    strSuccReport = "Success-Report: " + succrepot + "\r\n";
    strFailReport = "Failure-Report: " + failreport + "\r\n";
    strConType = "Content-Type: " + contype + "\r\n\r\n";
    m_bodylength = bodylength;
    buildStrByteRange(filestart,filesize);
    strEndLine = "\r\n-------" + strTransactionID + end + "\r\n";

    string str_tmpChunkMsg = strStartLine + strToPath + strFromPath + strMsgID + strSuccReport + strFailReport + strByteRange + strConType;// + strBody + strEndLine;

    //printf("tmpChunkmsg and endline size are:%d,%d\n", str_tmpChunkMsg.size(),strEndLine.size());

    cout<< str_tmpChunkMsg<<endl;
    p = chunkbuf;
    memcpy(p,str_tmpChunkMsg.c_str(),str_tmpChunkMsg.size());
    p+=str_tmpChunkMsg.size();
    memcpy(p,pBody,m_bodylength);
    p+=m_bodylength;
    memcpy(p,strEndLine.c_str(),strEndLine.size());
    p+=strEndLine.size();

}
void CMsrpChunk::buildStrByteRange(int filestart, int filesize)
{
	
    stringstream ss1;
    ss1 << filestart;

    stringstream ss2; 
    ss2 << filesize;

    stringstream ss3;
    ss3 << (filestart + m_bodylength -1);

    strByteRange = "Byte-Range: " + ss1.str() + "-" + ss3.str() + "/" + ss2.str() + "\r\n";
    //cout<< "byte-range is:" <<endl;
    //cout<<strByteRange<<endl;
}

int CMsrpChunk::getChunkTotalLength()
{
  int len = p - chunkbuf;
  //printf("chunk size  excluding body is: %d\n", len-m_bodylength);
  return len;
}
char* CMsrpChunk::getChunkMsg()
{
  return chunkbuf;
}