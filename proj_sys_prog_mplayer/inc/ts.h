#ifndef _TS_H
#define _TS_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>	//mmap
#include <stdbool.h>
#include <stdlib.h>

#include <linux/input.h>	//输入子系统头文件

//打开触摸屏
void open_ts(void);
//关闭触摸屏
void close_ts(void);
//坐标获取
void get_xy(int *x, int *y, int *px, int *py);


#endif
