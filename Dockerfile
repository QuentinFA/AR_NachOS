FROM debian

RUN echo "deb http://ftp.fr.debian.org/debian squeeze main" >> /etc/apt/sources.list
RUN echo "deb http://www.emdebian.org/debian/ squeeze main" >> /etc/apt/sources.list

RUN apt-get update
RUN apt-get install make
RUN apt-get -y --force-yes install gcc-4.4-mipsel-linux-gnu g++ g++-multilib
