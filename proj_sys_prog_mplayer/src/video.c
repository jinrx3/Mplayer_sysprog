#include "video.h"

int lcd_fd;				//lcd设备文件描述符
int *lcd_p=NULL;
int fifo_fd;			//有名管道文件描述符
int x, y, px, py;		//坐标和坐标偏移
pthread_mutex_t lock1;	//线程锁
FILE *mp=NULL;
unsigned int volume=60;	


void func_touch(node_pt temp,node_pt head)//视频操作函数
{
	
	int flag=0;//播放状态
	int i=0;
	while(1)
	{
		get_xy(&x,&y,&px,&py);
		printf("(%d, %d)\n", px, py);
		if(px>375 && px<500 && py>400 && py<480)//暂停继续
		{
			if(flag==0)
			{
				flag=1;//暂停
				//system("echo \"pause\" > /IOT/rfifo");
				system("killall -STOP mplayer");
				printf("------暂停----------\n");
			}
			else
			{	
				flag=0;//播放
				//system("echo \"pause\" > /IOT/rfifo");
				system("killall -CONT mplayer");
				printf("------播放----------\n");
			}	
		}	
 		 if(px>200 && px<350 && py>400 && py<480)//上一个
		{
			//pthread_mutex_lock(&lock1);
			system("killall mplayer");
			jpeg_show("./jpg/3.jpg", 0, 0);
			temp=temp->prev;
			if(temp==head)
				temp=temp->prev;
			
			Clean_Area(375,  400,  400, 32, 0x161415); //往屏幕指定区域填充底色
			char new_name1[32]={0};
			strcpy(new_name1,temp->jpgname);
			strtok(new_name1,".");
			Display_characterX(375 ,400, (unsigned char*)new_name1, 0xFFFFFF, 2);
			
			char mp4_buf[1024]={0};
			sprintf(mp4_buf,"mplayer -quiet -slave -geometry 100:0 -zoom -x 600 -y 380 -input file=/IOT/rfifo '%s'", temp->pathname);
			mp=popen(mp4_buf,"r");
			printf("------上一个----------\n");
			//pthread_mutex_unlock(&lock1);
		}  
	 	 if(px>550 && px<700 && py>400 && py<480)//下一个
		{
			//pthread_mutex_lock(&lock1);
			system("killall mplayer");
			jpeg_show("./jpg/3.jpg", 0, 0);
			
			temp=temp->next;
			if(temp==head)
				temp=temp->next;
			
			Clean_Area(375,  400, 400, 32, 0x161415); //往屏幕指定区域填充底色
			char new_name2[32]={0};
			strcpy(new_name2,temp->jpgname);
			strtok(new_name2,".");
			Display_characterX(375 ,400, (unsigned char*)new_name2, 0xFFFFFF, 2);
			
			char mp4_buf[1024]={0};
			sprintf(mp4_buf,"mplayer -quiet -slave -geometry 100:0 -zoom -x 600 -y 380 -input file=/IOT/rfifo '%s'", temp->pathname);
			mp=popen(mp4_buf,"r");
			printf("------下一个----------\n");
			//pthread_mutex_unlock(&lock1);
		}  
		if(px>100 && px<700 && py>370 && py<400)//点动进度条
		{
			flag=0;//播放
			float percent=((px-100)*100)/600;
			char  cmdt[1024] = {0};  
			sprintf(cmdt,"seek %d 1\n",(int)percent);
			write(fifo_fd,cmdt,strlen(cmdt));
		}
		if(x<-20 && px>100 && px<700 && py>0 && py<360)//快退
		{
			flag=0;//播放
			char  cmd1[1024] = {"seek -5\n"};  
			write(fifo_fd,cmd1,strlen(cmd1));
			//system("echo \"seek -5\" > /IOT/rfifo");
			printf("------快退----------\n");
		}
		if(x>20 && px>100 && px<700 && py>0 && py<360)//快进
		{
			flag=0;//播放
			char  cmd2[1024] = {"seek +5\n"};  
			write(fifo_fd,cmd2,strlen(cmd2));
			//system("echo \"seek +5\" > /IOT/rfifo");
			printf("------快进----------\n");
		}
		if(y>20 && px>100 && px<700 && py>0 && py<360)//音量-
		{
			flag=0;//播放
			volume-=20;
			char  cmd3[1024] = {0}; 
			sprintf(cmd3,"volume %d 1\n",volume);
			write(fifo_fd,cmd3,strlen(cmd3));
			//system("echo \"volume -20\" > /IOT/rfifo");
			printf("------音量减----------\n");
		}
		if(y<-20 && px>100 && px<700 && py>0 && py<360)//音量+
		{
			flag=0;//播放
			volume+=20;
			char  cmd4[1024] = {0};
			sprintf(cmd4,"volume %d 1\n",volume);			
			write(fifo_fd,cmd4,strlen(cmd4));
			//system("echo \"volume +20\" > /IOT/rfifo");
			printf("------音量加----------\n");
		}
		if(px>0 && px<100 && py>0 && py<100)//退出
		{
			//char  cmd0[1024] = {"quit\n"};  
			//write(fifo_fd,cmd0,strlen(cmd0));
			system("killall mplayer");
			//system("echo \"quit\" > /IOT/rfifo");
			printf("------退出----------\n");
			
			return ;
		}			
	}
}

void *write_mp(void *arg)//发送获取百分比命令
{
	while(1)
	{
		char  cmd[1024] = {"get_percent_pos\n"};  
		write(fifo_fd,cmd,strlen(cmd));
		sleep(1);
	}
	pthread_exit(NULL);	
}
 
void *func_rate(void *arg)//视频进度条刷新
{
	//打开设备
	open_lcd();
	//对LCD 设备进行映射 
	lcd_p  = mmap(NULL,800*480*4,PROT_READ|PROT_WRITE,MAP_SHARED,lcd_fd,0); 
	if(lcd_p == MAP_FAILED)
	{
		perror("");
		return 0;
	}
	//定位到进度条处  
	unsigned  int  *lcd_q =  lcd_p + 380*800; //放到380 行处
	
	//读取当期的播放进度  
	int percent=0;// 打印出文件中的当前位置为整数百分比[0-100）。
	char length[1024];
	while(1)//获取文件中的当前位置为整数百分比[0-100）。
	{	
		bzero(length, 1024);
		pthread_mutex_lock(&lock1);
		fgets(length, 1024, mp);  
		if(sscanf(length,"ANS_PERCENT_POSITION=%d", &percent) == 1)
		{
			rate_run(lcd_q, percent*6, 20, 0xffffff);
			rate_black(lcd_q, (100-percent)*6, 20, 0x161415);
		}	
		pthread_mutex_unlock(&lock1);
		usleep(20);
	}
	pthread_exit(NULL);	
} 

void video_play(node_pt temp,node_pt head)//视频播放函数
{
	pthread_mutex_init(&lock1,NULL);
	//判断管道是否存在，创建管道
	if(access("/IOT/rfifo", F_OK))
	{
		mkfifo("/IOT/rfifo", 0777); 
	}
	//打开有名管道
	fifo_fd = open("/IOT/rfifo",O_RDWR);	
	if(fifo_fd < 0)
	{
	 perror("管道不存在\n");
	 exit(0);
	}
	jpeg_show("./jpg/3.jpg", 0, 0);
	
	//字符显示
	char new_name[32]={0};
	strcpy(new_name,temp->jpgname);
	strtok(new_name,".");
	Display_characterX(375, 400, (unsigned char*)new_name, 0xFFFFFF, 2);
	
	//播放视频
	char mp4_buf[1024]={0};
	sprintf(mp4_buf,"mplayer -quiet -slave -geometry 100:0 -zoom -x 600 -y 380 -input file=/IOT/rfifo '%s'", temp->pathname);
	//system(mp4_buf);
	mp=popen(mp4_buf,"r");
	
	pthread_t tid1,tid2;
	//创建两个线程
	pthread_create(&tid1, NULL, func_rate, NULL);	//进度条刷新
	pthread_create(&tid2, NULL, write_mp, NULL);	//进度条发送命令
	
	//视频操作函数
	func_touch(temp,head);
	
	pthread_cancel(tid1);
	pthread_cancel(tid2);
	//回收线程资源
	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);

}

void video_dir(char *pathname)//视频目录界面
{	
	node_pt mylist = link_list_init();
	file_search(pathname, mylist);
	printf("---------------------\n");
	
	node_pt pos;
	
	while(1)
	{
		//显示背景图和预览图
		jpeg_show("./jpg/2.jpg", 0, 0);
		int i=0;
		int j=0;
		for(pos=mylist->next,i=100,j=50; pos!=mylist; pos=pos->next,i+=220)
		{
			printf("%s\n", pos->jpgname);
			char jpg_name[1024]={0};
			sprintf(jpg_name,"%s/00000001.jpg", pos->jpgname);
		if(i>540)
		{	
			i=100;
			j=250;
		}	
			jpeg_show(jpg_name, i,j);
			
			char new_name[32]={0};
			strcpy(new_name,pos->jpgname);
			strtok(new_name,".");
			Display_characterX(i+15, j+160, (unsigned char*)new_name, 0xFFFFFF, 1);	
		}
		printf("预览图列表显示完毕！\n");
		
		//触摸屏操作
		get_xy(&x,&y,&px,&py);
		printf("(%d, %d)\n", px, py);
		if(px>100 && px<250 && py>50 && py<200)
		{
			//进入视频播放界面
			video_play(mylist->next,mylist);
		}
		if(px>320 && px<470 && py>50 && py<200)
		{
			//进入视频播放界面
			video_play(mylist->next->next,mylist);
		}
		if(px>540 && px<710 && py>50 && py<200)
		{
			//进入视频播放界面
			video_play(mylist->next->next->next,mylist);
		}
		if(px>100 && px<250 && py>250 && py<400)
		{
			//进入视频播放界面
			video_play(mylist->prev->prev->prev,mylist);
		}
		if(px>320 && px<470 && py>250 && py<400)
		{
			//进入视频播放界面
			video_play(mylist->prev->prev,mylist);
		}
		if(px>540 && px<710 && py>250 && py<400)
		{
			//进入视频播放界面
			video_play(mylist->prev,mylist);
		}	
		if(px>700 && px<800 && py>0 && py<100)
		{
			link_list_exit(mylist);
			printf("退出电影系统\n");
			exit(0);
		}		
	}
}