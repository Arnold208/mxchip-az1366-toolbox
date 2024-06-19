#!/bin/bash

# Get the absolute path of the script's directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Define directories and file names
PROJECT_DIR="$SCRIPT_DIR/../MXChip/AZ3166/tools"
SECOND_SCRIPT="rebuild.sh"
BIN_FILE_DIR="$SCRIPT_DIR/../MXChip/AZ3166/build/app"
BIN_FILE_NAME="mxchip_azure_iot.bin"
MOUNT_POINT="/media/$USER/AZ3166"

# Check if project directory exists
if [ ! -d "$PROJECT_DIR" ]; then
  echo "Project directory $PROJECT_DIR does not exist"
  exit 1
fi

# Navigate to the project directory
cd "$PROJECT_DIR" || { echo "Failed to navigate to project directory"; exit 1; }

# Run the second script
./"$SECOND_SCRIPT"
if [ $? -ne 0 ]; then
  echo "Second script execution failed"
  exit 1
fi

# Navigate back to the script directory
cd "$SCRIPT_DIR"

# Check if bin file directory exists
if [ ! -d "$BIN_FILE_DIR" ]; then
  echo "Bin file directory $BIN_FILE_DIR does not exist"
  exit 1
fi

# Navigate to the directory containing the .bin file
cd "$BIN_FILE_DIR" || { echo "Failed to navigate to bin file directory"; exit 1; }

# Check if the .bin file exists
if [ ! -f "$BIN_FILE_NAME" ]; then
  echo ".bin file not found"
  exit 1
fi

# Check if the mount point exists and is accessible
if [ ! -d "$MOUNT_POINT" ]; then
  echo "Mount point $MOUNT_POINT does not exist"
  exit 1
fi

if [ ! -w "$MOUNT_POINT" ]; then
  echo "Mount point $MOUNT_POINT is not writable"
  exit 1
fi

# Copy the .bin file to the mounted drive
cp "$BIN_FILE_NAME" "$MOUNT_POINT"
if [ $? -eq 0 ]; then
  echo "Upload successful"
else
  echo "Upload failed"
  exit 1
fi
