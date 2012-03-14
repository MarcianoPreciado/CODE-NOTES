
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<unistd.h>
#include	<fcntl.h>
#include	<sys/mman.h>
#include	<sys/stat.h>

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:
 * =====================================================================================
 */
int
main ( int argc, char *argv[] )
{
        int fdin, fdout;
        void *src, *dst;

        struct stat statbuf;

        if ((fdin = open(argv[1], O_RDONLY)) < 0) {
                perror("fuck read");
        }
        if ((fdout = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0644)) < 0) {
                perror("argv2 error");
        }

        if (fstat(fdin, &statbuf) < 0) {
                perror("fstat error");
        }

        lseek(fdout, statbuf.st_size - 1, SEEK_SET); /* 也可用ftruncate来设置输出文件的长度。*/
        write(fdout, "", 1);
        /* 防止对相关存储区第一次引用时产生SIGBUS信号*/

        if ((src = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fdin, 0)) < 0) {
                perror("mmap fdin fucking error");
        }

        if ((dst = mmap(0, statbuf.st_size, PROT_WRITE, MAP_SHARED, fdout, 0)) < 0) {
                perror("mmap fdout error");
        }

        memcpy(dst, src, statbuf.st_size);
        msync(dst, statbuf.st_size, MS_SYNC);
        if ( munmap(src, statbuf.st_size) < 0) {
                perror("munmap error");
        }
        if ( munmap(src, statbuf.st_size) < 0) {
                perror("munmap error");
        }

        return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
