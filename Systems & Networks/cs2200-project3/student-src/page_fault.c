#include "mmu.h"
#include "pagesim.h"
#include "swapops.h"
#include "stats.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

/**
 * --------------------------------- PROBLEM 6 --------------------------------------
 * Checkout PDF section 7 for this problem
 * 
 * Page fault handler.
 * 
 * When the CPU encounters an invalid address mapping in a page table, it invokes the 
 * OS via this handler. Your job is to put a mapping in place so that the translation 
 * can succeed.
 * 
 * @param addr virtual address in the page that needs to be mapped into main memory.
 * 
 * HINTS:
 *      - You will need to use the global variable current_process when
 *      altering the frame table entry.
 *      - Use swap_exists() and swap_read() to update the data in the 
 *      frame as it is mapped in.
 * ----------------------------------------------------------------------------------
 */
void page_fault(vaddr_t addr) {
   // TODO: Get a new frame, then correctly update the page table and frame table

   // step 1
   // gets page table entry
   vpn_t vPage = vaddr_vpn(addr); // we need vpn first
   // pfn_t pPage = (*current_process).saved_ptbr;
   pte_t *ptEntry = (pte_t*) (mem + PTBR*PAGE_SIZE) + vPage; //index into page table of current process
   
   // step 2
   pfn_t newFrame = free_frame();
   uint8_t *temp = mem + newFrame*PAGE_SIZE;

   //step 3 & 4
   if (swap_exists(ptEntry)) {//how to check if there exists a swap entry?
      swap_read(ptEntry, temp);
   } else {
      (void) memset(temp, 0, PAGE_SIZE);
   }

   //step 5
   (*ptEntry).pfn = newFrame;

   //step 6: update flags in ft and pt
   (*ptEntry).valid = 1;
   (*ptEntry).dirty = 0;
   fte_t *ftEntry =  frame_table + newFrame;
   (*ftEntry).protected = 0;
   (*ftEntry).mapped = 1;
   (*ftEntry).referenced = 1;
   (*ftEntry).process = current_process;
   (*ftEntry).vpn = vPage;
}

#pragma GCC diagnostic pop
