/*
    传入一个源目录，一个目的目录，将源目录下所有文件都复制到目的目录下
*/
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<dirent.h>
#include<fcntl.h>
#include<sys/stat.h>
#define BUFFER_SIZE 256
void copy_file(char *file1,char *file2)
{
    int fd1, fd2; 						/* 定义一个文件描述符 */
  int cntr, cntw;
  char buffer[BUFFER_SIZE]; 			/* 定义一个缓冲数组*/
  char *ptr;
  fd1 = open(file1, O_RDONLY); 		/* 打开文件，返回文件描述符 */
  if (fd1 == -1)						/* 如果描述符为-1,打开失败*/
  {
    perror("open file1 failed ");
    exit(1);
  }
  fd2 = open(file2, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);		/* 打开文件，如果没有就创建一个新文件 */
  if (fd2 == -1) 						/* 打开失败 */
  {
    perror("open file2 failed ");
    exit(1);
  }
  while((cntr = read(fd1, buffer, BUFFER_SIZE)) != 0) 	/* 按缓冲区大小读取文件 */
  {
    if(cntr == -1)					/* 如果读取失败，输出错误 */
    {
      perror("read error");
      break;
    }
    else if(cntr > 0)					/* 如果读取成功，赋给目的文件 */
    {
      ptr = buffer;
      while((cntw = write(fd2, ptr, cntr)) != 0)			/* 写入目的文件 */
      {
        if(cntw == -1) 				/* 写入失败*/
        {
          perror("write error");
          break;
        }
        else if(cntw == cntr) 			/* 写入的字节数与读取的字节数是否相等 */
          break;
        else if(cntw > 0) 				/* 如果为写完，则继续使用写入操作 */
        {
          ptr += cntw;				/* 重新设置写入和读取字节数 */
          cntr -= cntw;
        }
      }
      if(cntw == -1) 				/* 如果写入错误就退出 */
        break;
    }
  }
  close(fd1); 						/* 关闭文件 */
  close(fd2);
}
void copydir(char *argv1,char *argv2)
{
    DIR *dp;
    struct dirent *dirp;
    dp = opendir(argv1);					/* 打开目录 */
    if(dp == NULL) 						/* 如果目录无效，则终止*/
    {
        perror("opendir error");
        exit(1);
    }

    while((dirp = readdir(dp)) != NULL) 		/* 如果没有文件 */
    {
        if((strcmp(dirp->d_name, ".") == 0) || (strcmp(dirp->d_name, "..") == 0))	/* 防止死循环，将父目录和目录屏蔽 */
            continue;

        if(dirp->d_type == DT_DIR) 			/* 如果是目录 */
        {
            chdir(argv1);
            chdir(dirp->d_name);
            char temp_dir1[256]="0";
            getcwd(temp_dir1,256);
            chdir(argv2);
            mkdir(dirp->d_name,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
            chdir(dirp->d_name);
            char temp_dir2[256]="0";
            getcwd(temp_dir2,256);
            copydir(temp_dir1,temp_dir2);
        }
        else								/* 如果是文件直接输出 */
        {
            char file1[256],file2[256];
            char *temp = dirp->d_name;
            chdir(argv1);
            getcwd(file1,256);
            strcat(file1,"/");
            strcat(file1,temp);
            chdir(argv2);
            getcwd(file2,256);
            strcat(file2,"/");
            strcat(file2,temp);
            copy_file(file1,file2);

        }
    }
    closedir(dp);
}
int main(int argc, char **argv)
{
  if(argc != 3) 							/* 如果参数小于2,则提示输入两个参数 */
  {
    printf("too few arguments\n");
    exit(1);
  }
  copydir(argv[1],argv[2]);
 char *testdir1 = "/home/yangchunsk/workspace/ydm/10/log";
 char *testdir2 = "/home/yangchunsk/workspace/ydm/10/txt";
    copydir(testdir1,testdir2);


  						/* 输出目录下所有文件，包含子目录的文件 */
  return 0;
}
