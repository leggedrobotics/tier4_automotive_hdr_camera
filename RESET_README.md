# TIER4 Automotive HDR Camera - Reset Functionality

## Overview

This enhanced TIER4 camera driver provides comprehensive camera reset functionality for automotive HDR cameras with ISX021 sensors. The implementation includes a 9-step recovery process that can restore camera functionality from communication failures without requiring system reboots.

## Reset Process Details

### Hardware Reset (Steps 1-5):
1. **Stop Streaming**: Clean video pipeline shutdown
2. **Reset MAX9295 Serializer**: Camera-side GMSL chip reset
3. **Reset MAX9296 Deserializer**: Host-side GMSL chip reset
4. **Power Cycling**: Complete electrical reset (platform-dependent)
5. **Hardware Stabilization**: Extended wait for stable operation

### Software Reinitialization (Steps 6-9):
6. **GMSL Link Reestablishment**: Rebuild communication pipeline
7. **Camera Sensor Reinitialization**: Restore sensor configuration
8. **Response Mode Setup**: Configure camera response protocols
9. **Final Stabilization**: Ensure ready state for streaming

---

# Installation Instructions

### Clean Previous Builds:
```bash
make -C /lib/modules/$(uname -r)/build M=$PWD clean
```

### Compile the Modules:
```bash
make -C /lib/modules/$(uname -r)/build M=$PWD modules
```

### Verify Compilation:
```bash
ls -la *.ko
# Should see: tier4-isx021.ko, tier4-max9295.ko, tier4-max9296.ko, tier4-fpga.ko
```

## Step 3: Install the Kernel Modules

### Create Installation Directory:
```bash
sudo mkdir -p /lib/modules/$(uname -r)/extra/
```

### Copy Modules:
```bash
sudo cp *.ko /lib/modules/$(uname -r)/extra/
```

### Update Module Dependencies:
```bash
sudo depmod -a
```

## Step 4: Load the Modules

### Load Modules in Dependency Order:
```bash
# Load dependencies first
sudo modprobe tier4_fpga
sudo modprobe tier4_max9296
sudo modprobe tier4_max9295

# Load main camera driver
sudo modprobe tier4_isx021
```

### Verify Modules are Loaded:
```bash
lsmod | grep tier4
# Should show all tier4 modules loaded
```

## Step 5: Verify Camera Reset Functionality

### Check if Camera is Detected:
```bash
sudo dmesg | grep -i "detected isx021"
# Should see: "Detected ISX021 sensor"
```

### Find Camera Reset Interface:
```bash
find /sys -name "camera_reset" 2>/dev/null
# Should show: /sys/devices/platform/.../camera_reset
```

### Test Camera Reset Interface:
```bash
# Read usage information
cat /sys/devices/platform/3180000.i2c/i2c-2/i2c-30/30-001b/camera_reset

# Trigger camera reset. Main command!
echo 1 | sudo tee /sys/devices/platform/3180000.i2c/i2c-2/i2c-30/30-001b/camera_reset

# Check reset activity in kernel log
sudo dmesg | grep -i "camera.*reset" | tail -10
```

## Step 6: Test Camera Functionality

### Check Video Device:
```bash
ls -la /dev/video*
v4l2-ctl --device=/dev/video0 --info
```

---

### Usage Commands:

#### View Usage Information:
```bash
cat /sys/devices/platform/3180000.i2c/i2c-2/i2c-30/30-001b/camera_reset
```

#### Trigger Camera Reset:
```bash
echo 1 | sudo tee /sys/devices/platform/3180000.i2c/i2c-2/i2c-30/30-001b/camera_reset
```

#### Monitor Reset Progress:
```bash
sudo dmesg | grep -i "camera.*reset" | tail -20
```

