#include<memory_allocator.h>
#include<general_settings.h>
#include<xil_32.h>
#include<xtimer.h>

#define XGAUSSIAN_BLUR_BASE_ADDRESS     0xA0040000

#define XGAUSSIAN_FILTER_ACCEL_CONTROL_ADDR_AP_CTRL      0x00
#define XGAUSSIAN_FILTER_ACCEL_CONTROL_ADDR_GIE          0x04
#define XGAUSSIAN_FILTER_ACCEL_CONTROL_ADDR_IER          0x08
#define XGAUSSIAN_FILTER_ACCEL_CONTROL_ADDR_ISR          0x0c
#define XGAUSSIAN_FILTER_ACCEL_CONTROL_ADDR_IMG_INP_DATA 0x10
#define XGAUSSIAN_FILTER_ACCEL_CONTROL_BITS_IMG_INP_DATA 64
#define XGAUSSIAN_FILTER_ACCEL_CONTROL_ADDR_IMG_OUT_DATA 0x1c
#define XGAUSSIAN_FILTER_ACCEL_CONTROL_BITS_IMG_OUT_DATA 64
#define XGAUSSIAN_FILTER_ACCEL_CONTROL_ADDR_ROWS_DATA    0x28
#define XGAUSSIAN_FILTER_ACCEL_CONTROL_BITS_ROWS_DATA    32
#define XGAUSSIAN_FILTER_ACCEL_CONTROL_ADDR_COLS_DATA    0x30
#define XGAUSSIAN_FILTER_ACCEL_CONTROL_BITS_COLS_DATA    32
#define XGAUSSIAN_FILTER_ACCEL_CONTROL_ADDR_SIGMA_DATA   0x38
#define XGAUSSIAN_FILTER_ACCEL_CONTROL_BITS_SIGMA_DATA   32



u32 XGaussian_filter_accel_IsDone(u64 InstancePtr) ;
void gaussian_kernel_set(u64 InstancePtr, addr_t input_img, addr_t output_img, u32 rows, u32 cols);
void gaussian_kernel_run(u64 InstancePtr);
void gaussian_kernel_run_async(u64 InstancePtr);
void gaussian_kernel_run_wait(u64 InstancePtr);