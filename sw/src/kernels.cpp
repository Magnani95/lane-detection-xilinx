#include<general_settings.h>
#include<kernels.h>
#include<profiling.h>
#include<images/mask_720_py.h>
#include "../include/LaneDetector.hpp"

//	MACROs ---------------------------------------------------------------------
//------------------------------------------------------------------------------

//---FUNCTIONs	----------------------------------------------------------------
//------------------------------------------------------------------------------

void kernel_video_mixed(cv::VideoCapture cap, addr_t xtimer)
{
	struct memory_allocator gauss_alloc, cvt_alloc, threshold_alloc, filter2d_alloc,
							mask_alloc;

	memory_allocator_init(&gauss_alloc, XGAUSSIAN_BLUR_BASE_ADDRESS, 64*1024);
    memory_allocator_init(&cvt_alloc, XCVTCOLOR_BGR2GRAY_BASE_ADDRESS, 64*1024);
    memory_allocator_init(&threshold_alloc, XTHRESHOLD_ACCEL_BASE_ADDRESS, 64*1024);
    memory_allocator_init(&filter2d_alloc, XFILTER2D_BASE_ADDRESS, 64*1024);
    memory_allocator_init(&mask_alloc, MASK_ACCEL_BASE_ADDRESS, 64*1024);

	addr_t gauss =  memory_allocator_alloc(&gauss_alloc, 64*1024);
	addr_t cvt=  memory_allocator_alloc(&cvt_alloc, 64*1024);
	addr_t threshold =  memory_allocator_alloc(&threshold_alloc, 64*1024);
	addr_t filter2d =  memory_allocator_alloc(&filter2d_alloc, 64*1024);
	addr_t mask =  memory_allocator_alloc(&mask_alloc, 64*1024);

	
	// Image memory
	memory_allocator mem_zero, mem_one, mem_mask;

	memory_allocator_init(&mem_zero, SEQUENCE_A_0,	RGB_LENGHT);
    memory_allocator_init(&mem_one,  SEQUENCE_B_1, RGB_LENGHT);
    memory_allocator_init(&mem_mask,  MASK_ADDRESS, GRAY_LENGHT);

	addr_t zero=  memory_allocator_alloc(& mem_zero , RGB_LENGHT);
    addr_t one = memory_allocator_alloc(& mem_one , RGB_LENGHT);
    addr_t mask_img = memory_allocator_alloc(& mem_mask , GRAY_LENGHT);

	
	printf("Load image mask\n");
	for (size_t i=0; i!= GRAY_LENGHT; i++ ){
		*((unsigned char *) (mask_img.virt_addr + i)) = mask_py_png_dat[i];
	}

	//	set
	printf("kernel Set\n");
 	gaussian_kernel_set(gauss.virt_addr, zero, one, IMG_HEIGHT, IMG_WIDTH);
	cvtcolor_bgr2gray_set(cvt.virt_addr, one, zero, IMG_HEIGHT, IMG_WIDTH);
	threshold_kernel_set(threshold.virt_addr, zero, one, IMG_HEIGHT, IMG_WIDTH);
	filter2d_kernel_set(filter2d.virt_addr, one, zero, IMG_HEIGHT, IMG_WIDTH);
	mask_kernel_set(mask.virt_addr, zero, one, mask_img, IMG_HEIGHT, IMG_WIDTH);

	LaneDetector lanedetector;  // Create the class object
	cv::Mat frame;
	cv::Mat img_denoise = cv::Mat::zeros(720, 1280, CV_8UC3);
	std::vector<cv::Vec4i> plines;
	std::vector<std::vector<cv::Vec4i> > left_right_lines;
	std::vector<cv::Point> lane;
	std::string turn;



	// Main algorithm starts. Iterate through every frame of the video
	while(true)
	{	
		cv::Mat output = cv::Mat::zeros(720, 1280, CV_8UC1);

		// Capture frame
		if (!cap.read(frame)){
			printf("No frame\n");
			break;
		}

		memcpy((unsigned char*)(zero.virt_addr), frame.data, RGB_LENGHT);

		// Denoise the image using a Gaussian filter
		// img_denoise=lanedetector.deNoise(frame);
		gaussian_kernel_run(gauss.virt_addr);

		// Detect edges in the image
		// Convert image from RGB to gray

		// cv::cvtColor(img_denoise, output, cv::COLOR_BGR2GRAY);
		cvtcolor_bgr2gray_run(cvt.virt_addr);
		
		// Binarize gray image
		// cv::threshold(output, output, 140, 255, cv::THRESH_BINARY);
		threshold_kernel_run(threshold.virt_addr);
		
		// Filter the binary image to obtain the edges
		// cv::filter2D(output, output, -1, kernel, anchor, 0, cv::BORDER_DEFAULT);
		filter2d_kernel_run(filter2d.virt_addr);

		// Mask the image so that we only get the ROI
		// output=lanedetector.mask(output);
		mask_kernel_run(mask.virt_addr);

		memcpy(output.data, (unsigned char *) (one.virt_addr), GRAY_LENGHT);

		// Obtain Hough lines in the cropped image
		plines=lanedetector.houghLinesP(output);
		
		if (!plines.empty()) {
			// Separate lines into left and right lines
			left_right_lines=lanedetector.lineSeparation(plines, output);
			// Apply regression to obtain only one line for each side of the lane
			lane=lanedetector.regression(left_right_lines, frame);
			// Predict the turn by determining the vanishing point of the the lines
			turn=lanedetector.predictTurn();
			// Plot lane detection
			lanedetector.plotLane(frame, lane, turn);
			cv::waitKey(1);
		}else{
			printf("void lines\n");
		}
	}
	
	//	deallocation
	memory_allocator_deinit (&mem_zero);
	memory_allocator_deinit (&mem_one);
	memory_allocator_deinit (&mem_mask);

	memory_allocator_deinit (&gauss_alloc);
	memory_allocator_deinit (&cvt_alloc);
	memory_allocator_deinit (&threshold_alloc);
	memory_allocator_deinit (&filter2d_alloc);
	memory_allocator_deinit (&mask_alloc);
}