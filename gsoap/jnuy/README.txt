gSOAP For SCO OpenServer 4.0.5安装指南
==========

系统环境
----------

1. 系统需要依次安装如下补丁：
RS505A，OSS646B,gwxlibs-1.3.1Ba,gnutools-5.0.7Kj补丁包
必须是root用户，用scoadmin software安装（见附件SCO4PATH.tar）

2. 在以上补丁包基础上编译安装gcc3.x.x以上版本。（附件有我编译并打包好的gcc 3.4.6，gcc-3.4.6-binary.tar 直接拷贝到SCO机子的 /usr/local目录下即可）
（开发机需要，部署到客户终端的话不需要）

配置环境变量为：

export PATH=/usr/local/gcc-3.4.6/bin:/usr/gnu/bin:/usr/bin:/bin:$HOME/bin
export LD_LIBRARY_PATH=/usr/local/gcc-3.4.6/lib:/usr/gnu/lib:/usr/lib:$HOME/lib

将以上两行添加进用户目录下的.profile文件 ~/.profile或/etc/profile文件

3. Makefile为示例Makefile文件，将其放入所建项目内。

4. 确保SCO可以正常联网，可ping 163.com测试


使用说明
----------

济南水业的客户端见附件中的code.tar

参照 gsoap官方文档
http://www.cs.fsu.edu/~engelen/soap.html
以及 jnuy.tar中代码文件夹的Makefile文件了解使用方法 (主要是wsdl2h和soapcpp2的命令使用, wsdl2h的编译需要依赖于g++3以上版本，但我无法在SCO机子上成功编译移植G++3，所以wsdl2h命令要在WIN/LINUX上使用并根据相应的wsdl文件生成相应的头文件，本例子中是生成jnuy.h)


jnuy.rar解压缩。
jnuy目录下 client.c soapList.h soapList.h为所编写实际代码，可优先参考，以粗略了解SOAP使用方法以及个别注意事项。
soapList链表为简单解析gsoap所返回XML数据。

其中 jnuy/soaptools文件夹提供了SOAP库以及所需工具soapcpp2和源码
以后其他类似SOAP项目也会需要这个文件夹，妥善保管。

jnuy/html/index.html 为文档说明，
主要是
jnuy/html/annotated.html
jnuy/html/soapList_8h.html


soapList所返回字符串均为UTF-8格式，以下为./client实际执行结果（用term  telnet到SCO主机所显示。term要设定编码为UTF-8）

...............................

测试hbwsh with: gs=1000008, id = 123, yyzd = 5-兴业银行
NewDataSet=NULL
NewDataSet:Error=NULL   parentname=NewDataSet
NewDataSet:Error:Source=App_Code        parentname=NewDataSet:Error
NewDataSet:Error:Message=此给水号不欠费！       parentname=NewDataSet:Error


测试hbwsh with: gs=1000005, id = 123, yyzd = 5-兴业银行
GET_HBWSH=NULL
GET_HBWSH:Gethbgs=NULL  parentname=GET_HBWSH
GET_HBWSH:Gethbgs:gs=1000005    parentname=GET_HBWSH:Gethbgs
GET_HBWSH:Gethbwsh=NULL parentname=GET_HBWSH
GET_HBWSH:Gethbwsh:id=123       parentname=GET_HBWSH:Gethbwsh
GET_HBWSH:Gethbwsh:gs=1000005   parentname=GET_HBWSH:Gethbwsh
GET_HBWSH:Gethbwsh:hm=张晓    parentname=GET_HBWSH:Gethbwsh
GET_HBWSH:Gethbwsh:mp=历下千佛山东2路11号楼1单元102室       parentname=GET_HBWSH:Gethbwsh
GET_HBWSH:Gethbwsh:sl=8 parentname=GET_HBWSH:Gethbwsh
GET_HBWSH:Gethbwsh:zje=25.2000  parentname=GET_HBWSH:Gethbwsh
GET_HBWSH:Gethbwsh:suo=1        parentname=GET_HBWSH:Gethbwsh
!$ uname -a
SCO_SV scosysv 3.2 5.0.5 i386 unknown unknown SCO Unix

..................
