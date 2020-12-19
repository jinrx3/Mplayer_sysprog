#ifndef __LIST_DATA_H__
#define __LIST_DATA_H__

#include <stdio.h>       
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

// 链表节点结构体（双向循环链表）
typedef struct dc_list{
	// 1.数据域
	char pathname[1024];
	char jpgname[1024];
	
	// 2.指针域
	struct dc_list *next;	//下节点地址
	struct dc_list *prev;	//上节点地址
}node_st, *node_pt;

// struct ss_list a		--> node_st a	(结构体变量定义)
// struct ss_list *p 	--> node_pt p	(结构体指针定义)


// 判断是否为空链表(只有头节点，没有数据节点)
bool is_empty(node_pt head);
// 显示链表所有数据（向后遍历）
void link_list_show_next(node_pt head);
// 显示链表所有数据（向前遍历）
void link_list_show_prev(node_pt head);
// 删除链表中的数据（包括所有相同数据）
bool link_list_del(node_pt head, node_pt del);
// 添加数据到链表头
void link_list_add_head(node_pt head, node_pt new);
// 添加数据到链表尾
void link_list_add_tail(node_pt head, node_pt new);
// 初始化单向链表
node_pt link_list_init(void);
//释放链表占用的全部堆空间
void link_list_exit(node_pt head);
//循环打印 单向循环链表 的数据
void sc_list_loop(node_pt head);

#endif