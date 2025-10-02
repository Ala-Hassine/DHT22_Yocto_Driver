# DHT22_Yocto_Driver
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

### 5. Modify Target Machine In conf/local.conf :
MACHINE ?= "beaglebone"

---



---



---