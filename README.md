# DHT22_Yocto_Driver

---

A Yocto-Based Embedded Linux Image For Beaglebone That Includes Drivers And Applications To Read Temperature And Humidity Data From A DHT22 Sensor.

---

## Step 1 : Setup Yocto Kirkstone Environment

### 1. Prepared Environment :
λ mkdir DHT22_Yocto_Driver
λ cd DHT22_Yocto_Driver
λ mkdir DHT22_Yocto_Driver/layers
λ cd DHT22_Yocto_Driver/layers

### 2. Clone Poky (Kirkstone Branch) :
λ git clone -b kirkstone git://git.yoctoproject.org/poky.git
λ cd poky

### 3. Clone Meta Layers :
λ git clone -b kirkstone git://git.yoctoproject.org/meta-openembedded.git
λ git clone -b kirkstone git://git.yoctoproject.org/meta-ti.git

### 4. Initialize The Build Environment :
λ cd ~/DHT22_Yocto_Driver
λ source layers/poky/oe-init-build-env build/
λ cd ..

### 5. Modify Target Machine In conf/local.conf :
MACHINE ?= "beaglebone"

---

## Step 2 : Create A Custom Layer For DHT22

### 1. Create Layer :
λ bitbake-layers create-layer meta-dht22

### 2. Add It To Your Build :
λ bitbake-layers add-layer ../meta-dht11

---

## Step 3 : Write The DHT22 Driver

### 1. Create DHT22 Drivers :
Go To Your Layer Directory : meta-dht22/recipes-drivers/dht22/ and Create DHT22 Drivers (.h/.c)

### 2. Create The BitBake Recipe :
Go To Your Layer Directory : meta-dht22/recipes-drivers/ and Create DHT22 BitBake Recipe (.bb)

### 3. Create The Makefile :
Go To Your Layer Directory : meta-dht22/recipes-drivers/dht22 and Create Makefile

---

## Step 4 : Include Driver In Yocto Image

### 1. Edit conf/local.conf :
IMAGE_INSTALL_append = " dht22"

### 2. Build The Image :
λ bitbake core-image-minimal

---

## Step 5 : Flash SD Card And Test

### 1. Flash .wic Image To SD Card.
### 2. Boot BeagleBone.
### 3. Check Temperature And Humidity

λ cat /dev/dht22
# Example Output : Temp:44C Hum:71%

---