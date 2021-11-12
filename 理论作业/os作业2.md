2.7 实地址和虚地址的区别是什么？

答：实地址为CPU访问内存空间时用的的物理地址，凭借实地址可以直接在内存上进行读写操作；虚地址由页号和页中的偏移量组成。进程的每页都可以放在内存中的任何地方，虚拟地址可以通过某种变换规则映射到实地址上，虚地址的范围比实地址所表示的范围大

 

2.9解释单体内核和微内核的区别？

答：单体内核是将操作系统的全部功能都放入内核中，包括调度、文件系统、网络、设备驱动、存储管理等。所有元素都共享相同的地址空间

微内核是将操作系统中最核心的功能放入内核中，其他功能作为处于用户态的进程向外界提供服务。

 

2.1



|      | 时间周期   | 吞吐量     | 处理器利用率      |
| ---- | ---------- | ---------- | ----------------- |
| A1   | NT         | 1/N        | 50%               |
| A2   | (N + 1/2)T | 4/(2N+1)   | 1/(1 + 2N) * 100% |
| A4   | (N + 3/2)T | 8/(2N + 3) | 1/(3 + 2N) * 100% |
| B1   | NT         | 1/N        | 50%               |
| B2   | (N + 1/4)T | 8/(4N + 1) | 2/(4N + 1) * 100% |
| B4   | (N + 3/4)T | 16/(4N+3)  | 2/(4N + 3) * 100% |

 

2.4系统调用的目的是什么？系统调用与操作系统及模式（内核模式和用户模式）操作的概念是如何关联的？

系统调用被应用程序用来调用一个由操作系统提供的函数。

当操作系统接收到系统调用请求后，会让处理器进入内核模式，从而执行诸如I/O操作，修改基址寄存器内容等指令，而当处理完系统调用内容后，操作系统会让处理器返回用户模式，来执行用户代码。通常情况下，系统调用最终会转换成在内核模式下的系统程序

 

向Linux操作系统添加自己的系统调用并进行测试

1. 首先，下载Linux内核4.15.3

·终端输入命令

`wget https://mirrors.tuna.tsinghua.edu.cn/kernel/v4.x/linux-4.15.3.tar.xz `

2. 解压文件

`tar xvJf linux-4.15.3.tar.xz`

3. 修改syscall_64.tbl文件，添加333位系统调用号

`333    64    mycal        sys_mycall`

4. 进入linux-4.15.3/include/linux 修改syscall.h文件，加入

`asmlinkage long sys_mycall(struct timeval *tv);`

5. 在 linux-4.15.3/kernel/sys.c中添加相应服务

`SYSCALL_DEFINE1(mycall, struct timeval *, tv) {
    struct timeval ktv;
    do_gettimeofday(&ktv);
    if(copy_to_user(tv, &ktv, sizeof(ktv) ) ) {
        return -EFAULT;
    }
    return 0;`

6. 安装相关依赖

`sudo apt install libncurses5-dev libssl-dev build-essential openssl  zlibc minizip libidn11-dev libidn11 libelf-dev`

7. 开始编译，选择配置

8. 编译安装

`sudo make`

`sudo make modules_install`

`sudo make install`

9. 重启系统，测试新的系统调用，编写测试程序test.c

```c
#define _GNU_SOURCE
#include <unistd.h>
#include <sys/time.h>
#include <sys/syscall.h>
#include <stdio.h>
#define __NR_mycall 333 //系统调用号

int main() {
struct timeval gettime;
struct timeval mycalltime;
gettimeofday(&gettime, NULL);
syscall(__NR_mycall, &mycalltime);
printf("gettimeofday:%ld %ld \n", gettime.tv_sec, gettime.tv_usec);
printf("mycall : %ld %ld \n", mycalltime.tv_sec, mycalltime.tv_usec); 
return 0;
}
```

10. 编译运行

`gcc test.c`

`./a.out`

最后程序输出为

`gettimeofday；1544009432 742916`

`mycall : 1544009432 742919`