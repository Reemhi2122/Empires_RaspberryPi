#!/bin/bash
echo "Installing libs for Stan Vogels and Melvin Rothers Empires!"
sudo apt update
sudo apt upgrade
sudo apt-get install libbullet.dev
git clone https://github.com/g-truc/glm.git
sudo apt-get install libgles2-mesa-dev -y
sudo apt-get install libxrandr-dev -y
echo "Installing required libs - done"

