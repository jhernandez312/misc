#include "proc.h"
#include "mmu.h"
#include "pagesim.h"
#include "va_splitting.h"
#include "swapops.h"
#include "stats.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

/**
 * --------------------------------- PROBLEM 3 --------------------------------------
 * Checkout PDF section 4 for this problem
 * 
 * This function gets called every time a new process is created.
 * You will need to allocate a frame for the process's page table using the
 * free_frame function. Then, you will need update both the frame table and
 * the process's PCB. 
 * 
 * @param proc pointer to process that is being initialized 
 * 
 * HINTS:
 *      - pcb_t: struct defined in pagesim.h that is a process's PCB.
 *      - You are not guaranteed that the memory returned by the free frame allocator
 *      is empty - an existing frame could have been evicted for our new page table.
 * ----------------------------------------------------------------------------------
 */
void proc_init(pcb_t *proc) {
    // TODO: initialize proc's page table.
    pfn_t num = free_frame(); // gets frame
    void *frameAddr = mem + num*PAGE_SIZE; // gets physical address
    memset(frameAddr, 0, PAGE_SIZE); // clears frame

    (*proc).saved_ptbr = num; //update process's PCB

    fte_t *entry = frame_table + num; //updating frame table

    (*entry).protected = 1; //setting properties
    (*entry).mapped = 1;
    (*entry).process = proc;


}

/**
 * --------------------------------- PROBLEM 4 --------------------------------------
 * Checkout PDF section 5 for this problem
 * 
 * Switches the currently running process to the process referenced by the proc 
 * argument.
 * 
 * Every process has its own page table, as you allocated in proc_init. You will
 * need to tell the processor to use the new process's page table.
 * 
 * @param proc pointer to process to become the currently running process.
 * 
 * HINTS:
 *      - Look at the global variables defined in pagesim.h. You may be interested in
 *      the definition of pcb_t as well.
 * ----------------------------------------------------------------------------------
 */
void context_switch(pcb_t *proc) {
    // TODO: update any global vars and proc's PCB to match the context_switch.
    // (*current_process).state = 0;
    // current_process = proc;
    // (*current_process).state = 1;
    PTBR = (*proc).saved_ptbr; //updating ptbr

}

/**
 * --------------------------------- PROBLEM 8 --------------------------------------
 * Checkout PDF section 8 for this problem
 * 
 * When a process exits, you need to free any pages previously occupied by the
 * process.
 * 
 * HINTS:
 *      - If the process has swapped any pages to disk, you must call
 *      swap_free() using the page table entry pointer as a parameter.
 *      - If you free any protected pages, you must also clear their"protected" bits.
 * ----------------------------------------------------------------------------------
 */
void proc_cleanup(pcb_t *proc) {
    // TODO: Iterate the proc's page table and clean up each valid page
    for (size_t i = 0; i < NUM_PAGES; i++) {
        //have to create a pointer to entry to process's pt
        pfn_t ptFrame = (*proc).saved_ptbr;
        pte_t *ptEntry = (pte_t*)(mem + ptFrame*PAGE_SIZE + i);
        pfn_t pFramePage = (*ptEntry).pfn; 

        if ((*ptEntry).valid) {
            (*ptEntry).valid = 0;
            frame_table[pFramePage].mapped = 0;
        }
        if (swap_exists(ptEntry)) {
            swap_free(ptEntry);
        }
    }

    //editing the frame table properties
    fte_t *currFT = frame_table + (*proc).saved_ptbr;
    (*currFT).mapped = 0;
    (*currFT).protected = 0;

}

#pragma GCC diagnostic pop
