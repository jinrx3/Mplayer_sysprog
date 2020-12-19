#ifndef __VIDEO_H__
#define __VIDEO_H__

#include <stdio.h>       
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

#include "list_data.h"
#include "jpeg_show.h"

#include "data_search.h"
#include "ts.h"
#include "font.h"
#include "lcd.h"
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>


void func_touch(node_pt temp,node_pt head);//视频操作函数

void *write_mp(void *arg);//发送获取百分比命令

void *func_rate(void *arg);//视频进度条刷新

void video_play(node_pt temp,node_pt head);//视频播放函数

void video_dir(char *pathname);//视频目录界面




#endif