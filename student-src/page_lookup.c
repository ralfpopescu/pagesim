#include "pagetable.h"
#include <assert.h>
/**
 * This function checks the page table of the current process to find
 * the VPN to PFN mapping.
 * 
 * @param vpn The virtual page number that has to be translated
 * @param offset The page offset of the address that is being translated
 * @param rw Specifies if access is read or a write
 * @param stats The struct used for storing statistics
 *
 * @return The page frame number (aka physical frame number) PFN linked to the VPN
 */
uint64_t page_lookup(uint64_t vpn, uint64_t offset, char rw, stats_t *stats)
{
	uint64_t pfn = 0;

	// (1) Check the "current_pagetable" at the param VPN to find the mapping
	// (2) If the mapping does not exist, call the "page_fault_handler" function
	// (3) Make sure to increment stats
	// (4) Make sure to mark the entry dirty if access is a write
	// (5) Make sure to increment the frequency count of the VPN that has been accessed

	/********* TODO ************/
    
    stats->accesses += 1;
    if(rw == WRITE) {
            stats->writes += 1;
        } else {
            stats->reads += 1;
        }
    
    pte_t* pte = &(current_pagetable[vpn]);
    
    
    
    if(pte->valid){
        pte->frequency += 1;
        if(rw == WRITE) {
            pte->dirty = 1;
        } 
        pfn = pte->pfn;
    } else {
        pfn = page_fault_handler(vpn, rw, stats);
        if(rw == WRITE) {
            pte->dirty = 1;
        }
        pte->frequency += 1;
        pte->valid = 1;
        pte->pfn = pfn;
    }
    

	return pfn;
}
