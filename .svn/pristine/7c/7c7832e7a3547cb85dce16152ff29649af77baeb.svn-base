#include <linux/fs.h>                              /* open( ), read( ), write( ), close( ) 커널 함수 */
#include <linux/cdev.h>                          /* 문자 디바이스 */
#include <linux/module.h>
#include <linux/io.h>                              /* ioremap( ), iounmap( ) 커널 함수 */
#include <linux/gpio.h> 
#include <asm/uaccess.h>                    /* copy_to_user( ), copy_from_user( ) 커널 함수 */
#include <mach/platform.h>                  /* GPIO_BASE 매크로 */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("YoungJin Suh");
MODULE_DESCRIPTION("Raspberry Pi GPIO LED Device Module");

/* 장치 파일의 주번호와 부번호 */
#define GPIO_MAJOR 		200
#define GPIO_MINOR 		0
#define GPIO_DEVICE             "gpioled"              /* 디바이스 장치 파일의 이름 */
#define GPIO_LED                18                         /* LED 사용을 위한 GPIO의 번호 */

static char msg[BLOCK_SIZE] = {0};                      /* write( ) 함수에서 읽은 데이터 저장 */

/* 입출력 함수를 위한 선언 */
static int gpio_open(struct inode *, struct file *);
static ssize_t gpio_read(struct file *, char *, size_t, loff_t *);
static ssize_t gpio_write(struct file *, const char *, size_t, loff_t *);
static int gpio_close(struct inode *, struct file *);

/* 유닉스 입출력 함수들의 처리를 위한 구조체 */
static struct file_operations gpio_fops = {
   .owner = THIS_MODULE,
   .read = gpio_read,
   .write = gpio_write,
   .open = gpio_open,
   .release = gpio_close,
};

struct cdev gpio_cdev;   

static int gpio_open(struct inode *inod, struct file *fil)
{
    printk("GPIO Device opened(%d/%d)\n", imajor(inod), iminor(inod));

    return 0;
}

static int gpio_close(struct inode *inod, struct file *fil)
{
    printk("GPIO Device closed(%d)\n", MAJOR(fil->f_dentry->d_inode->i_rdev));

    return 0;
}

static ssize_t gpio_read(struct file *inode, char *buff, size_t len, loff_t *off)
{
    int count;

    strcat(msg, " from Kernel");
    count = copy_to_user(buff, msg, strlen(msg)+1);       /* 유저 영역으로 데이터를 보낸다 */

    printk("GPIO Device(%d) read : %s(%d)\n", 
                            MAJOR(inode->f_dentry->d_inode->i_rdev), msg, count);

    return count;
}

static ssize_t gpio_write(struct file *inode, const char *buff, size_t len, loff_t *off)
{
    short count;
    memset(msg, 0, BLOCK_SIZE);
    count = copy_from_user(msg, buff, len);        /* 유저 영역으로 부터 데이터를 가져온다. */

    /*  LED를 설정한다. */
    gpio_set_value(GPIO_LED, (!strcmp(msg, "0"))?0:1);

    printk("GPIO Device(%d) write : %s(%d)\n", 
                             MAJOR(inode->f_dentry->d_inode->i_rdev), msg, len);

    return count;
}

int initModule(void)
{
    dev_t devno;
    unsigned int count;
   // static void *map;                                   /* I/O 접근을 위한 변수 */
    int err;

    printk(KERN_INFO "Hello module!\n");

    try_module_get(THIS_MODULE);

    /* 문자 디바이스를 등록한다. */
    devno = MKDEV(GPIO_MAJOR, GPIO_MINOR);
    register_chrdev_region(devno, 1, GPIO_DEVICE);

    /* 문자 디바이스를 위한 구조체를 초기화한다. */
    cdev_init(&gpio_cdev, &gpio_fops);

    gpio_cdev.owner = THIS_MODULE;
    count = 1;
    err = cdev_add(&gpio_cdev, devno, count);               /* 문자 디바이스를 추가한다. */
    if (err < 0) {
        printk("Error : Device Add\n");
        return -1;
    }

    printk("'mknod /dev/%s c %d 0'\n", GPIO_DEVICE, GPIO_MAJOR);
    printk("'chmod 666 /dev/%s'\n", GPIO_DEVICE);

    /* GPIO 사용을 요청한다. */
    gpio_request(GPIO_LED, "LED");  
    gpio_direction_output(GPIO_LED, 0);

    return 0;
}

void cleanupModule(void)
{
    dev_t devno = MKDEV(GPIO_MAJOR, GPIO_MINOR);
    unregister_chrdev_region(devno, 1);       /* 문자 디바이스의 등록을 해제한다. */

    cdev_del(&gpio_cdev);                                /* 문자 디바이스의 구조체를 해제한다. */

    /* 더 이상 사용이 필요없는 경우 관련 자원을 해제한다. */
    gpio_free(GPIO_LED);

    module_put(THIS_MODULE);

    printk(KERN_INFO "Good-bye module!\n");
}

module_init(initModule);
module_exit(cleanupModule);
