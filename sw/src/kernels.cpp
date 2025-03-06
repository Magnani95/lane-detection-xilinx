#include<general_settings.h>
#include<kernels.h>
#include<profiling.h>

// #include<images/gaussian_input_bgr.dat.h>
// #include<images/gaussian_output_bgr.dat.h>
// #include<images/cvtcolor_output_bgr.dat.h>
// #include<images/threshold_output_bgr.dat.h>
// #include<images/filter2d_output_bgr.dat.h>
#include<images/mask_720.h>
// #include<images/mask_output.png.dat.h>

#include "../include/LaneDetector.hpp"

//	MACROs ---------------------------------------------------------------------
//------------------------------------------------------------------------------

//---FUNCTIONs	----------------------------------------------------------------
//------------------------------------------------------------------------------

cv::Mat mask_ocv(cv::Mat img_edges) {
  cv::Mat output;
  cv::Mat mask = cv::Mat::zeros(img_edges.size(), img_edges.type());


  cv::Point pts[4] = {
	  cv::Point(210, 720),
	  cv::Point(550, 450),
	  cv::Point(717, 450),
	  cv::Point(1280, 720)
  };

  // Create a binary polygon mask
  cv::fillConvexPoly(mask, pts, 4, cv::Scalar(255, 0, 0));
  // Multiply the edges image and the mask to get the output
  cv::bitwise_and(img_edges, mask, output);

  return output;

}

void kernel_video_mixed(cv::VideoCapture cap, addr_t xtimer)
{
	printf("Begin\n");
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
	printf("mage memory\n");
	memory_allocator mem_zero, mem_one, mem_mask;

	memory_allocator_init(&mem_zero, SEQUENCE_A_0,	RGB_LENGHT);
    memory_allocator_init(&mem_one,  SEQUENCE_B_1, RGB_LENGHT);
    memory_allocator_init(&mem_mask,  MASK_ADDRESS, GRAY_LENGHT);

	addr_t zero=  memory_allocator_alloc(& mem_zero , RGB_LENGHT);
    addr_t one = memory_allocator_alloc(& mem_one , RGB_LENGHT);
    addr_t mask_img = memory_allocator_alloc(& mem_mask , GRAY_LENGHT);

	//	load first image &  mask
	// for (size_t i=0; i!= RGB_LENGHT; i++ )
	// 	*((unsigned char *) (zero.virt_addr + i)) = __gaussian_input_dat[i];
	
	printf("Load image mask\n");
	for (size_t i=0; i!= GRAY_LENGHT; i++ ){
		*((unsigned char *) (mask_img.virt_addr + i)) = mask_720p_png_dat[i];
	}

	//	set
	printf("kernel Set\n");
 	gaussian_kernel_set(gauss.virt_addr, zero, one, IMG_HEIGHT, IMG_WIDTH);
	cvtcolor_bgr2gray_set(cvt.virt_addr, one, zero, IMG_HEIGHT, IMG_WIDTH);
	threshold_kernel_set(threshold.virt_addr, zero, one, IMG_HEIGHT, IMG_WIDTH);
	filter2d_kernel_set(filter2d.virt_addr, one, zero, IMG_HEIGHT, IMG_WIDTH);
	mask_kernel_set(mask.virt_addr, zero, one, mask_img, IMG_HEIGHT, IMG_WIDTH);

	printf("Mixed begin\n");

	
	LaneDetector lanedetector;  // Create the class object
	cv::Mat frame;
	cv::Mat img_denoise = cv::Mat::zeros(720, 1280, CV_8UC3);
	cv::Mat img_edges;
	cv::Mat img_mask;
	cv::Mat img_lines;
	std::vector<cv::Vec4i> plines;
	std::vector<cv::Vec2f> lines;
	std::vector<std::vector<cv::Vec4i> > left_right_lines;
	std::vector<cv::Point> lane;
	std::string turn;



	// Main algorithm starts. Iterate through every frame of the video
	int i = 1;
	while(true)
	{	
		cv::Mat output = cv::Mat::zeros(720, 1280, CV_8UC1);
		cv::Mat kernel;
		cv::Point anchor;

		anchor = cv::Point(-1, -1);
		kernel = cv::Mat(1, 3, CV_32F);
		kernel.at<float>(0, 0) = -1;
		kernel.at<float>(0, 1) = 0;
		kernel.at<float>(0, 2) = 1;

		// Capture frame
		if (!cap.read(frame)){
			printf("No frame\n");
			break;
		}

		for (size_t i=0; i!=(RGB_LENGHT); i++){
			*((unsigned char*)(zero.virt_addr) +i) = frame.data[i];
		}
		
		// Denoise the image using a Gaussian filter

		// img_denoise=lanedetector.deNoise(frame);
		gaussian_kernel_run(gauss.virt_addr);

		// Detect edges in the image

		// img_edges=lanedetector.edgeDetector(img_denoise);

		// Convert image from RGB to gray
		// cv::cvtColor(img_denoise, output, cv::COLOR_BGR2GRAY);
		cvtcolor_bgr2gray_run(cvt.virt_addr);

		
		// Binarize gray image
	
		// cv::threshold(output, output, 140, 255, cv::THRESH_BINARY);
	
		threshold_kernel_run(threshold.virt_addr);
		
		// Create the kernel [-1 0 1]
		// This kernel is based on the one found in the
		// Lane Departure Warning System by Mathworks

		// Filter the binary image to obtain the edges
		// cv::filter2D(output, output, -1, kernel, anchor, 0, cv::BORDER_DEFAULT);
		filter2d_kernel_run(filter2d.virt_addr);

	

		// Mask the image so that we only get the ROI
		// output=lanedetector.mask(output);

		mask_kernel_run(mask.virt_addr);

		unsigned char * source = (unsigned char *) (one.virt_addr);
		for (size_t i=0; i!=GRAY_LENGHT; i++){
			output.data[i] = source[i];
		}
		cv::imshow("Lane", output);
		cv::waitKey(1);

		// Obtain Hough lines in the cropped image
		plines=lanedetector.houghLinesP(img_mask);
		//CLK_MEASURE(lines=lanedetector.houghLines(img_mask), start, end)
		
		if (!plines.empty()) {
			// Separate lines into left and right lines
			left_right_lines=lanedetector.lineSeparation(plines, img_edges);
			// Apply regression to obtain only one line for each side of the lane
			lane=lanedetector.regression(left_right_lines, frame);
			// Predict the turn by determining the vanishing point of the the lines
			turn=lanedetector.predictTurn();
			// Plot lane detection
			// lanedetector.plotLane(frame, lane, turn);
			// cv::waitKey(1);
			printf("normal branch\n");
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
//---


/*
	LaneDetector lanedetector; 
	cv::Mat frame;
	cv::Mat img_denoise;
	cv::Mat img_edges;
	cv::Mat img_mask;
	cv::Mat img_lines;
	cv::Mat img_translation = cv::Mat::zeros(720, 1280, CV_8UC1); 
	//img_translation_mask =  = cv::Mat::zeros(720, 1280, CV_8UC1); 
	std::vector<cv::Vec4i> plines;
	std::vector<std::vector<cv::Vec4i> > left_right_lines;
	std::vector<cv::Point> lane;
	std::string turn;

	// Main algorithm starts. Iterate through every frame of the video
	while(true)
	{	printf("Begin while loop\n");
		// Capture frame
		if(!cap.read(frame)){
			printf("No frame");
			break;
		}else{
			for (size_t i=0; i!=(RGB_LENGHT); i++)
				*((unsigned char*)(zero.virt_addr) +i) = frame.data[i];
		}
		// Denoise the image using a Gaussian filter
		img_denoise=lanedetector.deNoise(frame);
		// printf("Gauss\n");
		// gaussian_kernel_run(gauss.virt_addr);
  cv::Mat output;
  cv::Mat kernel;
  cv::Point anchor;
		  // Convert image from RGB to gray
    cv::cvtColor(img_denoise, output, cv::COLOR_BGR2GRAY);
  // Binarize gray image
    cv::threshold(output, output, 140, 255, cv::THRESH_BINARY);

		//img_edges=lanedetector.edgeDetector(img_denoise);
		// printf("bgr2gray\n");
		// cvtcolor_bgr2gray_run(cvt.virt_addr);
		// printf("threshold\n");
		// threshold_kernel_run(threshold.virt_addr);

  anchor = cv::Point(-1, -1);
  kernel = cv::Mat(1, 3, CV_32F);
  kernel.at<float>(0, 0) = -1;
  kernel.at<float>(0, 1) = 0;
  kernel.at<float>(0, 2) = 1;

  // Filter the binary image to obtain the edges
  cv::filter2D(output, output, -1, kernel, anchor, 0, cv::BORDER_DEFAULT);

		// printf("filter2d\n");
		// filter2d_kernel_run(filter2d.virt_addr);
		
		// Mask the image so that we only get the ROI
		img_mask=lanedetector.mask(img_edges);
		// printf("Mask\n");
		// mask_kernel_run(mask.virt_addr);

		//Conversion from fpga to cv::mat
		// printf("Translation\n");
		//unsigned char *source = (unsigned char *) one.virt_addr;
		
		// for (size_t i=0; i!=GRAY_LENGHT; i++){
		// 	img_translation.data[i] = source[i];
		// }

		// Obtain Hough lines in the cropped image
		// printf("HoughP\n");
		// plines=lanedetector.houghLinesP(img_translation);
		plines=lanedetector.houghLinesP(output);
		
		if (!plines.empty()){
			// Separate lines into left and right lines
			left_right_lines=lanedetector.lineSeparation(plines, img_edges);

			// Apply regression to obtain only one line for each side of the lane
			lane=lanedetector.regression(left_right_lines, frame);

			// Predict the turn by determining the vanishing point of the the lines
			turn=lanedetector.predictTurn();

			// Plot lane detection
			lanedetector.plotLane(frame, lane, turn);

			cv::waitKey(25);
			printf("Proper branch\n");
		}else{
			printf("else branch\n");
		}
	}
*/