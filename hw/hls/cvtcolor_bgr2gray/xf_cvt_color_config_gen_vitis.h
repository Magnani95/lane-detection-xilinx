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

#ifndef _XF_CVT_COLOR_CONFIG_GEN_VITIS_H_
	#define _XF_CVT_COLOR_CONFIG_GEN_VITIS_H_
#endif

#include "hls_stream.h"
#include "ap_int.h"
//#include "xf_config_params.h"
#include "common/xf_common.hpp"
#include "common/xf_utility.hpp"
#include "imgproc/xf_cvt_color.hpp"
#include "imgproc/xf_cvt_color_1.hpp"
//#include "imgproc/xf_rgb2hsv.hpp"
//#include "imgproc/xf_bgr2hsv.hpp"
// Has to be set when synthesizing
#define _XF_SYNTHESIS_ 1

#define NO 1
#define RO 0

// Image Dimensions
static constexpr int WIDTH = 1920;
static constexpr int HEIGHT = 1080;

#if NO
static constexpr int NPC = XF_NPPC1;
#elif RO
static constexpr int NPC = XF_NPPC8;
#endif

#define INPUT_PTR_WIDTH		32*NPC
#define OUTPUT_PRT_WIDTH	8*NPC

void cvtcolor_bgr2gray(ap_uint<INPUT_PTR_WIDTH>* imgInput, ap_uint<OUTPUT_PRT_WIDTH>* imgOutput,
						hls::stream<ap_uint<32> > & buffer, hls::stream<ap_uint<32> > & done);
