/*
 * EncodeSaver.cpp
 *
 *  Created on: Oct 11, 2016
 *      Author: tla001
 */

#include "EncodeSaver.h"

EncodeSaver::EncodeSaver(int in_w,int in_h,int out_w,int out_h,char* out_file) {
	// TODO Auto-generated constructor stub
	this->in_w=in_w;
	this->in_h=in_h;
	this->out_w=out_w;
	this->out_h=out_h;
	this->framesize=0;
	this->out_file=out_file;
	this->basicsize=in_w*in_h;
	this->frameNum=0;
}

EncodeSaver::~EncodeSaver() {
	// TODO Auto-generated destructor stub
}
int EncodeSaver::initDevice()
{
	av_register_all();
	//方法1.组合使用几个函数
	pFormatCtx = avformat_alloc_context();
	//猜格式
	fmt = av_guess_format(NULL, out_file, NULL);
	pFormatCtx->oformat = fmt;
	if (avio_open(&pFormatCtx->pb,out_file, AVIO_FLAG_READ_WRITE) < 0)
	{
		printf("输出文件打开失败");
		return -1;
	}

	video_st = av_new_stream(pFormatCtx, 0);
	if (video_st==NULL)
	{
		return -1;
	}
	pCodecCtx = video_st->codec;
	pCodecCtx->codec_id = fmt->video_codec;
	pCodecCtx->codec_type = AVMEDIA_TYPE_VIDEO;
	pCodecCtx->pix_fmt = PIX_FMT_YUV420P;
	pCodecCtx->width = in_w;
	pCodecCtx->height = in_h;
	pCodecCtx->time_base.num = 1;
	pCodecCtx->time_base.den = 25;
	pCodecCtx->bit_rate = 400000;
	pCodecCtx->gop_size=10;
	//H264
	pCodecCtx->me_range = 16;
	pCodecCtx->max_qdiff = 4;
	pCodecCtx->qmin = 10;
	pCodecCtx->qmax = 51;
	pCodecCtx->qcompress = 0.6;
	if (pCodecCtx->codec_id == CODEC_ID_MPEG2VIDEO)
	{
		pCodecCtx->max_b_frames = 2;
	}
	if (pCodecCtx->codec_id == CODEC_ID_MPEG1VIDEO)
	{
		pCodecCtx->mb_decision = 2;
	}
	if (!strcmp(pFormatCtx->oformat->name, "mp4") || !strcmp(pFormatCtx->oformat->name, "mov") || !strcmp(pFormatCtx->oformat->name, "3gp"))
	{
		pCodecCtx->flags |= CODEC_FLAG_GLOBAL_HEADER;
	}
	//输出格式信息
	av_dump_format(pFormatCtx, 0, out_file, 1);

	pCodec = avcodec_find_encoder(pCodecCtx->codec_id);
	if (!pCodec)
	{
		printf("没有找到合适的编码器！\n");
		return -1;
	}
	if (avcodec_open2(pCodecCtx, pCodec,NULL) < 0)
	{
		printf("编码器打开失败！\n");
		return -1;
	}
	//输出420

	picture = avcodec_alloc_frame();
	framesize = avpicture_get_size(PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height);
	picture_buf = (uint8_t *)av_malloc(framesize);
	avpicture_fill((AVPicture *)picture, picture_buf, PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height);
	//输入422
	picture422 = avcodec_alloc_frame();
	framesize422 = avpicture_get_size(PIX_FMT_YUYV422, pCodecCtx->width, pCodecCtx->height);
	picture422_buf = (uint8_t *)av_malloc(framesize422);
	avpicture_fill((AVPicture *)picture422, picture422_buf, PIX_FMT_YUYV422, pCodecCtx->width, pCodecCtx->height);
	img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, PIX_FMT_YUYV422, pCodecCtx->width, pCodecCtx->height, PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);
	//写文件头
	avformat_write_header(pFormatCtx,NULL);
	av_new_packet(&pkt,basicsize*3);
}
int EncodeSaver::closeDevice()
{
	int ret = flush_encoder(pFormatCtx,0);
	if (ret < 0) {
		printf("Flushing encoder failed\n");
		return -1;
	}

	//写文件尾
	av_write_trailer(pFormatCtx);

	//清理
	if (video_st)
	{
		avcodec_close(video_st->codec);
		av_free(picture);
		av_free(picture_buf);
		av_free(picture422);
		av_free(picture422_buf);
	}
	avio_close(pFormatCtx->pb);
	avformat_free_context(pFormatCtx);
	return 0;
}
int EncodeSaver::doEncode(unsigned char *buf)
{
	int ret;
	//420P
	memcpy(picture_buf,buf,basicsize*3/2);
	picture->data[0] = picture_buf;  // 亮度Y
	picture->data[1] = picture_buf+ basicsize;  // U
	picture->data[2] = picture_buf+ basicsize*5/4; // V


	//PTS
	if (pFormatCtx->oformat->flags & AVFMT_RAWPICTURE)
	{
		AVPacket pkt;
		av_init_packet(&pkt);
		pkt.flags |= AV_PKT_FLAG_KEY;
		pkt.stream_index = video_st->index;
		pkt.data = (uint8_t*)picture;
		pkt.size = sizeof(AVPicture);
		ret = av_write_frame(pFormatCtx, &pkt);
	}
	else{
		int out_size = avcodec_encode_video(pCodecCtx, picture_buf, framesize, picture);
		if (out_size > 0)
		{
			AVPacket pkt;
			av_init_packet(&pkt);
			pkt.pts = av_rescale_q(pCodecCtx->coded_frame->pts, pCodecCtx->time_base, video_st->time_base);
			if (pCodecCtx->coded_frame->key_frame)
			{
				pkt.flags |= AV_PKT_FLAG_KEY;
			}
			pkt.stream_index = video_st->index;
			pkt.data = picture_buf;
			pkt.size = out_size;
			ret = av_write_frame(pFormatCtx, &pkt);
		}
	}
	return 0;
}
int EncodeSaver::doEncode2(unsigned char *buf)
{
	int ret;
	//422
	memcpy(picture422_buf,buf,basicsize*2);
	picture422->data[0] = picture422_buf;
	sws_scale(img_convert_ctx, picture422->data, picture422->linesize, 0, pCodecCtx->height, picture->data, picture->linesize);


	//PTS
	if (pFormatCtx->oformat->flags & AVFMT_RAWPICTURE)
	{
		AVPacket pkt;
		av_init_packet(&pkt);
		pkt.flags |= AV_PKT_FLAG_KEY;
		pkt.stream_index = video_st->index;
		pkt.data = (uint8_t*)picture;
		pkt.size = sizeof(AVPicture);
		ret = av_write_frame(pFormatCtx, &pkt);
	}
	else{
		int out_size = avcodec_encode_video(pCodecCtx, picture_buf, framesize, picture);
		if (out_size > 0)
		{
			AVPacket pkt;
			av_init_packet(&pkt);
			pkt.pts = av_rescale_q(pCodecCtx->coded_frame->pts, pCodecCtx->time_base, video_st->time_base);
			if (pCodecCtx->coded_frame->key_frame)
			{
				pkt.flags |= AV_PKT_FLAG_KEY;
			}
			pkt.stream_index = video_st->index;
			pkt.data = picture_buf;
			pkt.size = out_size;
			ret = av_write_frame(pFormatCtx, &pkt);
		}
	}

	return ret;
}
int EncodeSaver::flush_encoder(AVFormatContext *fmt_ctx,unsigned int stream_index)
{
	int ret;
	int got_frame;
	AVPacket enc_pkt;
	if (!(fmt_ctx->streams[stream_index]->codec->codec->capabilities &
		CODEC_CAP_DELAY))
		return 0;
	while (1) {
		printf("Flushing stream #%u encoder\n", stream_index);
		//ret = encode_write_frame(NULL, stream_index, &got_frame);
		enc_pkt.data = NULL;
		enc_pkt.size = 0;
		av_init_packet(&enc_pkt);
		ret = avcodec_encode_video2 (fmt_ctx->streams[stream_index]->codec, &enc_pkt,
			NULL, &got_frame);
		av_frame_free(NULL);
		if (ret < 0)
			break;
		if (!got_frame)
		{ret=0;break;}
		printf("successed frame!\n");
		/* mux encoded frame */
		ret = av_write_frame(fmt_ctx, &enc_pkt);
		if (ret < 0)
			break;
	}
	return ret;
}
