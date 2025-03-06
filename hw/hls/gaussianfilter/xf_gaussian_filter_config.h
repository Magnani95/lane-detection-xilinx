/*
 * Copyright 2019 Xilinx, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _XF_GAUSSIAN_FILTER_CONFIG_H_
#define _XF_GAUSSIAN_FILTER_CONFIG_H_

#include "hls_stream.h"
#include "common/xf_common.hpp"
#include "common/xf_utility.hpp"
#include "imgproc/xf_gaussian_filter.hpp"


//---	#include "xf_config_params.h"
#define FILTER_SIZE_3 1
#define FILTER_SIZE_5 0
#define FILTER_SIZE_7 0

#define NO 0
#define RO 1

#define GRAY 0
#define RGBA 1

//---
typedef unsigned short int uint16_t;

#define WIDTH 1920
#define HEIGHT 1080

#if GRAY
#define TYPE XF_8UC1
#define CH_TYPE XF_GRAY
#else
#define TYPE XF_8UC3
#define CH_TYPE XF_RGB
#endif

#if FILTER_SIZE_3
#define FILTER_WIDTH 3
#define FILTER 3
#elif FILTER_SIZE_5
#define FILTER_WIDTH 5
#define FILTER 5
#elif FILTER_SIZE_7
#define FILTER_WIDTH 7
#define FILTER 7
#endif

#if NO
#define NPC1 XF_NPPC1
#endif
#if RO
#define NPC1 XF_NPPC8
#endif

#define INPUT_PTR_WIDTH 128
#define OUTPUT_PTR_WIDTH 128

void gaussian_filter_accel(
    ap_uint<INPUT_PTR_WIDTH>* img_inp, ap_uint<OUTPUT_PTR_WIDTH>* img_out,
	hls::stream<ap_uint<32> > & buffer, hls::stream<ap_uint<32> > & done);

#endif //_XF_GAUSSIAN_FILTER_CONFIG_H_
