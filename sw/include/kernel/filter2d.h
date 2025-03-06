#include<memory_allocator.h>
#include<general_settings.h>
#include<xil_32.h>
#include<xtimer.h>

#define XFILTER2D_BASE_ADDRESS      0xA0020000

#define XFILTER2D_ACCELERATED_CONTROL_ADDR_AP_CTRL      0x00
#define XFILTER2D_ACCELERATED_CONTROL_ADDR_GIE          0x04
#define XFILTER2D_ACCELERATED_CONTROL_ADDR_IER          0x08
#define XFILTER2D_ACCELERATED_CONTROL_ADDR_ISR          0x0c
#define XFILTER2D_ACCELERATED_CONTROL_ADDR_IMG_INP_DATA 0x10
#define XFILTER2D_ACCELERATED_CONTROL_BITS_IMG_INP_DATA 64
#define XFILTER2D_ACCELERATED_CONTROL_ADDR_IMG_OUT_DATA 0x1c
#define XFILTER2D_ACCELERATED_CONTROL_BITS_IMG_OUT_DATA 64
#define XFILTER2D_ACCELERATED_CONTROL_ADDR_ROWS_DATA    0x28
#define XFILTER2D_ACCELERATED_CONTROL_BITS_ROWS_DATA    32
#define XFILTER2D_ACCELERATED_CONTROL_ADDR_COLS_DATA    0x30
#define XFILTER2D_ACCELERATED_CONTROL_BITS_COLS_DATA    32

u32 XFilter2d_accelerated_IsDone(u64 InstancePtr) ;
void filter2d_kernel_set(u64 InstancePtr, addr_t input_img, addr_t output_img, u32 rows, u32 cols);
void filter2d_kernel_run(u64 InstancePtr);
void filter2d_kernel_run_async(u64 InstancePtr);
void filter2d_kernel_run_wait(u64 InstancePtr);