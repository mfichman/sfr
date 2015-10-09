#!/bin/sh
wget http://www.sfml-dev.org/files/SFML-2.2-sources.zip
unzip SFML-2.2-sources.zip
cd SFML-2.2 && cmake -G "Unix Makefiles" && make && make install
