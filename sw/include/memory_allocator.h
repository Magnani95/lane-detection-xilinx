#ifndef ALLOC_H
#define ALLOC_H

    #include <fcntl.h>
    #include <stdio.h>
    #include <errno.h>
    #include <string.h>
    #include <sys/mman.h>
    #include <unistd.h>

    #include <stdint.h>

    #define PAGESIZE 4*1024

    /**************************** Type Definitions ******************************/
    typedef uint8_t   u8;
    typedef uint16_t u16;
    typedef uint32_t u32;
    typedef uint64_t u64;

    typedef struct{
        u64 phys_addr;
        u64 virt_addr;
    } addr_t;

    /**************************** Type Definitions ******************************/

    struct memory_allocator {
        u64    _current_address_offset;
        size_t _allocation_size;
        addr_t _allocation;
    };

    void memory_allocator_init   (struct memory_allocator * alloc, u64 phys_addr, size_t size);
    void memory_allocator_deinit (struct memory_allocator * alloc);
    addr_t memory_allocator_alloc(struct memory_allocator * alloc, size_t size);
#endif