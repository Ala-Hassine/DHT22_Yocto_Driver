# 🌡️ DHT22 Yocto Driver

![Yocto](https://img.shields.io/badge/Yocto-Project-blue)
![Linux](https://img.shields.io/badge/Linux-Kernel_Driver-orange)
![BeagleBone](https://img.shields.io/badge/BeagleBone-Supported-green)
![License](https://img.shields.io/badge/License-MIT-yellow)

A complete Yocto-based embedded Linux solution for BeagleBone that includes kernel drivers and applications to read temperature and humidity data from DHT22 sensors.

## 🚀 Features

- ✅ **Linux Kernel Driver** - Character device interface (`/dev/dht22`)
- ✅ **Yocto Recipe** - Easy integration with Yocto Project
- ✅ **BeagleBone Support** - Optimized for BeagleBone devices
- ✅ **Real-time Monitoring** - Temperature & humidity readings
- ✅ **Production Ready** - Robust error handling and checksum verification

## 📋 Prerequisites

- 🖥️ Linux host machine (Ubuntu 20.04+ recommended)
- 💿 BeagleBone board (Black/Green/Wireless)
- 🌡️ DHT22 temperature/humidity sensor
- 🛠️ Basic Yocto knowledge

---

## 🛠️ Installation & Setup

### Step 1 : Setup Yocto Kirkstone Environment

```bash
# Create project directory
mkdir DHT22_Yocto_Driver
cd DHT22_Yocto_Driver
mkdir layers
cd layers

# Clone Poky (Kirkstone branch)
git clone -b kirkstone git://git.yoctoproject.org/poky.git
cd poky

# Clone required meta layers
git clone -b kirkstone git://git.yoctoproject.org/meta-openembedded.git
git clone -b kirkstone git://git.yoctoproject.org/meta-ti.git

# Initialize build environment
cd ~/DHT22_Yocto_Driver
source layers/poky/oe-init-build-env build/
cd ..

# Configure for BeagleBone
echo 'MACHINE = "beaglebone"' >> build/conf/local.conf
```

### Step 2 : Create Custom Layer for DHT22

```bash
# Create custom layer
bitbake-layers create-layer meta-dht22

# Add layer to build configuration
bitbake-layers add-layer ../meta-dht22
```

### Step 3 : Build DHT22 Driver

The layer includes:
- 📁 `recipes-kernel/dht22/dht22.c`  - Kernel Driver Source
- 📁 `recipes-kernel/dht22/dht22.h`  - Header File
- 📁 `recipes-kernel/dht22/Makefile` - Build Configuration
- 📁 `recipes-kernel/dht22.bb`       - BitBake Recipe

### Step 4 : Include Driver in Yocto Image

```bash
# Add driver to image
echo 'IMAGE_INSTALL:append = " dht22"' >> build/conf/local.conf

# Build the image
bitbake core-image-minimal
```

### Step 5: Flash & Test

```bash
# Flash to SD card
sudo dd if=tmp/deploy/images/beaglebone/core-image-minimal-beaglebone.wic of=/dev/sdX bs=1M status=progress

# Boot BeagleBone and test
cat /dev/dht22
# Output: Temperature: 25.5°C, Humidity: 45.2%
```

## 🔧 Hardware Configuration

### DHT22 Pinout Connection

| DHT22 Pin | BeagleBone Pin | Description |
|-----------|----------------|-------------|
| VCC       | P9_3 (3.3V)    | Power (3.3V) |
| DATA      | P8_12 (GPIO 60)| Data pin |
| GND       | P9_1 (GND)     | Ground |

## 📁 Project Structure

```
DHT22_Yocto_Driver/
├── 📁 layers/
│   ├── 📁 poky/
│   ├── 📁 meta-openembedded/
│   └── 📁 meta-ti/
├── 📁 meta-dht22/
│   └── 📁 recipes-kernel/       # Kernel Recipes Directory
│       ├── 🍳 dht22.bb          # BitBake recipe
│       └── 📁 dht22/            # Source files directory
│           ├── 🐧 dht22.c
│           ├── 📄 dht22.h
│           └── 🔧 Makefile
└── 📁 build/
```

## 🐛 Debugging

```bash
# Check if driver loaded
lsmod | grep dht22

# View kernel messages
dmesg | grep -i dht

# Manual module loading
insmod /lib/modules/$(uname -r)/extra/dht22.ko

# Check device node
ls -la /dev/dht22
```

## 🎯 Usage Examples

### Basic Reading
```bash
cat /dev/dht22
# Temperature: 23.1°C, Humidity: 55.7%
```

### Continuous Monitoring
```bash
watch -n 2 'cat /dev/dht22'
```

### Integration in Scripts
```bash
#!/bin/bash
READING=$(cat /dev/dht22)
echo "Sensor Data : $READING"
# Use in your applications
```

## 🔍 Technical Details

- **Driver Type**: Linux Character Device Driver
- **Interface**: `/dev/dht22` (read-only)
- **GPIO**: Pin 60 (configurable in dht22.h)
- **Update Rate**: ~2 seconds between readings
- **Accuracy**: ±0.5°C temperature, ±2% humidity

## 🤝 Contributing

We welcome contributions! Please feel free to submit pull requests, report bugs, or suggest new features.

1. 🍴 Fork the repository
2. 🌿 Create your feature branch (`git checkout -b feature/AlaFeature`)
3. 💾 Commit your changes (`git commit -m 'Add some AlaFeature'`)
4. 📤 Push to the branch (`git push origin feature/AlaFeature`)
5. 🔃 Open a Pull Request

## 📜 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- Yocto Project community
- BeagleBoard.org for hardware support
- Linux kernel documentation

---

## 📞 Support

If you encounter any issues or have questions:

1. 📖 Check the [Troubleshooting](#-debugging) section
2. 🐛 Open an [Issue](../../issues)
3. 💬 Start a [Discussion](../../discussions)

## 🌟 Star History

If you find this project useful, please give it a star! ⭐

---
