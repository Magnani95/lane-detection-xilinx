#include<xtimer.h>

u8 XTmrCtr_Offsets[] = { 0, XTC_TIMER_COUNTER_OFFSET };

#define XTmrCtr_ReadReg(BaseAddress, TmrCtrNumber, RegOffset)	\
	Xil_In32((BaseAddress) + XTmrCtr_Offsets[(TmrCtrNumber)] + \
			(RegOffset))

#define XTmrCtr_WriteReg(BaseAddress, TmrCtrNumber, RegOffset, ValueToWrite)\
	Xil_Out32(((BaseAddress) + XTmrCtr_Offsets[(TmrCtrNumber)] +	\
			(RegOffset)), (ValueToWrite))

void XTmrCtr_Start(u64 InstancePtr, u8 TmrCtrNumber)
{
	u32 ControlStatusReg;

	ControlStatusReg = XTmrCtr_ReadReg(InstancePtr,
					      TmrCtrNumber, XTC_TCSR_OFFSET);
						  
	XTmrCtr_WriteReg(InstancePtr, TmrCtrNumber,
			  XTC_TCSR_OFFSET,
			  XTC_CSR_LOAD_MASK);

	XTmrCtr_WriteReg(InstancePtr, TmrCtrNumber,
			  XTC_TCSR_OFFSET,
			  ControlStatusReg | XTC_CSR_ENABLE_TMR_MASK);
}

void XTmrCtr_Stop( u64 InstancePtr, u8 TmrCtrNumber)
{
	u32 ControlStatusReg;
	/*
	 * Read the current register contents
	 */
	ControlStatusReg = XTmrCtr_ReadReg(InstancePtr,
					      TmrCtrNumber, XTC_TCSR_OFFSET);
	/*
	 * Disable the timer counter such that it's not running
	 */
	ControlStatusReg &= (u32)~(XTC_CSR_ENABLE_TMR_MASK);

	/*
	 * Write out the updated value to the actual register.
	 */
	XTmrCtr_WriteReg(InstancePtr, TmrCtrNumber,
			  XTC_TCSR_OFFSET, ControlStatusReg);

	/*
	 * Indicate that the timer is stopped
	 */
	// if (TmrCtrNumber == 0) {
	// 	InstancePtr->IsStartedTmrCtr0 = 0;
	// } else {
	// 	InstancePtr->IsStartedTmrCtr1 = 0;
	// }
}

void XTmrCtr_Reset(u64 InstancePtr, u8 TmrCtrNumber)
{
	u32 CounterControlReg;

	/*
	 * Read current contents of the register so it won't be destroyed
	 */
	CounterControlReg = XTmrCtr_ReadReg(InstancePtr,
					       TmrCtrNumber, XTC_TCSR_OFFSET);
	/*
	 * Reset the timer by toggling the reset bit in the register
	 */
	XTmrCtr_WriteReg(InstancePtr, TmrCtrNumber,
			  XTC_TCSR_OFFSET,
			  CounterControlReg | XTC_CSR_LOAD_MASK);

	XTmrCtr_WriteReg(InstancePtr, TmrCtrNumber,
			  XTC_TCSR_OFFSET, CounterControlReg);
}


u32 XTmrCtr_GetValue(u64 InstancePtr, u8 TmrCtrNumber)
{
	return XTmrCtr_ReadReg(InstancePtr,
				  TmrCtrNumber, XTC_TCR_OFFSET);
}
//------------------------------------------------------------------------------