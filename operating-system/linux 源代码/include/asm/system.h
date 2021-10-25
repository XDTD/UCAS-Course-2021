#define move_to_user_mode() \
__asm__ ("movl %%esp,%%eax\n\t" \
	"pushl $0x17\n\t" \ //10111, 10是ldt数据段
	"pushl %%eax\n\t" \
	"pushfl\n\t" \
	"pushl $0x0f\n\t" \
	"pushl $1f\n\t" \
	"iret\n" \  // 模拟中断返回的栈，和cpu架构绑定80x86,基站
	"1:\tmovl $0x17,%%eax\n\t" \
	"movw %%ax,%%ds\n\t" \
	"movw %%ax,%%es\n\t" \
	"movw %%ax,%%fs\n\t" \
	"movw %%ax,%%gs" \
	:::"ax")

#define sti() __asm__ ("sti"::)
#define cli() __asm__ ("cli"::)
#define nop() __asm__ ("nop"::)

#define iret() __asm__ ("iret"::)

// dp1特权级
// 两个冒号把程序分三段
// 借用两个寄存器，凑
#define _set_gate(gate_addr,type,dpl,addr) \
__asm__ ("movw %%dx,%%ax\n\t" \
	"movw %0,%%dx\n\t" \
	"movl %%eax,%1\n\t" \  // 1对应第一个"o"，下面两行就是
	"movl %%edx,%2" \      
	: \
	: "i" ((short) (0x8000+(dpl<<13)+(type<<8))), \  // 凑idt的格式
	"o" (*((char *) (gate_addr))), \               //  eax送到0
	"o" (*(4+(char *) (gate_addr))), \              // edx送到+4
	"d" ((char *) (addr)),"a" (0x00080000))

#define set_intr_gate(n,addr) \
	_set_gate(&idt[n],14,0,addr) // &int[n] 就能找到描述符，描述符包含选择子（找gdt和段）和偏移（具体位置）

#define set_trap_gate(n,addr) \
	_set_gate(&idt[n],15,0,addr) // 15即二进制的1111

#define set_system_gate(n,addr) \
	_set_gate(&idt[n],15,3,addr)  // 特权级不一样

#define _set_seg_desc(gate_addr,type,dpl,base,limit) {\
	*(gate_addr) = ((base) & 0xff000000) | \
		(((base) & 0x00ff0000)>>16) | \
		((limit) & 0xf0000) | \
		((dpl)<<13) | \
		(0x00408000) | \
		((type)<<8); \
	*((gate_addr)+1) = (((base) & 0x0000ffff)<<16) | \
		((limit) & 0x0ffff); }

#define _set_tssldt_desc(n,addr,type) \
__asm__ ("movw $104,%1\n\t" \
	"movw %%ax,%2\n\t" \
	"rorl $16,%%eax\n\t" \
	"movb %%al,%3\n\t" \
	"movb $" type ",%4\n\t" \
	"movb $0x00,%5\n\t" \
	"movb %%ah,%6\n\t" \
	"rorl $16,%%eax" \
	::"a" (addr), "m" (*(n)), "m" (*(n+2)), "m" (*(n+4)), \
	 "m" (*(n+5)), "m" (*(n+6)), "m" (*(n+7)) \
	)

#define set_tss_desc(n,addr) _set_tssldt_desc(((char *) (n)),addr,"0x89")
#define set_ldt_desc(n,addr) _set_tssldt_desc(((char *) (n)),addr,"0x82")
