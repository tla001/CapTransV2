/*
 * EncodeSaver2.h
 *
 *  Created on: Oct 12, 2016
 *      Author: tla001
 */

#ifndef ENCODESAVER2_H_
#define ENCODESAVER2_H_

#include "Common.h"
#include "ThreadBase.h"

#include <queue>

class EncodeSaver2:public ThreadBase {

#define BUFFERSIZE422 614400				//10.24
public:
	EncodeSaver2(int in_w,int in_h,int out_w,int out_h,char* out_file);
	virtual ~EncodeSaver2();

	int initDevice();						//10.24
	int closeDevice();						//10.24
	int doEncode(unsigned char *buf);		//10.24
	void run();
	int flush_encoder(AVFormatContext *fmt_ctx,unsigned int stream_index);
	int isReady();

	struct PicBuf422{						//10.24
		uint8_t a[BUFFERSIZE422];
	};

private:
	AVFormatContext* pFormatCtx;
	AVOutputFormat* fmt;
	AVStream* video_st;
	AVCodecContext* pCodecCtx;
	AVCodec* pCodec;
	AVPacket pkt;

	uint8_t* picture_buf;
	AVFrame* picture;
	uint8_t* picture422_buf;
	AVFrame* picture422;
	SwsContext* img_convert_ctx;
	queue<PicBuf422> picture422_bufs;		//10.24
	PicBuf422 picture422_buft;				//10.24

	long long frameNum;
	double video_pts;
	char* out_file ;
	int in_w;
	int in_h;
	int out_w;
	int out_h;
	int framesize;
	int framesize422;
	int basicsize;
	int ready;
};

#endif /* ENCODESAVER2_H_ */
