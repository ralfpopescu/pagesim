#include "pagetable.h"
#include "global.h"
#include <assert.h>

/**
 * This function returns the page offset of a virtual address.
 *
 * @param virtual_address The 64 bit virtual address that the CPU has generated
 *
 * @return The page offset based on the page size
 */
uint64_t get_offset(uint64_t virtual_address)
{
	// Use page size from global.h to compute this

	/********* TODO ************/
    //virtual_address_size
    uint64_t mask = (1 << page_size) - 1;
	return virtual_address & mask;
}

/**
 * This function returns the vpn linked to a virtual address.
 *
 * @param virtual_address The VPN of the address
 *
 * @return The VPN based on virtual address size and page size
 */
uint64_t get_vpn(uint64_t virtual_address)
{
	// Use page size and virtual_address_size from global.h to compute this

	/********* TODO ************/
    uint64_t maskAmt = virtual_address_size - page_size;
    uint64_t shifted = virtual_address >> page_size;
    uint64_t mask = (1 << maskAmt) - 1;
	return shifted & mask;
}
