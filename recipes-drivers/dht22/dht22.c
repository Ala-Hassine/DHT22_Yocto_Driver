#include "dht22.h"

static int major;
static struct class *dht22_class;
static struct cdev dht22_cdev;
static char buf[64];

static int dht22_open(struct inode *inode, struct file *file) 
{ 
    DHT22_DEBUG("Device Opened\n");
    return 0; 
}

static int dht22_release(struct inode *inode, struct file *file) 
{ 
    DHT22_DEBUG("Device Closed\n");
    return 0; 
}

static int read_dht22(int *temperature, int *humidity) 
{
    int bits[40] = {0}, i, j = 0, data[5] = {0};
    int checksum;
    
    gpio_direction_output(DHT22_PIN, 0);
    mdelay(2);
    gpio_direction_input(DHT22_PIN);
    udelay(40);
    
    while (gpio_get_value(DHT22_PIN) == 1 && j++ < 100) udelay(1);
    if (j >= 100) return DHT22_ERROR_TIMEOUT;
    
    j = 0;
    while (gpio_get_value(DHT22_PIN) == 0 && j++ < 100) udelay(1);
    if (j >= 100) return DHT22_ERROR_TIMEOUT;
    
    j = 0;
    while (gpio_get_value(DHT22_PIN) == 1 && j++ < 100) udelay(1);
    if (j >= 100) return DHT22_ERROR_TIMEOUT;
    
    for (i = 0; i < 40; i++) 
    {
        j = 0;
        while (gpio_get_value(DHT22_PIN) == 0 && j++ < 100) udelay(1);
        if (j >= 100) return DHT22_ERROR_TIMEOUT;
        
        j = 0;
        while (gpio_get_value(DHT22_PIN) == 1 && j++ < 100) udelay(1);
        bits[i] = (j > 40) ? 1 : 0;
    }
    
    for (i = 0; i < 5; i++) 
    {
        data[i] = 0;
        for (j = 0; j < 8; j++) 
        {
            data[i] <<= 1;
            data[i] |= bits[i * 8 + j];
        }
    }
    
    checksum = (data[0] + data[1] + data[2] + data[3]) & 0xFF;
    if (data[4] != checksum) 
    {
        DHT22_ERR("Checksum Error ... Calculated : %d, Received : %d\n", checksum, data[4]);
        return DHT22_ERROR_CHECKSUM;
    }
    
    *humidity = ((data[0] << 8) | data[1]);
    *temperature = ((data[2] & 0x7F) << 8) | data[3];
    if (data[2] & 0x80) 
    {
        *temperature = -(*temperature);
    }

    return DHT22_SUCCESS;
}

static ssize_t dht22_read(struct file *file, char __user *user_buf, size_t len, loff_t *offset) 
{
    int temp = 0, hum = 0;
    float temp_c, hum_p;
    int ret;
    
    ret = read_dht22(&temp, &hum);
    if (ret != DHT22_SUCCESS) 
    {
        snprintf(buf, sizeof(buf), "Error Reading DHT22 Sensor (Error: %d)\n", ret);
    } 
    else 
    {
        temp_c = temp / 10.0;
        hum_p = hum / 10.0;
        snprintf(buf, sizeof(buf), "Temperature: %.1f°C, Humidity: %.1f%%\n", temp_c, hum_p);
    }
    
    if (copy_to_user(user_buf, buf, strlen(buf))) 
        return -EFAULT;
    return strlen(buf);
}

static int __init dht22_init(void) 
{
    dev_t devno;
    int ret;
    
    if (gpio_request(DHT22_PIN, "dht22")) 
    {
        DHT22_ERR("Failed To Request GPIO %d\n", DHT22_PIN);
        return -EBUSY;
    }
    
    ret = alloc_chrdev_region(&devno, 0, 1, "dht22");
    if (ret < 0) 
    {
        DHT22_ERR("Failed To Allocate Device Number\n");
        gpio_free(DHT22_PIN);
        return ret;
    }
    
    major = MAJOR(devno);
    cdev_init(&dht22_cdev, &dht22_fops);
    dht22_cdev.owner = THIS_MODULE;
    
    if (cdev_add(&dht22_cdev, devno, 1)) 
    {
        DHT22_ERR("Failed To Add Character Device\n");
        unregister_chrdev_region(devno, 1);
        gpio_free(DHT22_PIN);
        return -1;
    }
    
    dht22_class = class_create("dht22");
    if (IS_ERR(dht22_class)) 
    {
        DHT22_ERR("Failed To Create Device Class\n");
        cdev_del(&dht22_cdev);
        unregister_chrdev_region(devno, 1);
        gpio_free(DHT22_PIN);
        return PTR_ERR(dht22_class);
    }
    
    device_create(dht22_class, NULL, devno, NULL, "dht22");
    DHT22_INFO("Driver Loaded ... Major=%d, GPIO=%d\n", major, DHT22_PIN);
    return 0;
}

static void __exit dht22_exit(void) 
{
    dev_t devno = MKDEV(major, 0);
    device_destroy(dht22_class, devno);
    class_destroy(dht22_class);
    cdev_del(&dht22_cdev);
    unregister_chrdev_region(devno, 1);
    gpio_free(DHT22_PIN);
    DHT22_INFO("Driver Unloaded\n");
}

module_init(dht22_init);
module_exit(dht22_exit);
