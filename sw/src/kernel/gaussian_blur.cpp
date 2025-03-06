#include<kernel/guassian_blur.h>

//void gaussian_kernel(u64 InstancePtr, unsigned char input_img[], unsigned char output_img[])
void gaussian_kernel_set(u64 InstancePtr, addr_t input_img, addr_t output_img, u32 rows, u32 cols)
{	
	// ocv sigma is calculated from kernel-size
	// sigma = 0.3*((ksize-1)*0.5 - 1) + 0.8
	//const float sigma = 0.3*((3-1)*0.5 - 1) + 0.8;
	const float sigma=0.5f;
    
	//set_rows & cols & sigma
	Xil_Out32(InstancePtr+XGAUSSIAN_FILTER_ACCEL_CONTROL_ADDR_ROWS_DATA,rows);
	Xil_Out32(InstancePtr+XGAUSSIAN_FILTER_ACCEL_CONTROL_ADDR_COLS_DATA, cols);
	Xil_Out32(InstancePtr+XGAUSSIAN_FILTER_ACCEL_CONTROL_ADDR_SIGMA_DATA, *(u32*)&sigma);

	//Get sigma
	// u32 sigma_bits =  Xil_In32(InstancePtr+XGAUSSIAN_FILTER_ACCEL_CONTROL_ADDR_SIGMA_DATA);
	// float get_sigma = (*(float*)&sigma_bits);

	//set input img
	Xil_Out32(InstancePtr+XGAUSSIAN_FILTER_ACCEL_CONTROL_ADDR_IMG_INP_DATA, (u64)input_img.phys_addr);
	Xil_Out32(InstancePtr+XGAUSSIAN_FILTER_ACCEL_CONTROL_ADDR_IMG_INP_DATA+4, ((u64)input_img.phys_addr)>>32);

	//u64 input_address = Xil_In32(InstancePtr+XGAUSSIAN_FILTER_ACCEL_CONTROL_ADDR_IMG_INP_DATA);
	//nput_address	 += ((u64) Xil_In32(InstancePtr+XGAUSSIAN_FILTER_ACCEL_CONTROL_ADDR_IMG_INP_DATA+4))<<32;

    //set output img
	Xil_Out32(InstancePtr+XGAUSSIAN_FILTER_ACCEL_CONTROL_ADDR_IMG_OUT_DATA, (u64)output_img.phys_addr);
	Xil_Out32(InstancePtr+XGAUSSIAN_FILTER_ACCEL_CONTROL_ADDR_IMG_OUT_DATA+4, ((u64)output_img.phys_addr)>>32);

	//u64 output_address = Xil_In32(InstancePtr+XGAUSSIAN_FILTER_ACCEL_CONTROL_ADDR_IMG_OUT_DATA);
	//output_address	  += ((u64) Xil_In32(InstancePtr+XGAUSSIAN_FILTER_ACCEL_CONTROL_ADDR_IMG_OUT_DATA+4))<<32;

}

void gaussian_kernel_run(u64 InstancePtr)
{  
	//XGaussian_filter_accel_Start(&gaussFilter);
	u32 data_start = Xil_In32(InstancePtr+XGAUSSIAN_FILTER_ACCEL_CONTROL_ADDR_AP_CTRL)& 0x80;

	Xil_Out32(InstancePtr+XGAUSSIAN_FILTER_ACCEL_CONTROL_ADDR_AP_CTRL,data_start | 0x01 );

	while(!XGaussian_filter_accel_IsDone(InstancePtr));
}

void gaussian_kernel_run_async(u64 InstancePtr)
{  
	//XGaussian_filter_accel_Start(&gaussFilter);
	u32 data_start = Xil_In32(InstancePtr+XGAUSSIAN_FILTER_ACCEL_CONTROL_ADDR_AP_CTRL)& 0x80;

	Xil_Out32(InstancePtr+XGAUSSIAN_FILTER_ACCEL_CONTROL_ADDR_AP_CTRL,data_start | 0x01 );
}

void gaussian_kernel_run_wait(u64 InstancePtr)
{ 
	while(!XGaussian_filter_accel_IsDone(InstancePtr));
}
u32 XGaussian_filter_accel_IsDone(u64 InstancePtr)
{
    u32 Data;

    Data = Xil_In32(InstancePtr + XGAUSSIAN_FILTER_ACCEL_CONTROL_ADDR_AP_CTRL);
    return (Data >> 1) & 0x1;
}