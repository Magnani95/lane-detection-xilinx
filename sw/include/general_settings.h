#ifndef GENERAL_SETTINGS 
#define GENERAL_SETTINGS

#include <stdint.h>


// General  --------------------------------------------------------------------
#define IMG_HEIGHT 		720
#define IMG_WIDTH 		1280
#define HEIGHT_SLICE	720
#define N_KERNEL		5

#define RGB_LENGHT	IMG_HEIGHT*IMG_WIDTH*3
#define GRAY_LENGHT	IMG_HEIGHT*IMG_WIDTH

//	definitions, don't touch
#define LOAD_IMAGE		0
#define NO_LOAD_IMAGE	0
//	---

typedef uint8_t   u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
//  ----------------------------------------------------------------------------
//  Main    --------------------------------------------------------------------

#define PROFILING_STD_CHRONO 	1
#define PROFILING_XTIMER		0
//	---
#define VERBOSE 0

#if VERBOSE
	#define PRINTF	printf
#else
	#define PRINTF
#endif
//  ----------------------------------------------------------------------------
// kernels.cpp  ----------------------------------------------------------------
#define MASK_LOAD           1		// it should be useless
//---
#define BRAM_ZERO_FILL		1
#define KERNELCALL_VERBOSE  0
#define OUTPUT_CHECK		0

#if KERNELCALL_VERBOSE
	#define PRINTF	printf
#else
	#define PRINTF
#endif

//------------------------------------------------------------------------------

#endif