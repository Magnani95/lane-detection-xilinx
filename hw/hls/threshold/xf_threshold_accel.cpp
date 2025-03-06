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

#include "xf_threshold_config.h"

static constexpr int __XF_DEPTH = (HEIGHT * WIDTH * (XF_PIXELWIDTH(XF_8UC1, NPIX)) / 8) / (INPUT_PTR_WIDTH / 8);

void threshold_accel(ap_uint<INPUT_PTR_WIDTH>* img_inp,
                     ap_uint<OUTPUT_PTR_WIDTH>* img_out,
					 hls::stream<ap_uint<32> > & buffer,
					 hls::stream<ap_uint<32> > & done)
{

    #pragma HLS INTERFACE m_axi     port=img_inp  offset=slave bundle=gmem1 depth=__XF_DEPTH
    #pragma HLS INTERFACE m_axi     port=img_out  offset=slave bundle=gmem2 depth=__XF_DEPTH

    #pragma HLS INTERFACE axis port=done
	#pragma HLS INTERFACE axis port=buffer
//    #pragma HLS INTERFACE axis port=rows_p
//    #pragma HLS INTERFACE axis port=cols_p
	#pragma HLS INTERFACE ap_ctrl_none port=return

	ap_uint<32> buffer_temp = buffer.read();

	unsigned char thresh = buffer_temp.range(0,7), maxval=buffer_temp.range(8,15);
	int rows = buffer.read(), cols = buffer.read();

    xf::cv::Mat<XF_8UC1, HEIGHT, WIDTH, NPIX> in_mat(rows, cols);
    xf::cv::Mat<XF_8UC1, HEIGHT, WIDTH, NPIX> out_mat(rows, cols);


    #pragma HLS DATAFLOW

    xf::cv::Array2xfMat<INPUT_PTR_WIDTH, XF_8UC1, HEIGHT, WIDTH, NPIX>(img_inp, in_mat);

    xf::cv::Threshold<THRESH_TYPE, XF_8UC1, HEIGHT, WIDTH, NPIX>(in_mat, out_mat, thresh, maxval);

    xf::cv::xfMat2Array<OUTPUT_PTR_WIDTH, XF_8UC1, HEIGHT, WIDTH, NPIX>(out_mat, img_out);

    done.write(2);
}
