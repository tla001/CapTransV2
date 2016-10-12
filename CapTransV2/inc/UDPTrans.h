/*
 * UDPTrans.h
 *
 *  Created on: Oct 11, 2016
 *      Author: tla001
 */

#ifndef UDPTRANS_H_
#define UDPTRANS_H_

#include "Common.h"

class UDPTrans {

#define SERVER_PORT 8000
#define MAXLINE 1024
#define BLOCKSIZE 38400
#define SEVER_IP "219.216.88.150"

public:
	UDPTrans();
	virtual ~UDPTrans();

	void initUDPTrans();
	void sendUDPdata(void *datas,unsigned int size);
	void recUDPdata(char *udpRecbuf);
	void closeUDPtrans();

private:
	struct udptransbuf//包格式
	{
		char buf[BLOCKSIZE];//存放数据的变量
		char flag;//标志
	};
	struct sockaddr_in serveraddr;
	int confd;
	char udpRecbuf[MAXLINE];
};

#endif /* UDPTRANS_H_ */
