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

#include "xf_cvt_color_config_gen_vitis.h"


void cvtcolor_bgr2gray(ap_uint<32*NPC>* imgInput, ap_uint<8*NPC>* imgOutput,
						hls::stream<ap_uint<32> > & buffer, hls::stream<ap_uint<32> > & done) {
//void cvtcolor_bgr2gray(ap_uint<INPUT_PTR_WIDTH>* imgInput, ap_uint<OUTPUT_PRT_WIDTH>* imgOutput) {

    static constexpr int __XF_DEPTH_INP_0 = ((HEIGHT) * (WIDTH) * (XF_PIXELWIDTH(XF_8UC3, NPC))) / (32 * NPC);
    static constexpr int __XF_DEPTH_OUT_0 = ((HEIGHT) * (WIDTH) * (XF_PIXELWIDTH(XF_8UC1, NPC))) / (8 * NPC);

    #pragma HLS INTERFACE m_axi      port=imgInput    offset=slave  bundle=gmem_in0  depth=__XF_DEPTH_INP_0
    #pragma HLS INTERFACE m_axi      port=imgOutput   offset=slave  bundle=gmem_out0 depth=__XF_DEPTH_OUT_0

//    #pragma HLS INTERFACE axis port=rows_p
//	#pragma HLS INTERFACE axis port=cols_p
	#pragma HLS INTERFACE axis port=buffer
	#pragma HLS INTERFACE axis port=done
	#pragma HLS INTERFACE ap_ctrl_none port=return

    int rows=buffer.read(), cols=buffer.read();

    xf::cv::Mat<XF_8UC3, HEIGHT, WIDTH, NPC> _imgInput(rows, cols);
    xf::cv::Mat<XF_8UC1, HEIGHT, WIDTH, NPC> _imgOutput(rows, cols);

    #pragma HLS DATAFLOW

    xf::cv::Array2xfMat<32*NPC, XF_8UC3, HEIGHT, WIDTH, NPC>(imgInput, _imgInput);
//	xf::cv::Array2xfMat<INPUT_PTR_WIDTH, XF_8UC3, HEIGHT, WIDTH, NPC>(imgInput, _imgInput);

    xf::cv::bgr2gray<XF_8UC3, XF_8UC1, HEIGHT, WIDTH, NPC>(_imgInput, _imgOutput);

	xf::cv::xfMat2Array<8*NPC, XF_8UC1, HEIGHT, WIDTH, NPC>(_imgOutput, imgOutput);
//	xf::cv::xfMat2Array<OUTPUT_PRT_WIDTH, XF_8UC1, HEIGHT, WIDTH, NPC>(_imgOutput, imgOutput);

	done.write(1);
}
