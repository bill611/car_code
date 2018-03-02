/* Misc.h
 *
 *
 * Copyright (c)2008 Nuvoton technology corporation
 * http://www.nuvoton.com
 *
 * Misc function
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */

#ifndef __MISC_H__
#define __MISC_H__

#define DEBUG_PRINT(x, ...) fprintf(stdout, x, ##__VA_ARGS__)
//#define DEBUG_PRINT(x, ...)

#define ERR_PRINT(x, ...) fprintf(stderr, x, ##__VA_ARGS__)

#ifndef BOOL
#define BOOL int32_t
#define FALSE 0
#define TRUE 1
#endif

typedef enum
{
	eIMAGE_QVGA,
	eIMAGE_WQVGA,
	eIMAGE_VGA,
	eIMAGE_SVGA,
	eIMAGE_HD720
}E_IMAGE_RESOL;
	
#ifdef int64_t
#undef int64_t
#endif

typedef long long int64_t;
#define rc_MAX_QUANT  52
#define rc_MIN_QUANT  0

typedef struct
{
	int rtn_quant;
	//long long frames;
	int64_t frames;
	//long long total_size;
	double total_size;
	double framerate;
	int target_rate;
	short max_quant;
	short min_quant;
	//long long last_change;
	//long long quant_sum;
	int64_t last_change;
	int64_t quant_sum;
//	double quant_error[32];
	double quant_error[rc_MAX_QUANT];
	double avg_framesize;
	double target_framesize;
	double sequence_quality;
	int averaging_period;
	int reaction_delay_factor;
	int buffer;
	unsigned int IPInterval;
	unsigned int IPIntervalCnt;
	int pre_rtn_quant;
}H264RateControl;

//#define RC_DELAY_FACTOR         16
#define RC_DELAY_FACTOR         4
#define RC_AVERAGING_PERIOD     100
#define RC_BUFFER_SIZE_QUALITY  100 //quality sensitive, recommended
#define RC_BUFFER_SIZE_BITRATE  10  //bit rate sensitive, not recommended

#define quality_const (double)((double)2/(double)rc_MAX_QUANT)

//void H264RateControlInit(H264RateControl *rate_control,
//				unsigned int target_rate,
//				unsigned int reaction_delay_factor,
//				unsigned int averaging_period,
//				unsigned int buffer,
//				float framerate,
//				int max_quant,
//				int min_quant,
//				unsigned int initq,
//				unsigned int IPInterval);
//				
//void H264RateControlUpdate(H264RateControl *rate_control,
//				  short quant,
//				  int frame_size,
//				  int keyframe);

#endif

