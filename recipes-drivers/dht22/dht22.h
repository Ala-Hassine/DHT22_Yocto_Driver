#ifndef _DHT22_H
#define _DHT22_H

#include <linux/ioctl.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#define DHT22_PIN           60
#define DHT22_DEVICE_NAME   "dht22"
#define DHT22_IOC_MAGIC     'D'
#define DHT22_READ_RAW_DATA _IOR(DHT22_IOC_MAGIC, 1, struct dht22_raw_data)

struct dht22_raw_data 
{
    __s32 temperature;
    __s32 humidity;
    __u8 checksum;
    __u8 data_bytes[5];
};

struct dht22_reading 
{
    float temperature;
    float humidity;
    int error_code;
};

static int dht22_open(struct inode *inode, struct file *file);
static int dht22_release(struct inode *inode, struct file *file);
static ssize_t dht22_read(struct file *file, char __user *user_buf, size_t len, loff_t *offset);
static int read_dht22(int *temperature, int *humidity);

static struct file_operations dht22_fops = 
{
    .owner = THIS_MODULE,
    .open = dht22_open,
    .release = dht22_release,
    .read = dht22_read,
};

MODULE_LICENSE("MIT");
MODULE_AUTHOR("Ala Hassine");
MODULE_DESCRIPTION("DHT22 Temperature & Humidity Sensor Driver For BeagleBone");
MODULE_VERSION("1.0");

#define DHT22_SUCCESS          0
#define DHT22_ERROR_TIMEOUT   -1
#define DHT22_ERROR_CHECKSUM  -2
#define DHT22_ERROR_GPIO      -3

#define DHT22_START_SIGNAL_US   2000
#define DHT22_RESPONSE_TIMEOUT  100
#define DHT22_BIT_TIMEOUT       100

#ifdef DEBUG
    #define DHT22_DEBUG(fmt, args...) printk(KERN_DEBUG "DHT22 : " fmt, ##args)
#else
    #define DHT22_DEBUG(fmt, args...) 
#endif

#define DHT22_INFO(fmt, args...) printk(KERN_INFO "DHT22 : " fmt, ##args)
#define DHT22_ERR(fmt, args...) printk(KERN_ERR "DHT22 : " fmt, ##args)

#endif /* _DHT22_H */
