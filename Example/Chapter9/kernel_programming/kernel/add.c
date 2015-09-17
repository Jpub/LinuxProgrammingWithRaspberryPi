#include <linux/unistd.h> 
#include <linux/kernel.h> 
#include <asm/uaccess.h>

asmlinkage long sys_add(int a, int b, int *res) {                    
    long err = 0;
    int ret = 0;
   
    err = access_ok(VERIFY_WRITE, res, sizeof(int));  /* 유저 영역에 쓸 수 있는지 검사 */
    if(err == -EFAULT) return err;
   
    ret = a + b;
    put_user(ret, res);                                                      /* 유저 영역으로 값을 보낸다. */

    printk(KERN_INFO "%d + %d = %d from Linux Kernel\n", a, b, *res);
   
    return err;
}

