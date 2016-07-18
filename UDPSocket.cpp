///////////////////////////////////////////////////////////
// UDPSocket.cpp
///////////////////////////////////////////////////////////

#include <stdAfx.h>
#include <iostream>
using namespace std;

#include "UdpSocket.h"

extern char *IP1, *IP2, *IP3;

// UdpSocket Constructor
UdpSocket::UdpSocket(char *ip, int port) {
#ifdef WIN32
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2); 
	int err = WSAStartup( wVersionRequested, &wsaData );
	if (0 != err) {
		cerr << "winsock DLL not found\n";
		WSACleanup();
		exit(-1);
	}
#endif

	descriptor = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (INVALID_SOCKET == descriptor) {
		cerr << "socket not created\n" <<WSAGetLastError() << "\n";
		WSACleanup();
		exit(-1);
	}
}

// UdpSocket destructor
UdpSocket::~UdpSocket(void) {
	closesocket(descriptor);
#ifdef WIN32
	WSACleanup();
#endif
}

// UdpOutSocket Constructor
UdpOutSocket::UdpOutSocket(char *ip, int rPort) : UdpSocket(ip, rPort) {
	setPort(ip, rPort);
}

// UdpInSocket Constructor
UdpInSocket::UdpInSocket(char *ip, int lPort)   : UdpSocket(ip, lPort) {
	setPort(ip, lPort);
}

//*ip	-- remote IP
//lPort -- local port
//rPort -- remote port
// Sets the port according to the input values and current OS
void UdpInSocket::setPort(char *ip, int lPort) {
	localPort.sin_family = AF_INET;
	localPort.sin_addr.s_addr = INADDR_ANY;
	localPort.sin_port = htons(lPort);
	
	bind(descriptor,(struct sockaddr *)&localPort,sizeof(localPort));

	//Setting the socket size
	int blen = 1024 ;
	setsockopt (descriptor, SOL_SOCKET, SO_RCVBUF,(char *) &blen, sizeof (blen));
	
	/*
	int setsockopt (
  SOCKET s,                 
  int level,                
  int optname,              
  const char FAR * optval,  
  int optlen                
);
*/

#ifdef WIN32
	if (0 != WSAGetLastError()) {
		cerr << "socket not bound\n" << WSAGetLastError() << "\n";
		exit(-1);
	}
#endif
}

// Sets the port according to the input values
void UdpOutSocket::setPort(char *ip, int rPort) {
	remotePort.sin_family = AF_INET;
	remotePort.sin_addr.s_addr = inet_addr(ip); // garlin-for testing
//	remotePort.sin_addr.s_addr = inet_addr(IP1);
	remotePort.sin_port = htons(rPort);
/*
	int len;
	int size = sizeof(len);
	char str[10];
	getsockopt(descriptor,SOL_SOCKET,SO_RCVBUF,(char *) &len, &size);
	MessageBox(NULL,itoa(len,str,10),"Rcv Buffer Size",0);
	*/
}

// Sends inputted data to the output port
int UdpOutSocket::send(char *buffer, int len) {
	
	return sendto(descriptor, buffer, len,0, (struct sockaddr *)&remotePort, sizeof(remotePort));
}

// Receives data from the input port
int UdpInSocket::receive(char *buffer, int len) {
	
	struct sockaddr_in remotePort;
	int dummy =sizeof remotePort; // length of remote port is not 
	return recvfrom(descriptor, buffer, len, 0, (struct sockaddr *)&remotePort, &dummy);
}

// Sets the port to "No Block", thus, while receiving do not wait for the input data if no data is available
void UdpInSocket::setNoBlock(void) {
	unsigned long dummy = 1;
	if (0 != ioctlsocket(descriptor, FIONBIO, &dummy)) {
		cerr << "failed to set socket to non blocking mode\n";
		cerr << WSAGetLastError() << '\n';
		exit(-1);
	}
}
