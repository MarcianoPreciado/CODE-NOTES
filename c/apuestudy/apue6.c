/* 第6章：系统数据文件和信息
 */

//vipw命令允许管理员使用该命令编辑口令文件。

struct passwd {
        char *pw_name;          /* Username.  */
        char *pw_passwd;        /* Password.  */
        __uid_t pw_uid;         /* User ID.  */
        __gid_t pw_gid;         /* Group ID.  */
        char *pw_gecos;         /* Real name.  */
        char *pw_dir;           /* Home directory.  */
        char *pw_shell;         /* Shell program.  */
};

#include	<pwd.h>
struct passwd *getpwuid(uid_t uid);
struct passwd *getpwnam(const char *name);

struct passwd *getpwent(void);  /* 返回口令文件的下一项 */
void setpwent(void);            /* 反绕文件，最好在程序开始处调用一次，保护性措施。*/
void endpwent(void);            /* 使用getpwent查看完口令文件后，一定要调用endpwent关闭这些文件。 */



#include	<shadow.h>
struct spwd {
        char *sp_namp;          /* Login name.  */
        char *sp_pwdp;          /* Encrypted password.  */
        long int sp_lstchg;     /* Date of last change.  */
        long int sp_min;        /* Minimum number of days between changes.  */
        long int sp_max;        /* Maximum number of days between changes.  */
        long int sp_warn;       /* Number of days to warn user to change
				   the password.  */
        long int sp_inact;	/* Number of days the account may be
				   inactive.  */
        long int sp_expire;	/* Number of days since 1970-01-01 until
				   account expires.  */
        unsigned long int sp_flag;              /* Reserved.  */
};

struct spdw *getspnam(const char *name);
struct spdw *getspent(void);

void setspent(void);
void endspent(void);


struct group {
        char *gr_name;          /* Group name.  */
        char *gr_passwd;        /* Password.    */
        __gid_t gr_gid;         /* Group ID.    */
        char **gr_mem;          /* Member list. */
};
#include	<grp.h>

struct group *getgrgid(gid_t gid);
struct group *getgrnam(const char *name);

struct group *getgrent(void);
void setgrent(void);
void endgrent(void);

/* 为了获取和设置附加组ID，提供了以下几个函数
 */

#include	<unistd.h>
#include	<grp.h>
// getgroups将各附加组ID填写到数组grouplist中，元素最多为gidsetsize个。实际填写到数组中的附加组id数目由函数返回，如果出错返回-1。
// 作为一个特例，若gidsetseize为0，函数只返回附加组id数目，对数组grouplist不作修改。这样可以确定数组grouplist的长度。

int getgroups(int gidsetsize, gid_t grouplist[]);

int setgroups(int ngroups, const gid_t grouplist[]); /* 特权操作,ngroups的值不能大于NGROUP_MAX */
int initgroups(const char *username, gid_t basegid); /* 特权操作，login用到，这两个函数还需要进一步了解。 */

// freebsd中，阴影文件是/etc/master.passwd。

utmp文件记录当前登录进系统的各个用户；wtmp文件，跟踪各个登录和注销事件。last命令读wtmp文件并打印记录。


#include	<time.h>
time_t time(time_t *calptr);
int gettimeofday(struct timeval *restrict tp, void *restrict tzp); /* 总是返回0值，SUS中，tzp唯一合法值为NULL。 */

struct timeval {
        time_t tv_sec;
        long tv_usec;           /* 微秒 */
}

struct tm {
        int tm_sec;			/* Seconds.	[0-60] (1 leap second) */
        int tm_min;			/* Minutes.	[0-59] */
        int tm_hour;			/* Hours.	[0-23] */
        int tm_mday;			/* Day.		[1-31] */
        int tm_mon;			/* Month.	[0-11] */
        int tm_year;			/* Year	- 1900.  */
        int tm_wday;			/* Day of week.	[0-6] */
        int tm_yday;			/* Days in year.[0-365]	*/
        int tm_isdst;			/* DST.		[-1/0/1]*/

#ifdef	__USE_BSD
        long int tm_gmtoff;		/* Seconds east of UTC.  */
        __const char *tm_zone;	/* Timezone abbreviation.  */
#else
        long int __tm_gmtoff;		/* Seconds east of UTC.  */
        __const char *__tm_zone;	/* Timezone abbreviation.  */
#endif
};

struct tm *gmtime(const time_t *calptr); /* TZ,localtime */
struct tm *localtime(const time_t *calptr); /* UTC time */

time_t mktime(struct tm *tmptr); /* TZ,local time */

char *asctime(const struct tm *tmptr);
char *ctime(const time_t *calptr);              /* TZ,localtime */

size_t strftime(char *restrict buf, size_t maxsize,
                const char *restrict format,
                const struct tm *restrict tmptr); /* TZ,localtime */

