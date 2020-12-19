#include "jpeg_show.h"

//全局变量
extern int *lcd_p; 
int lcd_fd;
/*
 * Here's the routine that will replace the standard error_exit method:
 */
//错误处理函数
void my_error_exit (j_common_ptr cinfo)
{
  /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
  my_error_ptr myerr = (my_error_ptr) cinfo->err;

  /* Always display the message. */
  /* We could postpone this until after returning, if we chose. */
  (*cinfo->err->output_message) (cinfo);

  /* Return control to the setjmp point */
  longjmp(myerr->setjmp_buffer, 1);
}

 
/*
 * Sample routine for JPEG decompression.  We assume that the source file name
 * is passed in.  We want to return 1 on success, 0 on error.
 */

//read_JPEG_file   读取jpeg 文件，转换为 RGB 数据 
//GLOBAL   声明为外部变量 

int read_JPEG_file (char * filename,int px, int py)
{
  //1.定义一个解码结构体 
  struct jpeg_decompress_struct cinfo;

 //2.定义错误信息结构体
  struct my_error_mgr jerr;
  /* More stuff */
  FILE * infile;		/*源文件*/
  JSAMPARRAY buffer;		/* Output row buffer  输出行缓存 */
  int row_stride;		/*输出的行宽度physical row width in output buffer */


   //打开一个文件jpeg 文件
  if ((infile = fopen(filename, "rb")) == NULL) {
    fprintf(stderr, "can't open %s\n", filename);
    return 0;
  }

  /* Step 1: 初始化JPEG 解码对象 */

  /* We set up the normal JPEG error routines, then override error_exit. */
  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = my_error_exit;
  /* Establish the setjmp return context for my_error_exit to use. */
  if (setjmp(jerr.setjmp_buffer)) {
    /* If we get here, the JPEG code has signaled an error.
     * We need to clean up the JPEG object, close the input file, and return.
     */
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);
    return 0;
  }
  /*初始化解码对象*/
  jpeg_create_decompress(&cinfo);

  /* Step 2: 关联解码源文件 */

  jpeg_stdio_src(&cinfo, infile);

  /* Step 3: 读取JPEG 图片的头数据 */

  (void) jpeg_read_header(&cinfo, TRUE);
	  

  /* Step 5: 开始解码 */
  (void) jpeg_start_decompress(&cinfo);
 
		//输出图像的 信息
	  printf("图像的高度 %d\n",cinfo.output_height);
      printf("图像的宽度 %d\n",cinfo.output_width);
      printf("图像的位深 %d\n",cinfo.output_components);
 
  /* JSAMPLEs per row in output buffer */
  //求出一行R G B 数据所占用的空间大小
  row_stride = cinfo.output_width * cinfo.output_components;
  /* Make a one-row-high sample array that will go away when done with image */
  //根据行宽分配堆空间
  buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);


  

  //重点！！！！！！！循环一行一行的把 jpeg 数据转换成  RGB 数据
  while (cinfo.output_scanline < cinfo.output_height) {
  
			//读取一行数据进行转换
    (void) jpeg_read_scanlines(&cinfo, buffer, 1);
	
	char  *p  = buffer[0]; 
	
    //输出一行 R G B 数据  ，存放 buffer[0] 中
	int x=0;
	for(x=0;x<cinfo.output_width;x++)
	{
		 
		
		char  r =  *p++;  
		char  g =  *p++;
		char  b =  *p++; 
		
		int color  =  r << 16 | g << 8 | b; 
		if(x+px < 800 && cinfo.output_scanline+py < 480) //限制显示的范围
		{
			*(lcd_p + x + px +(py+cinfo.output_scanline)*800) = color; 
		}
	}
  
  }


  /* Step 7: Finish decompression */
  (void) jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);


  fclose(infile);


  return 1;
}

void rate_run(unsigned  int *lcd,int w,int h,int color)
{
	int x=0,y=0;  
		 
	for(y=0;y<h;y++)
	for(x=0;x<w;x++)
	{
		*(lcd  + x+100 + y*800) = color;
	}
}

void rate_black(unsigned  int *lcd,int w,int h,int color)
{
	int x=0,y=0;  
		 
	for(y=0;y<h;y++)
	for(x=w;x>0;x--)
	{
		*(lcd  + 700-x + y*800) = color;
	}
}

int jpeg_show(char *pathname, int x, int y)
{
	
	//1.打开LCD 设备 
	open_lcd();
		
	//2.对LCD 设备进行映射 
	lcd_p  = mmap(NULL,800*480*4,PROT_READ|PROT_WRITE,MAP_SHARED,lcd_fd,0); 
		 if(lcd_p == MAP_FAILED)
		 {
			 perror("");
			 return 0;
		 }
	//调用JPEG 官方的解码接口 
	read_JPEG_file(pathname, x, y);
	
	munmap(lcd_p,800*480*4);
	close_lcd();
	
	return 0;
}