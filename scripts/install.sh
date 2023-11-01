#!/bin/sh

sudo cp dist/*.AppImage /usr/local/bin/dnd_installer
sudo cp dist/*.png /usr/local/share/icons
sudo cp dist/*.desktop /usr/share/applications
sudo chmod +x /usr/local/bin/dnd_installer
