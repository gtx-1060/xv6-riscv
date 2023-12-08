// Physical memory allocator, for user processes,
// kernel stacks, page-table pages,
// and pipe buffers. Allocates whole 4096-byte pages.

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"
#include "buddy.h"

#define PGSTART(end) PGROUNDUP((uint64)end)
#define PAGES(end) (((PHYSTOP - PGSTART(end)) / PGSIZE) + 1)
#define PAGE_IND(addr, end) (( (uint64)addr - PGSTART(end) ) / PGSIZE)

extern char end[];  // first address after kernel.
                    // defined by kernel.ld.
uint8* page_refs;
struct spinlock pagelock;

void kinit() {
  char *p = (char*)PGSTART(end);
  bd_init(p, (void*)PHYSTOP);

  page_refs = bd_malloc(PAGES(end));
  memset(page_refs, 0, PAGES(end));
  initlock(&pagelock, "local_page_refs");
}

// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void kfree(void *pa) {
  acquire(&pagelock);
  if (pa == 0)
    return;
  uint ind = PAGE_IND(pa, end);
  if (page_refs[ind] == 0) {
    release(&pagelock);
//    printf("attempt to free freed page!\n");
    return;
  }
  else if (page_refs[ind] == 1)
    bd_free(pa);
  page_refs[ind] -= 1;
  release(&pagelock);
//  printf("kfree at %u (%u)\n", ind , page_refs[ind]);
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *kalloc(void) {
  acquire(&pagelock);
  void* pa = bd_malloc(PGSIZE);
  if (pa == 0){
    release(&pagelock);
    return 0;
  }
  page_refs[PAGE_IND(pa, end)] = 1;
  release(&pagelock);
//  printf("kalloc at %u\n", PAGE_IND(pa, end));
  return pa;
}

void kaddref(void* pa) {
  acquire(&pagelock);
  page_refs[PAGE_IND(pa, end)] += 1;
  release(&pagelock);
}

uint8 kpagerefs(void* pa) {
  acquire(&pagelock);
  uint8 refs = page_refs[PAGE_IND(pa, end)];
  release(&pagelock);
  return refs;
}
