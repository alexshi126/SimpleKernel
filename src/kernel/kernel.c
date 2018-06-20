
// This file is a part of MRNIU/SimpleKernel (https://github.com/MRNIU/SimpleKernel).

// kernel.c for MRNIU/SimpleKernel.

#include "../include/kernel.h"
#include "../include/port.h"
#include "../include/vga.h"
#include "../include/tty.h"

// 内核入口
void kernel_main(void)
{
	// 系统初始化
	terminal_init();	// 控制台初始化
	gdt_init();	// GDT 初始化
	idt_init();	// IDT 初始化



	// 输出一些基本信息
	printk("Welcome to my kernel.\n");
	printk("terminal_color: %d\n", terminal_color);
	printk("current cursor pos: %d\n", terminal_getcursorpos());

	//asm volatile ("int $0x01");
	//asm volatile ("int $0x04");

	printk("int 0x03 successful!");


}