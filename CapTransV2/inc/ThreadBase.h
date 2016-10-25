/*
 * ThreadBase.h
 *
 *  Created on: Oct 25, 2016
 *      Author: tla001
 */

#ifndef THREADBASE_H_
#define THREADBASE_H_

extern "C"{
#include <pthread.h>
#include <stdio.h>
}

class ThreadBase {
public:
	ThreadBase();
	virtual ~ThreadBase();

	int start();
	int stop();
	int isStop();
	int isStart();
	virtual void run()=0;
private:
	static void *thread_proxy_func(void *args);
	int stopFlag;//1Í£Ö¹
	int startFlag;//1¿ªÊ¼
	pthread_t tid;
};

#endif /* THREADBASE_H_ */
