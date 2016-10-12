/*
 * main.cpp
 *
 *  Created on: Oct 11, 2016
 *      Author: tla001
 */

#include "Common.h"
#include "CapFbTest.h"

int main()
{
//	CapFbTest::v4l2Test();
//	CapFbTest::framebufferTest();
//	printf("hello\n");
	CapFbTest *ct=new CapFbTest();
	ct->CapVideoInit();
	while(1){
		ct->dispVideo();
	}
	return 0;
}
