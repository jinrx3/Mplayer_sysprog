#include "data_search.h"

int file_search(char *pathname,node_pt mylist)
{
	
	//打开目录
	DIR *dp=opendir(pathname);
	if(dp==NULL)
	{
		perror("文件夹不存在\n");
		return -1;
	}
	
	//循环读取目录
	while(1)
	{
		struct dirent *p=readdir(dp);
		if(p == NULL)
		{
			printf("读取目录完毕\n");
			break;
		}
		if(!strcmp(".",p->d_name) || !strcmp("..",p->d_name))
			continue;
		
		if(p->d_type==DT_DIR)
		{
			char dir_buf[1024]={0};
			sprintf(dir_buf,"%s/%s",pathname,p->d_name);
			file_search(dir_buf, mylist);
		}
		if(p->d_type==DT_REG)
		{	
			//判断文件是否为.avi和.mp4
			int len = strlen(p->d_name);
			if((len>4 && p->d_name[len-4]=='.'
				 && p->d_name[len-3]=='m'
				 && p->d_name[len-2]=='p'
				 && p->d_name[len-1]=='4')||
				 (len>4 && p->d_name[len-4]=='.'
				 && p->d_name[len-3]=='a'
				 && p->d_name[len-2]=='v'
				 && p->d_name[len-1]=='i'))
			{
				node_pt new_buf= malloc(sizeof(node_st));
				sprintf(new_buf->pathname,"%s/%s",pathname,p->d_name);
				
				char jpeg_buf[1024]={0};
				strcpy(new_buf->jpgname,p->d_name);
				printf("%s \n",new_buf->jpgname);
				
				//生成预览图
				sprintf(jpeg_buf,"mplayer -zoom -x 150 -y 150 -ss 85 -noframedrop -nosound -vo jpeg:outdir=./%s  -frames 1 %s ",p->d_name,new_buf->pathname);
				system(jpeg_buf);
				//printf("文件%s \n",p->d_name);
				
				//对生成的预览图重命名，可以不用每次都指定outdir到不同路径
				bzero(jpeg_buf,1024);
				char rename[1024]={0};
				strtok(p->d_name, ".");
				strcpy(rename, p->d_name);
				sprintf(jpeg_buf, "mv 00000001.jpg  %s.jpg", rename);
				
				//插入链表
				link_list_add_tail(mylist, new_buf);
				//printf("文件%s \n",new_buf->pathname);
				
			}	
		}
	}
	closedir(dp);
}