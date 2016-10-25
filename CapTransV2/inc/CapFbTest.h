/*
 * CapFbTest.h
 *
 *  Created on: Oct 11, 2016
 *      Author: tla001
 */

#ifndef CAPFBTEST_H_
#define CAPFBTEST_H_

#include "Common.h"

class CapFbTest {
public:
	CapFbTest();
	virtual ~CapFbTest();

	static void v4l2Test();
	static void framebufferTest();

	void CapVideoInit();
	void dispVideo();

private:
	unsigned char *rgbFrame;
	unsigned char *yuv420Frame;
	unsigned char *yuvFrame;
	unsigned int len;
	int screen_w;
	int screen_h;

	V4L2Capture *cap;
	FrameBufferDisp *fb;
	IpuProcess *ipu;
	SDLDisp *sdl;
	EncodeSaver *encoder;
	EncodeSaver2 *encoder2;

};

#endif /* CAPFBTEST_H_ */
