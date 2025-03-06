#ifndef _XF_MASK_ACCEL_CONFIG_H_
#define _XF_MASK_ACCEL_CONFIG_H_

#include "hls_stream.h"
#include "ap_int.h"

#include "common/xf_common.hpp"
#include "common/xf_utility.hpp"

#include "core/xf_arithm.hpp"

typedef ap_uint<8> ap_uint8_t;
typedef ap_uint<64> ap_uint64_t;
//-----------------------------
#define NO 0
#define RO 1

//-----------------------------
/*  set the height and weight  */
#define HEIGHT 1080
#define WIDTH 1920

#if RO
	#define NPIX XF_NPPC8
#endif
#if NO
	#define NPIX XF_NPPC1
#endif

#define INPUT_PTR_WIDTH 128
#define OUTPUT_PTR_WIDTH 128

void mask_accel(ap_uint<INPUT_PTR_WIDTH>* img_inp, ap_uint<INPUT_PTR_WIDTH>* img_mask, ap_uint<OUTPUT_PTR_WIDTH>* img_out,
				hls::stream<ap_uint<32> > & buffer, hls::stream<ap_uint<32> > & done);

#endif // end of _XF_<prj>_CONFIG_H_
