
#include "mm/mem.h"
#include "type.h"

#include "driver/riscv.h"


/* 映射类型 */
#define MAP_TYPE_IDENTICAL  0  // 恒等映射
#define MAP_TYPE_FRAMED     1  // 新建页面映射


// 负责管理kernel的地址空间
MemorySet kernel_ms;


u8 kernel_stack[APP_NUM_MAX][KERNEL_STACK_SIZE];
u8 user_stack[APP_NUM_MAX][USER_STACK_SIZE];




u64 get_kernel_stack_top( u64 app_id ) {
    return (u64)(kernel_stack[app_id]) + KERNEL_STACK_SIZE;
}


u64 get_user_stack_top( u64 app_id ) {
    return (u64)(user_stack[app_id]) + USER_STACK_SIZE;
}




/* vector初始化相关函数 */
//比较函数
i64 vPPN_cmp(const void* elemAddr1,const void* elemAddr2) {
    return *(PhysPageNum *)elemAddr1 - *(PhysPageNum *)elemAddr2;
}




/**
 *  @brief: 将相应地址空间建立映射
 *  @param:
 *      pt: 对应的页表管理项
 *      pa_start: 地址空间起始地址
 *      pa_end: 地址空间中止地址
 *      ext_pte_flags：除V位以外的其他标志位
 *      map_type: 映射类型
 *  @return: 
 */
void mm_map_for_mmu(PageTable *pt, PhysAddr pa_start, PhysAddr pa_end, u64 ext_pte_flags, u64 map_type){
    // 解析PPN
    PhysPageNum ppn_start = pa_to_ppn(pa_start, ADDR_FLOOR);
    PhysPageNum ppn_end = pa_to_ppn(pa_end, ADDR_FLOOR);
    PhysPageNum ppn_target;
    // 将对应地址空间的ppn和vpn建立恒等映射
    for(u64 i = ppn_start; i <= ppn_end; i ++){
        /* 确定最终ppn */
        if(map_type == MAP_TYPE_IDENTICAL){ // 恒等映射
            ppn_target = i;
        }else if(map_type == MAP_TYPE_FRAMED){ // 新建页面
            ppn_target = frame_allocator_alloc();
            printk("[mm/mem.c] new frame = 0x%x\n", ppn_target);
        }
        // 建立PPN和VPN的映射
        pt_map_ppn_vpn(pt, ppn_target, i, ext_pte_flags);
    }
}



void test(){
    printk("[mem.c] *0x80201000 = 0x%x\n", *((u64*)0x80201000));
    printk("[mem.c] *0x7ffffff000 = 0x%x\n", *((u8*)0x7fFFFFf000));
}

/**
 *  @brief: 初始化MMU
 *  @param:
 *  @return: 
 */
void mm_init( void ){
    frame_allocator_init();
    // kernel_mm_set_for_mmu_init();
    memory_set_init(&kernel_ms);
    memory_set_kernel_new(&kernel_ms);
    w_satp( ((u64)0x8 << 60) | (u64)kernel_ms.page_table.root_ppn);
    asm volatile("sfence.vma");
    test();
}






/* 纪念写错satp导致的bug，特此保留debug用的其中一个函数（虽然这个函数测不出bug */
#if 0
void mm_map_test(PhysPageNum root_ppn, VirtPageNum vpn){
    u64 vpn_index[3];
    PageTableEntry *target;
    for(i64 i = 2; i >= 0; i --){
        vpn_index[i] = vpn & 0x1ff;
        vpn >>= 9;
    }
    target = (PageTableEntry *)ppn_to_pa(root_ppn) + vpn_index[0];
    for(i64 i = 1; i < 3; i ++){
        target = (PageTableEntry *)ppn_to_pa(pte_get_ppn(*target)) + vpn_index[i];
    }
    // printk("[mm/mem.c] test target pte = 0x%x\n", *target >> 10);
}

// 调用测试函数
mm_map_test(kernel_pt.root_ppn, 0x80200);
mm_map_test(kernel_pt.root_ppn, 0x80201);
mm_map_test(kernel_pt.root_ppn, 0x80202);
mm_map_test(kernel_pt.root_ppn, 0x80222);
#endif
