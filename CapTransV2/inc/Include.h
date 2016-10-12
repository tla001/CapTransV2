/*
 * Include.h
 *
 *  Created on: Oct 11, 2016
 *      Author: tla001
 */

#ifndef INCLUDE_H_
#define INCLUDE_H_

extern "C"{
	#ifndef INT64_C
	#define INT64_C(c) (c ## LL)
	#define UINT64_C(c) (c ## ULL)
	#endif

	//basic
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <math.h>

	//ffmpeg
	#include <stdint.h>
	#include <libavutil/common.h>
	#include <libavutil/opt.h>
	#include <libavcodec/avcodec.h>
	#include <libavutil/channel_layout.h>
	#include <libavutil/imgutils.h>
	#include <libavutil/mathematics.h>
	#include <libavutil/samplefmt.h>
	#include "libavutil/avutil.h"
	#include "libavutil/pixdesc.h"
	//#include <libavutil/old_pix_fmts.h>
	#include <libavformat/avformat.h>
	#include <libavfilter/avfilter.h>
	#include <libavfilter/avfiltergraph.h>
	#include "libavfilter/avcodec.h"
	#include "libavfilter/buffersink.h"
	#include "libavfilter/buffersrc.h"
	#include <libswscale/swscale.h>
	#include <libavdevice/avdevice.h>

	#include <SDL/SDL.h>

	//linux
	#include <unistd.h>
	#include <error.h>
	#include <getopt.h>
	#include <fcntl.h>
	#include <sys/ioctl.h>
	#include <linux/types.h>
	#include <linux/spi/spidev.h>
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <linux/i2c-dev.h>
	#include <linux/i2c.h>
	#include <asm-generic/ioctl.h>
	#include <sys/sysinfo.h>
	#include <pthread.h>
	//#include <linux/videodev2.h>
	#include <sys/wait.h>

	#include <sys/mman.h>
	#include <linux/fb.h>
	#include <time.h>
	#include <ipu.h>
	#include <sys/socket.h>
	#include <arpa/inet.h>
	#include <netinet/in.h>
}

#include <iostream>

using namespace std;

#endif /* INCLUDE_H_ */
