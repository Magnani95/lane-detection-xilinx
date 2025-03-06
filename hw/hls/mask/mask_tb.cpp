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
#include "mask_accel_config.h"

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

    cv::Mat in_img, output_cv, output_vitis, diff;

    in_img = cv::imread(argv[1], 0); // reading in the gray image
    if (!in_img.data) {
        fprintf(stderr, "Failed to load the image ... !!!\n ");
        return -1;
    }
    output_cv.create(in_img.rows, in_img.cols, CV_8UC1);
    output_vitis.create(in_img.rows, in_img.cols, CV_8UC1);
    int height = in_img.rows;
    int width = in_img.cols;
    printf("HERE\n");
    printf("W-H\t:%d - %d\n", in_img.cols, in_img.rows);

    // OpenCV
    cv::Mat mask = cv::Mat::zeros(in_img.size(), in_img.type());

    cv::Point pts[4] = {
  	  cv::Point(0, 128),
  	  cv::Point(28, 20*2),
  	  cv::Point(100, 20*2),
  	  cv::Point(128, 128)
    };
//        cv::Point pts[4] = {
//      	  cv::Point(0, 1080),
//      	  cv::Point(480, 400),
//      	  cv::Point(1440, 400),
//      	  cv::Point(1920, 1080)
//        };

    // Create a binary polygon mask
    cv::fillConvexPoly(mask, pts, 4, cv::Scalar(255, 0, 0));
    cv::imwrite("mask.png", mask);

    // Multiply the edges image and the mask to get the output
    cv::bitwise_and(in_img, mask, output_cv);
    //	---

    // Vitis
    mask_accel( (ap_uint<INPUT_PTR_WIDTH>*) in_img.data,  (ap_uint<INPUT_PTR_WIDTH>*) mask.data, (ap_uint<OUTPUT_PTR_WIDTH>*) output_vitis.data, in_img.rows, in_img.cols);
//    mask_accel(	(ap_uint<INPUT_PTR_WIDTH>*) in_img.data,
//				(ap_uint<OUTPUT_PTR_WIDTH>*) mask.data,
//				(ap_uint<INPUT_PTR_WIDTH>*) output_vitis.data
//		);
    //	---


    // Write output image
    cv::imwrite("out_cv.png", output_cv);
    cv::imwrite("out_vitis.png", output_vitis);

    cv::absdiff(output_cv, output_vitis, diff);
    imwrite("error.png", diff);


    return 0;
}
