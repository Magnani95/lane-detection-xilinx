#include<memory_allocator.h>
#include<general_settings.h>
#include<xil_32.h>
#include<xtimer.h>

#define XCVTCOLOR_BGR2GRAY_BASE_ADDRESS 0xA0010000

#define XCVTCOLOR_BGR2GRAY_CONTROL_ADDR_AP_CTRL        0x00
#define XCVTCOLOR_BGR2GRAY_CONTROL_ADDR_GIE            0x04
#define XCVTCOLOR_BGR2GRAY_CONTROL_ADDR_IER            0x08
#define XCVTCOLOR_BGR2GRAY_CONTROL_ADDR_ISR            0x0c
#define XCVTCOLOR_BGR2GRAY_CONTROL_ADDR_IMGINPUT_DATA  0x10
#define XCVTCOLOR_BGR2GRAY_CONTROL_BITS_IMGINPUT_DATA  64
#define XCVTCOLOR_BGR2GRAY_CONTROL_ADDR_IMGOUTPUT_DATA 0x1c
#define XCVTCOLOR_BGR2GRAY_CONTROL_BITS_IMGOUTPUT_DATA 64
#define XCVTCOLOR_BGR2GRAY_CONTROL_ADDR_ROWS_DATA      0x28
#define XCVTCOLOR_BGR2GRAY_CONTROL_BITS_ROWS_DATA      32
#define XCVTCOLOR_BGR2GRAY_CONTROL_ADDR_COLS_DATA      0x30
#define XCVTCOLOR_BGR2GRAY_CONTROL_BITS_COLS_DATA      32
u32 XCvtcolor_bgr2gray_IsDone(u64 InstancePtr);

void cvtcolor_bgr2gray_set(u64 InstancePtr,addr_t input_img, addr_t output_img, u32 rows, u32 cols);
void cvtcolor_bgr2gray_run(u64 InstancePtr);
void cvtcolor_bgr2gray_run_async(u64 InstancePtr);
void cvtcolor_bgr2gray_run_wait(u64 InstancePtr);