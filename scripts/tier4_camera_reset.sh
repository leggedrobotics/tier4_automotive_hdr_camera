#!/bin/bash

# Quick Camera Fix Script - Use this when camera stops working
# This script quickly reloads the enhanced camera modules

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

echo -e "${BLUE}🎥 TIER4 Camera Quick Fix${NC}"
echo "=================================="

# Check if running as root
if [[ $EUID -ne 0 ]]; then
    echo -e "${RED}❌ This script must be run with sudo${NC}"
    echo "Usage: sudo $0"
    exit 1
fi

# Define paths
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SOURCE_DIR="$(dirname "$SCRIPT_DIR")/src/tier4-camera-gmsl"

if [[ ! -d "$SOURCE_DIR" ]]; then
    echo -e "${RED}❌ Source directory not found: $SOURCE_DIR${NC}"
    exit 1
fi

echo -e "${YELLOW}🔄 Reloading camera modules...${NC}"

# Remove existing modules silently
rmmod tier4_imx728 tier4_imx490 2>/dev/null || true
rmmod tier4_isx021 2>/dev/null || true

# Load enhanced modules
cd "$SOURCE_DIR"
if insmod tier4-isx021.ko 2>/dev/null; then
    echo -e "${GREEN}✅ Enhanced tier4_isx021 module loaded${NC}"
else
    echo -e "${RED}❌ Failed to load tier4_isx021 module${NC}"
    echo "Try running the full reload script: sudo scripts/reload_camera_modules.sh"
    exit 1
fi

# Wait for initialization
sleep 2

# Check if reset interface is available
RESET_PATH=$(find /sys -name "camera_reset" 2>/dev/null | head -1)
if [[ -n "$RESET_PATH" ]]; then
    echo -e "${GREEN}✅ Reset interface available: $RESET_PATH${NC}"
    
    # Trigger a reset to ensure camera is working
    echo -e "${YELLOW}🔄 Triggering camera reset...${NC}"
    echo 1 > "$RESET_PATH" 2>/dev/null || true
    sleep 3
    
    echo -e "${GREEN}✅ Camera reset completed${NC}"
else
    echo -e "${YELLOW}⚠️  Reset interface not found yet${NC}"
fi

# Check video device
if [[ -e "/dev/video0" ]]; then
    echo -e "${GREEN}✅ Camera device available: /dev/video0${NC}"
else
    echo -e "${YELLOW}⚠️  Camera device not found yet${NC}"
fi

echo
echo -e "${BLUE}📋 Quick Status:${NC}"
lsmod | grep tier4_isx021 && echo -e "${GREEN}✅ Module loaded${NC}" || echo -e "${RED}❌ Module not loaded${NC}"

echo
echo -e "${GREEN}🎉 Quick fix completed!${NC}"
echo
echo -e "${BLUE}Next steps:${NC}"
echo "• Test camera: cd ~/uvgRTPWrapper/build/linux && ./example_sender"
echo "• Manual reset: echo 1 | sudo tee $RESET_PATH"
echo "• Full reload: sudo scripts/reload_camera_modules.sh"
