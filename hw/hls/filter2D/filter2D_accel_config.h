#ifndef _XF_FILTER2D_CONFIG_H_
#define _XF_FILTER2D_CONFIG_H_

#include "hls_stream.h"
#include "ap_int.h"

#include "common/xf_common.hpp"
#include "common/xf_utility.hpp"

#include "imgproc/xf_custom_convolution.hpp"

typedef ap_uint<8> ap_uint8_t;
typedef ap_uint<64> ap_uint64_t;
//-----------------------------
#define NO 0
#define RO 1
#define GRAY 1
//-----------------------------
/*  set the height and weight  */
#define WIDTH 128
#define HEIGHT 128

#define FILTER_WIDTH	3
#define FILTER_HEIGHT	1

#if RO
	#define NPIX XF_NPPC8
#endif
#if NO
	#define NPIX XF_NPPC1
#endif

#define INPUT_PTR_WIDTH 128
#define OUTPUT_PTR_WIDTH 128

void filter2D_accelerated(ap_uint<INPUT_PTR_WIDTH>* img_inp,
                     ap_uint<OUTPUT_PTR_WIDTH>* img_out,
					 hls::stream<ap_uint<32> > & buffer,
					 hls::stream<ap_uint<32> > & done);


#endif // end of _XF_THRESHOLD_CONFIG_H_
