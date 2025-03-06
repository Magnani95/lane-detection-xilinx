#ifndef KERNELS_H
#define KERNELS_H

#include<memory_allocator.h>
#include<memory_address.h>
#include<profiling.h>
#include<xtimer.h>
#include<utils.h>

#include<kernel/guassian_blur.h>
#include<kernel/cvtcolor_bgrgray.h>
#include<kernel/threshold.h>
#include<kernel/filter2d.h>
#include<kernel/mask.h>

#include "opencv2/opencv.hpp"

void kernel_sequence(addr_t xtimer);
void kernel_pipeline(addr_t xtimer);
void kernel_pipeline_parallel(addr_t xtimer);
void kernel_pipeline_parallel_optimized(addr_t xtimer);
void kernel_video_mixed(cv::VideoCapture cap, addr_t xtimer);


void kernel_gauss(addr_t xtimer, bool load_img);
void kernel_cvtcolor_bgr2gray(addr_t xtimer, bool load_img);
void kernel_threshold(addr_t xtimer, bool load_img);
void kernel_filter2d(addr_t xtimer, bool load_img);
void kernel_mask(addr_t xtimer, bool load_img);

#endif