#ifndef __LCD_H
#define __LCD_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>	//mmap
#include <stdlib.h>

// 打开LCD设备
void open_lcd(void);
// 关闭LCD设备
void close_lcd(void);

#endif
	