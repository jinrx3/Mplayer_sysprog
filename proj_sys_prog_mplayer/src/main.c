#include "main.h"

int lcd_fd;				//lcd设备文件描述符
int x, y, px, py;		//坐标和坐标偏移

int main(int argc, char *argv[])
{	
	//屏保界面
	jpeg_show("./jpg/4.jpg", 0 ,0);
	
	open_lcd();	//	打开lcd设备
	open_ts();	//	打开触摸屏
	Init_Font();//  初始化字体库
	
	get_xy(&x,&y,&px,&py);
	printf("(%d, %d)\n", px, py);
	
	if(x>10 || y<-10)
	{
		jpeg_show("./jpg/2.jpg", 0, 0);
		//进入视频目录界面
		video_dir(argv[1]);
	}	
				
	UnInit_Font();	//  关闭字体库
	close_ts();     //	关闭触摸屏
	close_lcd();	//	关闭lcd设备
	return 0;
}