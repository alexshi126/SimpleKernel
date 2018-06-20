
// This file is a part of MRNIU/SimpleKernel (https://github.com/MRNIU/SimpleKernel).

// kernel.h for MRNIU/SimpleKernel.

#include "stdint.h"
#include "stddef.h"
#include "stdarg.h"

#ifndef _KERNEL_H
#define _KERNEL_H


//------------------------------------------------------------------------------
// kernel.c
/* Check if the compiler thinks we are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

//------------------------------------------------------------------------------
// vsprintf.c
int vsprintf(char * buf, const char * fmt, va_list args);

//------------------------------------------------------------------------------
// printk.c
int printk(const char * fmt, ...);

//------------------------------------------------------------------------------
// string.c
size_t strlen(const char* str); // 获取字符串长度

//------------------------------------------------------------------------------
// tty.h

size_t terminal_row; // 命令行行数
size_t terminal_column; // 当前命令行列数
uint8_t terminal_color; // 当前命令行颜色

volatile uint16_t * terminal_buffer;

// Hardware text mode color constants.

void terminal_init(void); // 命令行初始化

void terminal_setcolor(uint8_t color); // 设置命令行颜色

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y); // 在指定位置输出字符

void terminal_putchar(char c); // 在当前位置输出字符

void terminal_write(const char* data, size_t size); // 命令行写

void terminal_writestring(const char* data); // 命令行写字符串

void terminal_setcursorpos(size_t x, size_t y);	// 设置光标位置

uint16_t terminal_getcursorpos();	// 获取光标位置

void terminal_scroll();	// 滚动显示

//------------------------------------------------------------------------------
// vga.h

enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

#define VGA_ADDR 0x3D4	// CRT 控制寄存器-地址
#define VGA_DATA 0x3D5	// CRT 控制寄存器-数据
#define VGA_CURSOR_H	0xE	// 光标高位
#define VGA_CURSOR_L	0xF	// 光标低位
#define VGA_MEM_BASE	0xB8000	// VGA 缓存基址
#define VGA_MEM_SIZE	0x8000	// VGA 缓存大小

// 设置颜色，详解见 '颜色设置与位运算.md'
// fg-font
// bg-back
static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg);
static inline uint16_t vga_entry(unsigned char uc, uint8_t color);

// 规定显示行数、列数
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

//------------------------------------------------------------------------------
// port.h

static inline void outb(uint16_t port, uint8_t value);	// 端口写一个字节
static inline uint8_t inb(uint16_t port);	// 端口读一个字节
static inline uint16_t inw(uint16_t port);	// 端口读一个字

//------------------------------------------------------------------------------
// gdt.h

#define GDT_LENGTH 6	// 全局描述符表长度
// 各个内存段所在全局描述符表下标
#define SEG_NULL    0
#define SEG_KTEXT   1
#define SEG_KDATA   2
#define SEG_UTEXT   3
#define SEG_UDATA   4
#define SEG_TSS     5
#define GD_KTEXT    ((SEG_KTEXT) << 3)      // 内核代码段 0x08
#define GD_KDATA    ((SEG_KDATA) << 3)      // 内核数据段
#define GD_UTEXT    ((SEG_UTEXT) << 3)      // 用户代码段
#define GD_UDATA    ((SEG_UDATA) << 3)      // 用户数据段
#define GD_TSS      ((SEG_TSS) << 3)        // 任务段
// 段描述符 DPL
#define DPL_KERNEL  (0)	// 内核级
#define DPL_USER    (3)	// 用户级


// 各个段的全局描述符表的选择子
#define KERNEL_CS   ((GD_KTEXT) | DPL_KERNEL)
#define KERNEL_DS   ((GD_KDATA) | DPL_KERNEL)
#define USER_CS     ((GD_UTEXT) | DPL_USER)
#define USER_DS     ((GD_UDATA) | DPL_USER)

// 访问权限
#define KREAD_EXEC 0x9A
#define KREAD_WRITE 0x92
#define UREAD_EXEC 0xFA
#define UREAD_WRITE 0xF2

void gdt_init(void);	// 初始化全局描述符表
extern void gdt_load();	// GDT 加载到 GDTR 的函数
extern void tss_load();	// TSS 刷新[汇编实现]

//------------------------------------------------------------------------------
// intr.h

#define INTERRUPT_MAX 256	// 中断表最大值

// 定义IRQ
#define  IRQ0     32    // 电脑系统计时器
#define  IRQ1     33    // 键盘
#define  IRQ2     34    // 与 IRQ9 相接，MPU-401 MD 使用
#define  IRQ3     35    // 串口设备
#define  IRQ4     36    // 串口设备
#define  IRQ5     37    // 建议声卡使用
#define  IRQ6     38    // 软驱传输控制使用
#define  IRQ7     39    // 打印机传输控制使用
#define  IRQ8     40    // 即时时钟
#define  IRQ9     41    // 与 IRQ2 相接，可设定给其他硬件
#define  IRQ10    42    // 建议网卡使用
#define  IRQ11    43    // 建议 AGP 显卡使用
#define  IRQ12    44    // 接 PS/2 鼠标，也可设定给其他硬件
#define  IRQ13    45    // 协处理器使用
#define  IRQ14    46    // IDE0 传输控制使用
#define  IRQ15    47    // IDE1 传输控制使用





#endif