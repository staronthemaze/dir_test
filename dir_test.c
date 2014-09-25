/*
    传入一个目录，输出目录下所有文件，包含子目录下的文件
*/
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<dirent.h>
int i=0;
void showdir(char *argv)
{
    DIR *dp;
    struct dirent *dirp;
   // struct stat *sb;
    dp = opendir(argv);					/* 打开目录 */
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
            i++;
            printf("%2d   ",i);
            printf("%s  <director>\n", dirp->d_name);
            chdir(argv);
            chdir(dirp->d_name);
            char temp_dir[256]="0";
            getcwd(temp_dir,256);

           // printf("%s\n",temp_dir);
            showdir(temp_dir);
        }
        else								/* 如果是文件直接输出 */
        {
            i++;
            printf("%2d   ",i);
            printf("%s\n", dirp->d_name);
        }
    }
    closedir(dp);
}
int main(int argc, char **argv)
{
  if(argc != 2) 							/* 如果参数小于2,则提示输入两个参数 */
  {
    printf("too few arguments\n");
    exit(1);
  }
 //char *testdir = "/home/yangchunsk/workspace/dir_test";
  showdir(argv[1]);
 // showdir(testdir);
  						/* 输出目录下所有文件，包含子目录的文件 */
  return 0;
}
