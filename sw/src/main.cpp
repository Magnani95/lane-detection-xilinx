#include<general_settings.h>
#include<xil_32.h>
#include<xtimer.h>
#include<utils.h>
#include<memory_address.h>
#include<kernels.h>
#include<profiling.h>

#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include "opencv2/opencv.hpp"

// #include "LaneDetector.cpp"


//	MACRO ----------------------------------------------------------------------
//------------------------------------------------------------------------------

//	FUNCTIONs ------------------------------------------------------------------
//------------------------------------------------------------------------------
void single_kernel_call(addr_t xtimer);
void sequence_kernel_call(addr_t xtimer);

//	MAIN -----------------------------------------------------------------------
//------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	if (argc != 2) {
		std::cout << "Pass one parameter [video]\n";
		return -1;
	}
	
	std::string source = argv[1];
	// The input argument is the location of the video
	
	cv::VideoCapture cap(source, cv::CAP_ANY);
	//cv::VideoCapture cap(0);

	if (!cap.isOpened()){
		printf("cap is not opened\n");
		return -1;
	}

	//	Axi Timer
	struct memory_allocator xtimer_alloc;
    memory_allocator_init(&xtimer_alloc, 0xA0000000, 64*1024);
	addr_t xtimer =  memory_allocator_alloc(&xtimer_alloc, 64*1024);
	
	kernel_video_mixed(cap, xtimer);


	printf("Natural end of the program\n");
	return 0;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------