#ifndef __DATA_SEARCH_H__
#define __DATA_SEARCH_H__

#include <stdio.h>       
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

#include "list_data.h"

int file_search(char *pathname,node_pt mylist);//检索文件存入双向链表中

#endif