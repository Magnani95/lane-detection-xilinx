#include<kernel/filter2d.h>

void filter2d_kernel_set(u64 InstancePtr, addr_t input_img, addr_t output_img, u32 rows, u32 cols)
{
    //  set in img
    Xil_Out32(InstancePtr + XFILTER2D_ACCELERATED_CONTROL_ADDR_IMG_INP_DATA, (u64)(input_img.phys_addr));
    Xil_Out32(InstancePtr + XFILTER2D_ACCELERATED_CONTROL_ADDR_IMG_INP_DATA + 4, ((u64)input_img.phys_addr)>>32);
    // set out img
    Xil_Out32(InstancePtr+ XFILTER2D_ACCELERATED_CONTROL_ADDR_IMG_OUT_DATA, (u64)(output_img.phys_addr));
    Xil_Out32(InstancePtr+ XFILTER2D_ACCELERATED_CONTROL_ADDR_IMG_OUT_DATA + 4, ((u64)output_img.phys_addr)>>32);
    // set resolution
    Xil_Out32(InstancePtr + XFILTER2D_ACCELERATED_CONTROL_ADDR_ROWS_DATA, rows);
    Xil_Out32(InstancePtr + XFILTER2D_ACCELERATED_CONTROL_ADDR_COLS_DATA, cols);
}

void filter2d_kernel_run(u64 InstancePtr)
{
	//XGaussian_filter_accel_Start(&gaussFilter);
    u32 data_start = Xil_In32(InstancePtr + XFILTER2D_ACCELERATED_CONTROL_ADDR_AP_CTRL) & 0x80;
    Xil_Out32(InstancePtr + XFILTER2D_ACCELERATED_CONTROL_ADDR_AP_CTRL, data_start | 0x01);
	
	while(!XFilter2d_accelerated_IsDone(InstancePtr));
}

void filter2d_kernel_run_async(u64 InstancePtr)
{
	//XGaussian_filter_accel_Start(&gaussFilter);
    u32 data_start = Xil_In32(InstancePtr + XFILTER2D_ACCELERATED_CONTROL_ADDR_AP_CTRL) & 0x80;
    Xil_Out32(InstancePtr + XFILTER2D_ACCELERATED_CONTROL_ADDR_AP_CTRL, data_start | 0x01);
}

void filter2d_kernel_run_wait(u64 InstancePtr)
{
	while(!XFilter2d_accelerated_IsDone(InstancePtr));
}

u32 XFilter2d_accelerated_IsDone(u64 InstancePtr) {
    u32 Data;

    Data = Xil_In32(InstancePtr+XFILTER2D_ACCELERATED_CONTROL_ADDR_AP_CTRL);
    return (Data >> 1) & 0x1;
}