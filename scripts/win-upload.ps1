# Get the absolute path of the script's directory
$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path

# Define directories and file names
$projectDir = Join-Path $scriptDir "../MXChip/AZ3166/tools"
$secondScript = "rebuild.bat"
$binFileDir = Join-Path $scriptDir "MXChip/AZ3166/build/app"
$binFileName = "mxchip_azure_iot.bin"
$driveLabel = "AZ3166"

# Navigate to the project directory
try {
    Set-Location -Path $projectDir -ErrorAction Stop
} catch {
    Write-Host "Failed to navigate to project directory: $projectDir"
    exit 1
}

# Run the second script
& cmd /c $secondScript
if ($LASTEXITCODE -ne 0) {
    Write-Host "Second script execution failed"
    exit 1
}

# Navigate back to the script directory
Set-Location -Path $scriptDir

# Navigate to the directory containing the .bin file
try {
    Set-Location -Path $binFileDir -ErrorAction Stop
} catch {
    Write-Host "Failed to navigate to bin file directory: $binFileDir"
    exit 1
}

# Check if the .bin file exists
if (-not (Test-Path -Path $binFileName)) {
    Write-Host ".bin file not found"
    exit 1
}

# Find all USB drives with the specified label and a FAT file system
$usbDrivesWithLabel = Get-WmiObject Win32_Volume | Where-Object {
    $_.Label -eq $driveLabel -and $_.DriveType -eq 2 -and $_.FileSystem -eq "FAT"
}

if ($usbDrivesWithLabel.Count -eq 0) {
    Write-Host "No USB drives with label $driveLabel and FAT file system found"
    exit 1
}

foreach ($drive in $usbDrivesWithLabel) {
    $mountPath = $drive.DriveLetter + "\"

    # Check if the mount point is writable
    if (-not (Test-Path -Path $mountPath -PathType Container)) {
        Write-Host "Mount point $mountPath is not writable"
        continue
    }

    # Copy the .bin file to the mounted drive
    try {
        Copy-Item -Path $binFileName -Destination $mountPath -ErrorAction Stop
        Write-Host "Upload successful to $mountPath"
    } catch {
        Write-Host "Upload failed to $mountPath: $_"
    }
}
