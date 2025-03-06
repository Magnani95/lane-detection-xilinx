#include "filter2D_accel_config.h"

static constexpr int __XF_DEPTH = (HEIGHT * WIDTH * (XF_PIXELWIDTH(XF_8UC1, NPIX)) / 8) / (INPUT_PTR_WIDTH / 8);

void filter2D_accelerated(ap_uint<INPUT_PTR_WIDTH>* img_inp, ap_uint<OUTPUT_PTR_WIDTH>* img_out,
		hls::stream<ap_uint<32> > & buffer, hls::stream<ap_uint<32> > & done)
{
	#pragma HLS INTERFACE m_axi     port=img_inp  offset=slave bundle=gmem1 depth=__XF_DEPTH
	#pragma HLS INTERFACE m_axi     port=img_out  offset=slave bundle=gmem2 depth=__XF_DEPTH

	#pragma HLS INTERFACE axis port=buffer
	#pragma HLS INTERFACE axis port=done
	#pragma HLS INTERFACE ap_ctrl_none port=return


	int rows = buffer.read(), cols = buffer.read();

	xf::cv::Mat<XF_8UC1, HEIGHT, WIDTH, NPIX> in_mat(rows, cols);
	xf::cv::Mat<XF_8UC1, HEIGHT, WIDTH, NPIX> out_mat(rows, cols);

	short int filter[3] = { [0]=-1, [1]=0, [2]=1};

	#pragma HLS DATAFLOW
	// clang-format on

	xf::cv::Array2xfMat<INPUT_PTR_WIDTH, XF_8UC1, HEIGHT, WIDTH, NPIX>(img_inp, in_mat);
	/*
	template	<int BORDER_TYPE,
				int FILTER_WIDTH, int FILTER_HEIGHT,
				int SRC_T,int DST_T,
				int ROWS, int COLS,
				int NPC>

	void filter2D	(xf::Mat<SRC_T, ROWS, COLS, NPC> & _src_mat,
					xf::Mat<DST_T, ROWS, COLS, NPC> & _dst_mat,
					short int filter[FILTER_HEIGHT*FILTER_WIDTH],
					unsigned char _shift)
	*/
	xf::cv::filter2D<XF_BORDER_CONSTANT,
					FILTER_WIDTH, FILTER_HEIGHT,
					XF_8UC1, XF_8UC1,
					HEIGHT, WIDTH, NPIX>
					(in_mat, out_mat, filter, 0); //XF_16SP

	xf::cv::xfMat2Array<OUTPUT_PTR_WIDTH, XF_8UC1, HEIGHT, WIDTH, NPIX>(out_mat, img_out);

		done.write(3);
}
