#include "reverselookup.h"
#include "pagetable.h"
#include "stats.h"
#include "tlb.h"

static uint64_t find_free_frame(stats_t *stats);

/**
 * This function is used to resolve a page fault. It finds a free frame, and returns
 * the PFN after doing some book keeping
 *
 * @param vpn The virtual page number that has to be looked up
 * @param rw Specifies if an access is a read or a write
 * @param stats The struct used to store stats
 *
 */
uint64_t page_fault_handler(uint64_t vpn, char rw, stats_t *stats)
{
	/* DO NOT MODIFY BELOW LINE */
	uint64_t victim_pfn = find_free_frame(stats);
	/****************************/

	// The above call finds a free frame using the below function
	// Implement that first

	// Now that you have a free frame you can use:

	// (a) Update RLT
	// 		(1) Set the PCB of the RLT at victim_pfn
	// 		(2) Set the RLT at victim_pfn as valid
	// 		(3) Set the VPN in the RLT entry at victim_pfn
	// (b) Update the page table of the page faulting procees using the current_pagetable
	//		(1) Set it valid
	// 		(2) Set the correct VPN to PFN mapping


	/********* TODO ************/

    stats->page_faults += 1;
    
    rlte_t* entry = &(rlt[victim_pfn]);
    
    entry->task_struct = current_process; //(1)
    entry->valid = 1; //(2)
    entry->vpn = vpn; //(3)
    
    
    pte_t* page = &(current_pagetable[vpn]);
    page->valid = 1;
    page->pfn = victim_pfn;
    page->frequency = 1;
    
    
    if(rw == WRITE){
        page->dirty = 1;
    } else {
        page->dirty = 0;
        stats->reads_from_disk += 1;
    }
    
    


	// return victim page frame number
	return victim_pfn;
}

/**
 * This functions finds a free frame by using the Least Frequently Used algorithm
 *
 * @param stats The struct used for keeping track of statistics
 *
 * @return The physical frame number that the page fault handler can use
 */
static uint64_t find_free_frame(stats_t *stats)
{
	// (1) Look for an invalid frame in the RLT - Return that frame
	// (2) Use Least frequently used to identify victim frame
	//		(a) Update the victim page table
	// 			(i) Use the RLT to find the page table of the victim process
	//			(ii) Mark it invalid
	//			(iii) Increment writebacks if the victim frame is dirty
	//		(b) Return the victim frame

	uint64_t victim_pfn = 0;

	/********* TODO ************/

   
    
    int rltsize = 1 << rlt_size;
    rlte_t* rlte = 0;
    //rlte_t* invalid_rlte = 0;
    
    //int found = 0;
    uint64_t LFU = 9999999;
    uint64_t accesses = 9999999;
    rlte_t* victim = &(rlt[0]);
    
    for(int i = 0; i < rltsize; i++) { //go through every physical frame
        rlte = &(rlt[i]); //rlte is entry


        if(rlte->valid == 0) {
            //we have invalid page           
            
            return i;
        }

        task_struct* ts = rlte->task_struct; //get process using physical frame
        pte_t* pt = ts->pagetable;  //get page table of process
        pte_t* entry = &(pt[rlte->vpn]); //get entry at that vpn

        accesses = entry->frequency; //frequency to compare

        if(accesses < LFU) {
            LFU = accesses;
            victim = rlte; //we have LFU page
            victim_pfn = i;
        }
    }
    
    task_struct* victim_task_struct = victim->task_struct; //access PCB of victim process
    pte_t* victim_pagetable = victim_task_struct->pagetable; //access page table of victim frame
    pte_t* victim_pte = &(victim_pagetable[victim->vpn]); //access mapped entry

    //victim_pfn = victim_pte->pfn;
    
    victim_pte->valid = 0; //make mapping invalid in other process page table
    
    
    if(victim_pte->dirty == 1) {
        stats->writes_to_disk += 1;
    }
    
    
    


	// If the victim page table entry belongs to the current process, there is a chance
	// that it also resides in the TLB. So to avoid fake valid mappings from the TLB,
	// we clear that particular TLB entry.
	/******************* DO NOT MODIFY CODE BELOW THIS LINE **************/
	if (rlt[victim_pfn].task_struct == current_process) {
		tlb_clearOne(rlt[victim_pfn].vpn);
	}
	return victim_pfn;
}
