#include "ts.h"

//全局变量
static int ts_fd;	//触摸屏文件描述符

//打开触摸屏
void open_ts(void)
{
	// 1.打开触摸屏文件	
	ts_fd = open("/dev/input/event0", O_RDONLY);
	if(ts_fd == -1)
	{
		perror("open ts failed");
		exit(0);
	}
}

//关闭触摸屏
void close_ts(void)
{
	// 3.关闭触摸屏文件	
	close(ts_fd);
}

//坐标获取
void get_xy(int *x, int *y,int *px, int *py)
{
	int x1,x2,y1,y2;
	int flag = 0;
	struct input_event ts_buf;
	while(1)
	{
		bzero(&ts_buf, sizeof(ts_buf));
		read(ts_fd, &ts_buf, sizeof(ts_buf));
		if(ts_buf.type==EV_ABS)	//触摸屏事件类型
		{
			if(ts_buf.code==ABS_X)	// X轴编码
			{
				*px = (int)((float)ts_buf.value*0.78125);
			}
			else if(ts_buf.code==ABS_Y)	// Y轴编码
			{
				*py = (int)((float)ts_buf.value*0.8);
			}
		}
		else if(ts_buf.type==EV_KEY)
		{
			if(ts_buf.code==BTN_TOUCH && ts_buf.value==1 && flag==0)
			{
				x1 = *px;
				y1 = *py;
				flag =1;
			}
			else if(ts_buf.code==BTN_TOUCH && ts_buf.value==0)
			{
				x2 = *px;
				y2 = *py;
				break;
			}
		}
	}
	//按下和弹起坐标差值
	*x = x2 - x1;
	*y = y2 - y1;
	return;
}

