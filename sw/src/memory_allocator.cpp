#include "memory_allocator.h"

void memory_allocator_init (struct memory_allocator * alloc, u64 phys_addr, size_t size) {

    alloc->_current_address_offset = 0;
    alloc->_allocation_size = size;

    int fd;
    if((fd = open("/dev/mem", O_RDWR | O_SYNC)) == -1) {
        printf("/dev/mem could not be opened.\n");
    }

    alloc->_allocation.phys_addr = phys_addr;        
    alloc->_allocation.virt_addr = (u64) mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, alloc->_allocation.phys_addr);

    if(alloc->_allocation.virt_addr == (u64)MAP_FAILED) {
        printf("Failed to mmap %s\n", strerror(errno));
    }

    close(fd);
}

void memory_allocator_deinit (struct memory_allocator * alloc) {
    munmap((void*)alloc->_allocation.virt_addr, alloc->_allocation_size);
}

addr_t memory_allocator_alloc(struct memory_allocator * alloc, size_t size) {

    addr_t addr;

    addr.virt_addr = alloc->_allocation.virt_addr + alloc->_current_address_offset;
    addr.phys_addr = alloc->_allocation.phys_addr + alloc->_current_address_offset;

    alloc->_current_address_offset += size;

    return addr;
}