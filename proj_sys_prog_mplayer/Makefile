CC		 	= arm-linux-gcc #默认情况下CC = gcc 
ELF 		= ./bin/main
SRC 		= $(wildcard ./src/*.c)
CONFIG 		= -I./inc -I/home/gec/armjpeg/include/ -L./lib -L/home/gec/armjpeg/lib/ -ljpeg -lfont 
									# -I 指定头文件所在的路径
									# -L 指定库文件的路径
									# -l 指定需要链接的库
OBJ			= $(patsubst %.c,%.o,$(SRC))


$(ELF):$(OBJ)
	$(CC) $^ -o $@ $(CONFIG) -lpthread

%.o:%.c
	$(CC) $< -o $@  -c $(CONFIG)

clean:
	$(RM) $(ELF) $(OBJ)