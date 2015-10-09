#!/bin/sh
wget http://www.sfml-dev.org/files/SFML-2.3-linux-gcc-64-bit.tar.gz
tar -xzvf SFML-2.3-linux-gcc-64-bit.tar.gz
cp SFML-2.3/lib/* /usr/lib/x86_64-linux-gnu/
cp -r SFML-2.3/include/SFML /usr/include/
