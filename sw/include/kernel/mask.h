#include<memory_allocator.h>
#include<general_settings.h>
#include<xil_32.h>
#include<xtimer.h>

#define MASK_ACCEL_BASE_ADDRESS     0xA0030000

#define XMASK_ACCEL_CONTROL_ADDR_AP_CTRL       0x00
#define XMASK_ACCEL_CONTROL_ADDR_GIE           0x04
#define XMASK_ACCEL_CONTROL_ADDR_IER           0x08
#define XMASK_ACCEL_CONTROL_ADDR_ISR           0x0c
#define XMASK_ACCEL_CONTROL_ADDR_IMG_INP_DATA  0x10
#define XMASK_ACCEL_CONTROL_BITS_IMG_INP_DATA  64
#define XMASK_ACCEL_CONTROL_ADDR_IMG_MASK_DATA 0x1c
#define XMASK_ACCEL_CONTROL_BITS_IMG_MASK_DATA 64
#define XMASK_ACCEL_CONTROL_ADDR_IMG_OUT_DATA  0x28
#define XMASK_ACCEL_CONTROL_BITS_IMG_OUT_DATA  64
#define XMASK_ACCEL_CONTROL_ADDR_ROWS_DATA     0x34
#define XMASK_ACCEL_CONTROL_BITS_ROWS_DATA     32
#define XMASK_ACCEL_CONTROL_ADDR_COLS_DATA     0x3c
#define XMASK_ACCEL_CONTROL_BITS_COLS_DATA     32


u32 XMask_accel_IsDone(u64 InstancePtr);
void mask_kernel_set(u64 InstancePtr, addr_t input_img, addr_t output_img, addr_t mask_img, u32 rows, u32 cols);
void mask_kernel_run(u64 InstancePtr);
void mask_kernel_run_async(u64 InstancePtr);
void mask_kernel_run_wait(u64 InstancePtr);