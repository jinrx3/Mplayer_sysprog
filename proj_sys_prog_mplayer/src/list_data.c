#include "list_data.h"


//初始化双向链表
node_pt link_list_init(void)
{
	// a.给头节点分配堆空间
	node_pt mylist = malloc(sizeof(node_st));
	if(mylist == NULL)
	{
		perror("malloc error!");
		return NULL;
	}
	// b.使next指向头节点本身，prev指向本身
	mylist->next = mylist;
	mylist->prev = mylist;
	
	// c.返回头节点地址
	return mylist;
}

//释放链表占用的全部堆空间
void link_list_exit(node_pt head)
{
	node_pt pos,temp;
	while(pos == head)
	{
		temp = pos->next;
		free(pos);
		pos = temp;
	}
	free(head);
}


// 添加数据到链表头
void link_list_add_head(node_pt head, node_pt new)
{
	// 修改前后指向
		// a.操作新节点（使其prev前指上一个节点，next后指下一节点）
		// （顺序可调换）
	new->next = head->next;		//头节点->next
	new->prev = head->next->prev;//首节点->prev
	
	
		// b.操作前后节点（后节点的prev和前节点next，都指向新节点）
		// （顺序不可调换，必须先操作后节点的prev）
	head->next->prev = new;
	head->next = new;
}


// 添加数据到链表尾
void link_list_add_tail(node_pt head, node_pt new)
{	
	// 2.修改前后指向
		// a.操作新节点。（前指尾节点，后指头节点）
		// （顺序可随意交换）
	//new_node->next = 尾节点->next;
	//new_node->prev = 头节点->prev;
	new->next = head->prev->next;
	new->prev = head->prev;
	
		// b.操作前后节点。（让他们都指向新节点）
		// （顺序不可以随意交换）
	//尾节点->next = new_node;
	//头节点->prev = new_node;
	head->prev->next = new;
	head->prev = new;
	
}


// 判断是否为空链表(只有头节点，没有数据节点)
bool is_empty(node_pt head)
{
	return head->next==head || head->prev==head;
}

// 删除链表中的数据
bool link_list_del(node_pt head, node_pt del)
{
	// 0.判断是否为空链表
	if(is_empty(head))
	{
		printf("Empty!!\n");
		return false;
	}
	// 1.遍历链表
		// 找到了，记录下标，跳出循环
		// 没找到，提示用户，并返回
	node_pt pos;
	for(pos=head->next; pos!=head; pos=pos->next)
	{
		if(pos == del)
			break;
	}
	if(pos == head)
	{
		printf("not found!\n");
		return false;
	}
	// 2.修改前后节点指向（顺序可调换）
		// 2.1 操作前节点next，使其指向后节点
	//前节点->next = 后节点;
	pos->prev->next = pos->next;
		
		// 2.2 操作后节点prev，使其指向前节点
	//后节点->prev = 前节点;
	pos->next->prev = pos->prev;
	
	// 3.释放pos所在节点堆空间
	free(pos);
	return true;
}

// 显示链表所有数据（向后遍历）
void link_list_show_next(node_pt head)
{
	//与单向循环链表一模一样
	// 遍历链表
	node_pt pos;
	for(pos=head->next; pos!=head; pos=pos->next)
	{
		printf("%s\n", pos->pathname);
		printf("%s\n", pos->jpgname);
	}
}

// 显示链表所有数据（向前遍历）
void link_list_show_prev(node_pt head)
{
	node_pt pos;
	for(pos=head->prev; pos!=head; pos=pos->prev)
	{
		printf("%s\n ", pos->pathname);
	}
}




















