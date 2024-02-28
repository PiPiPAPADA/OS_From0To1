#include "idt.h"
#include "string.h"
#include "debug.h"
#include "vmm.h"
#include "pmm.h"

pgd_t pgd_kern[PGD_SIZE] __attribute__((aligned(PAGE_SIZE)));
static pte_t pte_kern[PTE_COUNT][PTE_SIZE] __attribute__((aligned(PAGE_SIZE)));

void init_vmm(){
    //0xc0000000 这个地址在页目录的索引
    uint32_t kern_pte_first_idx  = PGD_INDEX(PAGE_OFFSET);

    uint32_t i,j;
    for(i = kern_pte_first_idx,j=0;i<PTE_COUNT+kern_pte_first_idx;i++,j++){
        //此处是内核va, MMU需要PA, 减去偏移
        pgd_kern[i] = ((uint32_t)pte_kern[j]-PAGE_OFFSET) | PAGE_PRESENT | PAGE_WRITE;
    }

    uint32_t *pte = (uint32_t *) pte_kern;
    //不映射第0页，便于跟踪NULL指针
    for(i=1;i<PTE_COUNT * PTE_SIZE;i++){
        pte[i] = (i<<12) | PAGE_PRESENT | PAGE_WRITE;
    }

    uint32_t pgd_kern_phy_addr = (uint32_t) pgd_kern - PAGE_OFFSET;
    //register IH handler
    register_interrupt_handler(14, &page_fault);

    switch_pgd(pgd_kern_phy_addr);
}

void switch_pgd(uint32_t pd){
    asm volatile("mov %0, %%cr3": : "r"(pd));
}

void map(pgd_t *pgd_now, uint32_t va, uint32_t pa, uint32_t flags){
    uint32_t pgd_idx = PGD_INDEX(va);
    uint32_t pte_idx = PTE_INDEX(va);
    
    pte_t *pte = (pte_t *) (pgd_now[pgd_idx] & PAGE_MASK);

    if(!pte){
        pte = (pte_t *) pmm_alloc_page();
        pgd_now[pgd_idx] = (uint32_t) pte | PAGE_PRESENT | PAGE_WRITE;
        //switch kernel address
        pte = (pte_t *)((uint32_t) pte + PAGE_OFFSET);
        bzero(pte,PAGE_SIZE);
    } else {
        pte = (pte_t *) ((uint32_t) pte + PAGE_OFFSET);
    }
    pte[pte_idx] = (pa & PAGE_MASK) | flags;
    //通知CPU更新页表缓存
    asm volatile ("invlpg (%0)": : "a"(va));
}

void unmap(pgd_t *pgd_now, uint32_t va){
    uint32_t pgd_idx = PGD_INDEX(va);
    uint32_t pte_idx = PTE_INDEX(va);

    pte_t *pte = (pte_t*) (pgd_now[pgd_idx] & PAGE_MASK);
    if(!pte){
        return;
    }
    pte = (pte_t *) ((uint32_t)pte + PAGE_OFFSET);
    pte[pte_idx] = 0;
    asm volatile ("invlpg (%0)": : "a"(va));
}

uint32_t get_mapping(pgd_t* pgd_now, uint32_t va, uint32_t *pa){
    uint32_t pgd_idx = PGD_INDEX(va);
    uint32_t pte_idx = PTE_INDEX(va);

    pte_t *pte = (pte_t*) (pgd_now[pgd_idx] & PAGE_MASK);
    if(!pte){
        return 0;
    }
    pte = (pte_t *) ((uint32_t)pte + PAGE_OFFSET);
    if(pte[pte_idx] != 0 && pa){
        *pa = pte[pte_idx] & PAGE_MASK;
        return 1;
    }
    return 0;
}