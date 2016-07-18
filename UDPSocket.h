///////////////////////////////////////////////////////////
// UdpSocket.h
///////////////////////////////////////////////////////////

#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <sys/types.h>
#include <winsock.h> 
//#ifdef WIN32
//#include <winsock.h> 
//#endif

#pragma message ("Automatically linking to WS2_32.lib")
#pragma comment (lib,"ws2_32.lib")

class UdpSocket {
public:
	UdpSocket(char *ip, int port);
	~UdpSocket(void);
//protected:
	unsigned int descriptor;
};

class UdpOutSocket : public UdpSocket {
public:
	UdpOutSocket(char *ip, int rPort);
	int send(char *buffer, int len);
	void setPort(char *ip, int rPort);
private:
	struct sockaddr_in remotePort;
	
};

class UdpInSocket : public UdpSocket {
public:
	UdpInSocket(char *ip, int lPort);
	int receive(char *buffer, int len);
	void setNoBlock(void);
	void setPort(char *ip, int lPort);
private:
	struct sockaddr_in localPort;
	
};

#endif //__SOCKET_H__
