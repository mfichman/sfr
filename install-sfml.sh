#!/bin/sh
wget http://www.sfml-dev.org/files/SFML-2.3-linux-gcc-64-bit.tar.gz
tar -xzvf SFML-2.3-linux-gcc-64-bit.tar.gz
cd SFML-2.3 && cmake -G "Unix Makefiles" && make && make install
