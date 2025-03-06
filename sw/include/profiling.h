#ifndef PROFILING_H
#define PROFILING_H

#include<chrono>
#include<iostream>
#include<xtimer.h>
#include<general_settings.h>

#if	PROFILING_STD_CHRONO
	//std::chrono::time_point<std::chrono::steady_clock> start, end;
	#define CLK_NOW(a) (a)=std::chrono::steady_clock::now();
	#define CLK_INTERVAL(start, end) std::chrono::duration_cast<std::chrono::microseconds>( (end)-(start)).count()
	#define CLK_PRINT(message, start, end) std::cout<<"++"<<(message)<<" - [microsec]:\t"<<CLK_INTERVAL(start, end)<<std::endl;
	#define CLK_MEASURE(command, start, end) {CLK_NOW(start); (command); CLK_NOW(end); CLK_PRINT( (#command), (start), (end))}
#else
	#define CLK_NOW(a)
	#define CLK_INTERVAL(a, b)
	#define CLK_PRINT(a, b, c)
	#define CLK_MEASURE(command, start, end) (command);
#endif

#define FCLK_NOW(a) (a)=std::chrono::steady_clock::now();
#define FCLK_INTERVAL(start, end) std::chrono::duration_cast<std::chrono::microseconds>( (end)-(start)).count()
#define FCLK_PRINT(message, start, end) std::cout<<"\n++"<<(message)<<" - [microsec]:\t"<<FCLK_INTERVAL(start, end)<<std::endl;
#define FCLK_MEASURE(command, start, end) {FCLK_NOW(start); (command); FCLK_NOW(end); FCLK_PRINT( (#command), (start), (end))}

// #if PROFILING_XTIMER
// 	#define X_PRE_MEASURE 	XTmrCtr_Reset(xtimer.virt_addr, 0); \
// 							u32 s = XTmrCtr_GetValue(xtimer.virt_addr, 0);	\
// 							XTmrCtr_Start(xtimer.virt_addr, 0);

// 	#define X_POST_MEASURE	XTmrCtr_Stop(xtimer.virt_addr, 0); \
// 							u32 e = XTmrCtr_GetValue(xtimer.virt_addr, 0); \
// 							printf("%u\n", (e-s));
							
// 							//printf("-->%u \t(%u) \t- (%u)\n", (e-s),s,e);
#if PROFILING_XTIMER
	#define X_PRE_MEASURE 	XTmrCtr_Reset(xtimer.virt_addr, 0); \
							XTmrCtr_Start(xtimer.virt_addr, 0);

	#define X_POST_MEASURE	XTmrCtr_Stop(xtimer.virt_addr, 0); \
							printf("%u\n", XTmrCtr_GetValue(xtimer.virt_addr,0)); \
							XTmrCtr_Reset(xtimer.virt_addr, 0);
							//printf("-->%u \t(%u) \t- (%u)\n", (e-s),s,e);
							
#else
	#define X_PRE_MEASURE
	#define X_POST_MEASURE
#endif

#endif	//PROFILING_H