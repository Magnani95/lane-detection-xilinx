#include "mask_accel_config.h"


static constexpr int __XF_DEPTH = (HEIGHT * WIDTH * (XF_PIXELWIDTH(XF_8UC1, NPIX)) / 8) / (INPUT_PTR_WIDTH / 8);

void mask_accel(ap_uint<INPUT_PTR_WIDTH>* img_inp,  ap_uint<INPUT_PTR_WIDTH>* img_mask, ap_uint<OUTPUT_PTR_WIDTH>* img_out,
		hls::stream<ap_uint<32> > & buffer, hls::stream<ap_uint<32> > & done)
{
	#pragma HLS INTERFACE m_axi     port=img_inp  offset=slave bundle=gmem1 depth=__XF_DEPTH
	#pragma HLS INTERFACE m_axi     port=img_mask offset=slave bundle=gmem2 depth=__XF_DEPTH
	#pragma HLS INTERFACE m_axi     port=img_out  offset=slave bundle=gmem3 depth=__XF_DEPTH

	#pragma HLS INTERFACE axis port=buffer
	#pragma HLS INTERFACE axis port=done
	#pragma HLS INTERFACE ap_ctrl_none port=return

	int rows = buffer.read(), cols = buffer.read();

	xf::cv::Mat<XF_8UC1, HEIGHT, WIDTH, NPIX> in_mat(rows, cols);
	xf::cv::Mat<XF_8UC1, HEIGHT, WIDTH, NPIX> in_mask(rows, cols);
	xf::cv::Mat<XF_8UC1, HEIGHT, WIDTH, NPIX> out_mat(rows, cols);

	#pragma HLS DATAFLOW

	xf::cv::Array2xfMat<INPUT_PTR_WIDTH, XF_8UC1, HEIGHT, WIDTH, NPIX>(img_inp, in_mat);
	xf::cv::Array2xfMat<INPUT_PTR_WIDTH, XF_8UC1, HEIGHT, WIDTH, NPIX> (img_mask, in_mask);

	/*
	template	<int SRC_T, int ROWS, int COLS, int NPC=1>

	void bitwise_and (
			xf::Mat<int SRC_T, int ROWS, int COLS, int NPC> src1,
			xf::Mat<int SRC_T, int ROWS, int COLS, int NPC> src2,
			xf::Mat<int SRC_T, int ROWS, int COLS, int NPC> dst )
	*/
	xf::cv::bitwise_and<XF_8UC1, HEIGHT, WIDTH, NPIX> (in_mat, in_mask, out_mat);

	xf::cv::xfMat2Array<OUTPUT_PTR_WIDTH, XF_8UC1, HEIGHT, WIDTH, NPIX>(out_mat, img_out);

	done.write(4);

}
