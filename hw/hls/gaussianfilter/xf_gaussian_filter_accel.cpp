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

#include "xf_gaussian_filter_config.h"

static constexpr int __XF_DEPTH = (HEIGHT * WIDTH * (XF_PIXELWIDTH(TYPE, NPC1)) / 8) / (INPUT_PTR_WIDTH / 8);
//static constexpr int __XF_DEPTH =  192;

void gaussian_filter_accel(
    ap_uint<INPUT_PTR_WIDTH>* img_inp, ap_uint<OUTPUT_PTR_WIDTH>* img_out,
	hls::stream<ap_uint<32> > & buffer, hls::stream<ap_uint<32> > & done) {
// clang-format off
    #pragma HLS INTERFACE m_axi     port=img_inp  offset=slave bundle=gmem1 depth=__XF_DEPTH
    #pragma HLS INTERFACE m_axi     port=img_out  offset=slave bundle=gmem2 depth=__XF_DEPTH

//	#pragma HLS INTERFACE axis port=sigma_p
//	#pragma HLS INTERFACE axis port=rows_p
//	#pragma HLS INTERFACE axis port=cols_p
	#pragma HLS INTERFACE axis port=done
	#pragma HLS INTERFACE axis port=buffer
	#pragma HLS INTERFACE ap_ctrl_none port=return

//    #pragma HLS INTERFACE s_axilite port=sigma
//    #pragma HLS INTERFACE s_axilite port=rows
//    #pragma HLS INTERFACE s_axilite port=cols
//    #pragma HLS INTERFACE s_axilite port=return


	int rows = buffer.read(), cols = buffer.read();
	float sigma = buffer.read();

    xf::cv::Mat<TYPE, HEIGHT, WIDTH, NPC1> in_mat(rows, cols);
    xf::cv::Mat<TYPE, HEIGHT, WIDTH, NPC1> out_mat(rows, cols);

    #pragma HLS DATAFLOW

    xf::cv::Array2xfMat<INPUT_PTR_WIDTH, TYPE, HEIGHT, WIDTH, NPC1>(img_inp, in_mat);

    xf::cv::GaussianBlur<FILTER_WIDTH, XF_BORDER_CONSTANT, TYPE, HEIGHT, WIDTH, NPC1>(in_mat, out_mat, sigma);

    xf::cv::xfMat2Array<OUTPUT_PTR_WIDTH, TYPE, HEIGHT, WIDTH, NPC1>(out_mat, img_out);

    done.write(0);
}
