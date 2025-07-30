# TIER4 Automotive HDR Camera - Reset Functionality

## Overview
Some of the funcionalities added to the original repo regards Reset MAX9295 Serializer, MAX9296 Deserializer and its power cycle, restore communication gmsl settings, restore camera configuration and configure camera response protocols. 

## Installation Instructions

Clean Previous Builds:
```bash
make -C /lib/modules/$(uname -r)/build M=$PWD clean
```
compile the Modules:
```bash
make -C /lib/modules/$(uname -r)/build M=$PWD modules
```
verify Compilation:
```bash
ls -la *.ko
# Should see: tier4-isx021.ko, tier4-max9295.ko, tier4-max9296.ko, tier4-fpga.ko
```

Install the Kernel Modules. First create Installation Directory:
```bash
sudo mkdir -p /lib/modules/$(uname -r)/extra/
```
Then copy Modules:
```bash
sudo cp *.ko /lib/modules/$(uname -r)/extra/
```
Update Module Dependencies:
```bash
sudo depmod -a
```

Load the Modules in dependency Order:
```bash
# Load dependencies first
sudo modprobe tier4_fpga
sudo modprobe tier4_max9296
sudo modprobe tier4_max9295

# Load main camera driver
sudo modprobe tier4_isx021
```

verify that the modules are Loaded:
```bash
lsmod | grep tier4
# Should show all tier4 modules loaded
```

Verify Camera Reset Functionality

Check if Camera is Detected:
```bash
sudo dmesg | grep -i "detected isx021"
# Should see: "Detected ISX021 sensor"
```

Find Camera Reset Interface:
```bash
find /sys -name "camera_reset" 2>/dev/null
# Should show: /sys/devices/platform/.../camera_reset
```

## Usage Commands:
Trigger Camera Reset:
```bash
sudo ~/tier4_automotive_hdr_camera/scripts/quick_camera_fix.sh

```
