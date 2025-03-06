#ifndef XTIMER_H
#define XTIMER_H

#include <stdint.h>
#include <xil_32.h>

#define XTIMER_BASE_ADDRESS     0xA0000000

#define XTC_TCSR_OFFSET			0
#define XTC_CSR_LOAD_MASK		0x00000020
#define XTC_CSR_ENABLE_TMR_MASK		0x00000080 
#define XTC_TIMER_COUNTER_OFFSET	16
#define XTC_TCR_OFFSET		8
    
typedef uint8_t   u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

void XTmrCtr_Start(u64 InstancePtr, u8 TmrCtrNumber);
void XTmrCtr_Stop( u64 InstancePtr, u8 TmrCtrNumber);
void XTmrCtr_Reset(u64 InstancePtr, u8 TmrCtrNumber);

u32 XTmrCtr_GetValue(u64 InstancePtr, u8 TmrCtrNumber);

#endif

/*
	u32 s, e;

    XTmrCtr_Start(xtimer.virt_addr,0);
	s = XTmrCtr_GetValue(xtimer.virt_addr, 0);
	<command to measure;>
    e = XTmrCtr_GetValue(xtimer.virt_addr, 0);
	printf("-->%u \t(%u) \t- (%u)\n", (e-s),s,e);
*/
