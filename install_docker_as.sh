#!/bin/bash
echo "Adding new key to install Docker..."
sudo apt-key adv --keyserver hkp://p80.pool.sks-keyservers.net:80 --recv-keys 58118E89F3A912897C070ADBF76221572C52609D

if [[ `lsb_release -rs` == "14.04" ]]
then
   echo "deb https://apt.dockerproject.org/repo ubuntu-trusty main" > /etc/apt/sources.list.d/docker.list
elif [[ `lsb_release -rs` == "15.04" ]]
then
   echo "deb https://apt.dockerproject.org/repo ubuntu-vivid main" > /etc/apt/sources.list.d/docker.list
elif [[ `lsb_release -rs` == "15.10" ]]
then
   echo "deb https://apt.dockerproject.org/repo ubuntu-wily main" > /etc/apt/sources.list.d/docker.list
fi

echo "Getting package list"
apt-get update
echo "Purge of the old depo (if existing)"
apt-get purge lxc-docker
apt-cache policy docker-engine

if [[ `lsb_release -rs` == "15.04" ]] || [[ `lsb_release -rs` == "14.04" ]] || [[ `lsb_release -rs` == "15.10" ]]
then
   echo "Installing special packages (14,04, 15.04 or 15.10)"
   sudo apt-get install linux-image-extra-$(uname -r)
fi

sudo apt-get update

echo "-------------------------------------"
echo "--------- Installing Docker ---------"
echo "-------------------------------------"

sudo apt-get install docker-engine

echo "Creating docker user group (allows to use docker commands without sudo)"
sudo usermod -aG docker `whoami`
unset DOCKER_HOST

echo "Starting Docker daemon"
sudo service docker start
sudo docker run hello-world

echo "Downloading the image for NachOS !"
docker pull quentinfa/as_ricm4_nachos

echo "Installation finished !"
echo "Logout and login to test : docker run hello-world"
echo "If it fails, run : sudo usermod -aG docker `whoami`"
echo ""
echo "Docker usage :"
echo "docker run -i -t quentinfa/as_ricm4_nachos -v *WORKING_DIR*:/mnt"
echo "Launches the docker container with your WORKING_DIR in /mnt"
