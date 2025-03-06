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
#include "xf_cvt_color_config_gen_vitis.h"

using namespace std;

#define ERROR_THRESHOLD 2
#define OCV_CVT 1
#define BYTES_TOLLERANCE 10

#define GRAY_LENGHT	16384


static bool check_ischanged(const unsigned char input_img[],const unsigned char output_img[], size_t lenght);
static void opencv_version_cmp(const unsigned char input_img[],const unsigned char output_img[], size_t lenght);
static void check_bytes(const unsigned char img0[],const unsigned char img1[], size_t lenght);
static void max_byte_count(const unsigned char img0[],const unsigned char ref_img[], size_t lenght);




int main(int argc, char** argv) {
	printf("Begin\n");
    cv::Mat imgInput0, ocv_cvtgray, errImg0;
    imgInput0 = cv::imread(argv[1], 1);
	if (!imgInput0.data) {
		fprintf(stderr, "Can't open image %s !!.\n ", argv[1]);
		return -1;
	}

#if ! OCV_CVT
	refOutput0 = cv::imread(argv[2], 0);
	if (!refOutput0.data) {
		fprintf(stderr, "Can't open image %s !!.\n ", argv[2]);
		return -1;
	}
#endif

	cv::Mat imgOutput0(HEIGHT, WIDTH, CV_8UC1);

	cv::cvtColor(imgInput0, ocv_cvtgray, cv::COLOR_BGR2GRAY);
	cvtcolor_bgr2gray( (ap_uint<INPUT_PTR_WIDTH>*)imgInput0.data, (ap_uint<OUTPUT_PRT_WIDTH>*)imgOutput0.data, imgInput0.rows, imgInput0.cols);

	opencv_version_cmp( (unsigned char *)ocv_cvtgray.data, (unsigned char *)imgOutput0.data, GRAY_LENGHT);

	cv::imwrite("fpga_GRAY.png", imgOutput0);
	cv::imwrite("ocv_GRAY.png", ocv_cvtgray);
	printf("here\n");
	cv::Size S0(WIDTH, HEIGHT);
	errImg0.create(S0, CV_8UC1);

	cv::absdiff(ocv_cvtgray, imgOutput0, errImg0);

	cv::imwrite("err_GRAY.png", errImg0);

    /* ## *************************************************************** ##*/
    return 0;
}


bool check_ischanged(const unsigned char input_img[],const unsigned char output_img[], size_t lenght)
{
	for(size_t i=0; i< lenght; i++){
		if (output_img[i] != input_img[i]){
			return true;
		}
	}
	return false;
}

void check_bytes(const unsigned char img0[],const unsigned char img1[], size_t lenght)
{
	size_t right_bytes=0;
	unsigned char tollerance=BYTES_TOLLERANCE;
	unsigned char lower, upper;
	float percent;
	for (size_t i = 0; i < lenght; ++i) {
		lower= ( img0[i] > tollerance ) ? img0[i]-tollerance : 0;
		upper= ( (255-img0[i]) <= tollerance)  ? 255 : img0[i]+tollerance;
		if ((img1[i] >= lower) && (img1[i] <= upper)) {
			right_bytes++;
		}
	}
	percent = (float) right_bytes / lenght *100;
	printf("|-> with tolerance [%u] (%.2f%%)\t%lu / %lu\n",tollerance, percent, right_bytes,lenght);
}

void opencv_version_cmp(const unsigned char input_img[],const unsigned char output_img[], size_t lenght)
{
	if (check_ischanged(input_img, output_img, lenght)){
		printf("|-> ATTENTION -- Output is different from Opencv version\n");
	}else{
		printf("|-> Output and Opencv version are the same\n");
	}
	check_bytes(input_img, output_img,lenght);
}
