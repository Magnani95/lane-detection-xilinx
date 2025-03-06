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

#include "common/xf_headers.hpp"
#include "xf_gaussian_filter_config.h"

#include<chrono>
#include<iostream>

//#define PROFILING
#if defined PROFILING
	#define CLK_NOW(a) (a)=std::chrono::steady_clock::now();
	#define CLK_INTERVAL(start, end) std::chrono::duration_cast<std::chrono::microseconds>( (end)-(start)).count()
	#define CLK_PRINT(message, start, end) std::cout<<"\n++"<<(message)<<":\t"<<CLK_INTERVAL(start, end);
	#define CLK_MEASURE(command, start, end) {CLK_NOW(start); (command); CLK_NOW(end); CLK_PRINT( (#command), (start), (end))}
#else
	#define CLK_NOW(a)
	#define CLK_INTERVAL(a, b)
	#define CLK_PRINT(a, b, c)
	#define CLK_MEASURE(command, start, end) (command);
#endif


using namespace std;

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: <executable> <input image path>\n");
        return -1;
    }

    printf("HERE\n");
    printf("Dim\t:%u\n", sizeof(ap_int<1024>*));

    cv::Mat in_img, out_img, hls_out, ocv_ref, in_img_gau;
    cv::Mat in_gray, in_gray1, diff;


#if GRAY
    in_img = cv::imread(argv[1], 0); // reading in the color image
#else
    in_img = cv::imread(argv[1], 1); // reading in the color image
#endif
    if (!in_img.data) {
        fprintf(stderr, "Failed to load the image ... !!!\n ");
        return -1;
    }
    cv::imwrite("begin.png", in_img);
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

#if FILTER_WIDTH == 3
    float sigma = 0.5f;
#endif
#if FILTER_WIDTH == 7
    float sigma = 1.16666f;
#endif
#if FILTER_WIDTH == 5
    float sigma = 0.8333f;
#endif

    int height = in_img.rows;
    int width = in_img.cols;
    printf("HERE\n");
    printf("W-H\t:%d - %d\n", in_img.cols, in_img.rows);


    // OpenCV Gaussian filter function

//	cv::GaussianBlur(in_img, ocv_ref, cvSize(FILTER_WIDTH, FILTER_WIDTH), FILTER_WIDTH / 6.0, FILTER_WIDTH / 6.0,
//                   cv::BORDER_CONSTANT);
	cv::GaussianBlur(in_img, ocv_ref, cvSize(3, 3), 0.5, 0.5, cv::BORDER_CONSTANT);
    imwrite("output_ocv.png", ocv_ref);

	// Call the top function
    gaussian_filter_accel((ap_uint<INPUT_PTR_WIDTH>*)in_img.data, (ap_uint<OUTPUT_PTR_WIDTH>*)out_img.data, height,
                          width, sigma);

    // Write output image
    cv::imwrite("hls_out.png", out_img);
    // Compute absolute difference image
    cv::absdiff(ocv_ref, out_img, diff);

    imwrite("error.png", diff); // Save the difference image for debugging purpose

    float err_per;
    xf::cv::analyzeDiff(diff, 0, err_per);

    if (err_per > 0.0f) {
    	printf("\nERROR [%f]\n",err_per );
        return 0;
    }

    return 0;
}
