#include "lcd.h"
// 全局变量
extern int lcd_fd;	//LCD设备文件描述符

// 打开LCD设备
void open_lcd(void)
{
	lcd_fd = open("/dev/fb0", O_RDWR);
	if(lcd_fd == -1)
	{
		perror("open lcd failed");
		exit(0);
	}
}

// 关闭LCD设备
void close_lcd(void)
{
	
	close(lcd_fd);
}