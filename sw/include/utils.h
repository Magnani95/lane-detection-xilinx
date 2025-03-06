#ifndef UTILS_H
#define UTILS_H

#include<memory_allocator.h>
#include<cstdio>

#define BYTES_TOLLERANCE 3

void check_bytes(const unsigned char img0[],const unsigned char img1[], size_t lenght);
void confront_images(addr_t img0,const unsigned char img1[], size_t lenght);
void max_byte_count(const unsigned char img0[], const unsigned char ref_img[], size_t lenght);
void zero_fill(unsigned char img[], size_t lenght);

#endif