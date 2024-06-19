#!/bin/sh

# Function to check if a package is installed
is_package_installed() {
    dpkg-query -W -f='${Status}' $1 2>/dev/null | grep -c "ok installed"
}

# List of packages to check and install
packages="apt-transport-https ca-certificates gnupg software-properties-common wget gcc-arm-none-eabi ninja-build cmake"

# Update package lists
echo "Updating package lists..."
sudo apt-get update -qq

all_installed=true

for package in $packages; do
    if [ $(is_package_installed $package) -eq 0 ]; then
        all_installed=false
        break
    fi
done

if $all_installed; then
    echo "All required packages are already installed."
    exit 0
fi

echo "Installing missing packages..."

for package in $packages; do
    if [ $(is_package_installed $package) -eq 0 ]; then
        echo "Installing $package..."
        sudo apt-get install -y $package
    fi
done

echo "All required packages are now installed."
