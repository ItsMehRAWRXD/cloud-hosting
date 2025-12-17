#!/bin/bash

# Remove old and create fresh
rm -rf /c/cloud-hosting 2>/dev/null
mkdir -p /c/cloud-hosting
cd /c/cloud-hosting

# Init git
git init
git config user.name "RawrXD"
git config user.email "rawrxd@dev.local"

echo "[✓] Git repository initialized"
