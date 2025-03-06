#include<memory_allocator.h>
#include<general_settings.h>
#include<xil_32.h>
#include<xtimer.h>

#define XTHRESHOLD_ACCEL_BASE_ADDRESS     0xA0060000


#define XTHRESHOLD_ACCEL_CONTROL_ADDR_AP_CTRL      0x00
#define XTHRESHOLD_ACCEL_CONTROL_ADDR_GIE          0x04
#define XTHRESHOLD_ACCEL_CONTROL_ADDR_IER          0x08
#define XTHRESHOLD_ACCEL_CONTROL_ADDR_ISR          0x0c
#define XTHRESHOLD_ACCEL_CONTROL_ADDR_IMG_INP_DATA 0x10
#define XTHRESHOLD_ACCEL_CONTROL_BITS_IMG_INP_DATA 64
#define XTHRESHOLD_ACCEL_CONTROL_ADDR_IMG_OUT_DATA 0x1c
#define XTHRESHOLD_ACCEL_CONTROL_BITS_IMG_OUT_DATA 64
#define XTHRESHOLD_ACCEL_CONTROL_ADDR_THRESH_DATA  0x28
#define XTHRESHOLD_ACCEL_CONTROL_BITS_THRESH_DATA  8
#define XTHRESHOLD_ACCEL_CONTROL_ADDR_MAXVAL_DATA  0x30
#define XTHRESHOLD_ACCEL_CONTROL_BITS_MAXVAL_DATA  8
#define XTHRESHOLD_ACCEL_CONTROL_ADDR_ROWS_DATA    0x38
#define XTHRESHOLD_ACCEL_CONTROL_BITS_ROWS_DATA    32
#define XTHRESHOLD_ACCEL_CONTROL_ADDR_COLS_DATA    0x40
#define XTHRESHOLD_ACCEL_CONTROL_BITS_COLS_DATA    32

u32 XThreshold_accel_IsDone(u64 InstancePtr);
void threshold_kernel_set(u64 InstancePtr, addr_t input_img, addr_t output_img, u32 rows, u32 cols);
void threshold_kernel_run(u64 InstancePtr);
void threshold_kernel_run_async(u64 InstancePtr);
void threshold_kernel_run_wait(u64 InstancePtr);