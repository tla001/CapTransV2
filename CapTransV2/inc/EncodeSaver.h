/*
 * EncodeSaver.h
 *
 *  Created on: Oct 11, 2016
 *      Author: tla001
 */

#ifndef ENCODESAVER_H_
#define ENCODESAVER_H_

#include "Common.h"

class EncodeSaver {
public:
	EncodeSaver(int in_w,int in_h,int out_w,int out_h,char* out_file);
	virtual ~EncodeSaver();

	int initDevice();
	int closeDevice();
	int doEncode(unsigned char *buf);
	int doEncode2(unsigned char *buf);
	int flush_encoder(AVFormatContext *fmt_ctx,unsigned int stream_index);

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
};

#endif /* ENCODESAVER_H_ */
