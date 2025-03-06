#include "common/xf_headers.hpp"
#include "filter2D_accel_config.h"

using namespace std;

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: <executable> <input image path>\n");
        return -1;
    }

    cv::Mat in_img, out_img, hls_out, ocv_ref, in_img_gau;
    cv::Mat in_gray, in_gray1, diff;

#if GRAY
    in_img = cv::imread(argv[1], 0); // reading in the gray image
#else
    in_img = cv::imread(argv[1], 1); // reading in the color image
#endif
    if (!in_img.data) {
        fprintf(stderr, "Failed to load the image ... !!!\n ");
        return -1;
    }
// extractChannel(in_img, in_img, 1);
#if GRAY

    out_img.create(in_img.rows, in_img.cols, CV_8UC1); // create memory for output image
    diff.create(in_img.rows, in_img.cols, CV_8UC1);    // create memory for OCV-ref image
    ocv_ref.create(in_img.rows, in_img.cols, CV_8UC1); // create memory for OCV-ref image

#else
    out_img.create(in_img.rows, in_img.cols, CV_8UC3); // create memory for output image
    diff.create(in_img.rows, in_img.cols, CV_8UC3);    // create memory for OCV-ref image
    ocv_ref.create(in_img.rows, in_img.cols, CV_8UC3); // create memory for OCV-ref image
#endif

    int height = in_img.rows;
    int width = in_img.cols;

    // OpenCV Gaussian filter function
    cv::Mat kernel;
    cv::Point anchor;

	anchor = cv::Point(-1, -1);
	kernel = cv::Mat(1, 3, CV_32F);
	kernel.at<float>(0, 0) = -1;
	kernel.at<float>(0, 1) = 0;
	kernel.at<float>(0, 2) = 1;

    cv::filter2D(in_img, ocv_ref, -1, kernel, anchor, 0, cv::BORDER_DEFAULT);

    imwrite("output_ocv.png", ocv_ref);

    // Call the top function
    //filter2D_accelerated(img_inp, img_out, rows, cols);
    filter2D_accelerated((ap_uint<INPUT_PTR_WIDTH>*)in_img.data, (ap_uint<OUTPUT_PTR_WIDTH>*)out_img.data,
    		height,width);

    // Write output image
    cv::imwrite("hls_out.png", out_img);

    // Compute absolute difference image
    cv::absdiff(ocv_ref, out_img, diff);

    imwrite("error.png", diff); // Save the difference image for debugging purpose

    return 0;
}
