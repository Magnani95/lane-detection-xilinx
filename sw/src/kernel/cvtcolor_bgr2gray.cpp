#include<kernel/cvtcolor_bgrgray.h>

void cvtcolor_bgr2gray_set(u64 InstancePtr,addr_t input_img, addr_t output_img, u32 rows, u32 cols)
{

    Xil_Out32(InstancePtr+XCVTCOLOR_BGR2GRAY_CONTROL_ADDR_ROWS_DATA, (u32) rows);
    Xil_Out32(InstancePtr+XCVTCOLOR_BGR2GRAY_CONTROL_ADDR_COLS_DATA, (u32) cols);

    //printf("setting the accelerator...\n");
	//XCvtcolor_bgr2gray_Set_imgInput(&cvtcolor, (u64) input_img);
    Xil_Out32(InstancePtr+XCVTCOLOR_BGR2GRAY_CONTROL_ADDR_IMGINPUT_DATA, (u64)(input_img.phys_addr));
    Xil_Out32(InstancePtr+XCVTCOLOR_BGR2GRAY_CONTROL_ADDR_IMGINPUT_DATA + 4, ((u64)input_img.phys_addr)>> 32);

	//XCvtcolor_bgr2gray_Set_imgOutput(&cvtcolor, (u64) output_img);
    Xil_Out32(InstancePtr+XCVTCOLOR_BGR2GRAY_CONTROL_ADDR_IMGOUTPUT_DATA, (u64)(output_img.phys_addr));
    Xil_Out32(InstancePtr+XCVTCOLOR_BGR2GRAY_CONTROL_ADDR_IMGOUTPUT_DATA + 4, ((u64)output_img.phys_addr)>> 32);

}

void cvtcolor_bgr2gray_run(u64 InstancePtr)
{
    u32 data = Xil_In32(InstancePtr+ XCVTCOLOR_BGR2GRAY_CONTROL_ADDR_AP_CTRL) & 0x80;
    Xil_Out32(InstancePtr+XCVTCOLOR_BGR2GRAY_CONTROL_ADDR_AP_CTRL, data | 0x01);

	while (!XCvtcolor_bgr2gray_IsDone(InstancePtr));
}

void cvtcolor_bgr2gray_run_async(u64 InstancePtr)
{
    u32 data = Xil_In32(InstancePtr+ XCVTCOLOR_BGR2GRAY_CONTROL_ADDR_AP_CTRL) & 0x80;
    Xil_Out32(InstancePtr+XCVTCOLOR_BGR2GRAY_CONTROL_ADDR_AP_CTRL, data | 0x01);
}

void cvtcolor_bgr2gray_run_wait(u64 InstancePtr)
{
	while (!XCvtcolor_bgr2gray_IsDone(InstancePtr));
}

u32 XCvtcolor_bgr2gray_IsDone(u64 InstancePtr) 
{
    u32 Data;

    Data = Xil_In32(InstancePtr+XCVTCOLOR_BGR2GRAY_CONTROL_ADDR_AP_CTRL);
    return (Data >> 1) & 0x1;
}