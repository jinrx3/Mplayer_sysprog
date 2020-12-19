#ifndef __JPEG_SHOW_H__
#define __JPEG_SHOW_H__

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include "jpeglib.h"
#include <setjmp.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include "lcd.h"
#include <fcntl.h>
#include <unistd.h>


//添加输入结构体模型 
#include <linux/input.h>

//错误信息结构体
typedef struct my_error_mgr {
  struct jpeg_error_mgr pub;	/* "public" fields */

  jmp_buf setjmp_buffer;	/* for return to caller */
}my_error_mgr, * my_error_ptr;
  

void my_error_exit (j_common_ptr cinfo);
void rate_run(unsigned  int *lcd,int w,int h,int color);//快进百分比刷新进度条
void rate_black(unsigned  int *lcd,int w,int h,int color);//快退百分比刷新进度条
int read_JPEG_file (char * filename,int px, int py);
int jpeg_show(char *pathname, int x, int y);//jpeg图片显示


#endif