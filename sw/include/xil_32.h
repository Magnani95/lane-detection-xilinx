#ifndef XIL_32_H
#define XIL_32_H

#include <stdint.h>

    
    typedef uint8_t   u8;
    typedef uint16_t u16;
    typedef uint32_t u32;
    typedef uint64_t u64;

static inline u32 Xil_In32(u64 Addr)
{
	return *(volatile u32 *) Addr;
}

static inline void Xil_Out32(u64 Addr, u32 Value)
{
	volatile u32 *LocalAddr = (volatile u32 *)Addr;
	*LocalAddr = Value;
}
#endif