#
#  @file    makefile
#  @author  Ferenc Nemeth
#  @date    18 Sept 2018
#  @brief   Makefile for laser rangefinder.
# 
#           Copyright (c) 2018 Ferenc Nemeth - https://github.com/ferenc-nemeth/
# 

CC=g++
CFLAGS= -std=c++14 `pkg-config --cflags --libs opencv` -Wall -Wextra

laser: laser.cpp
	$(CC) $? $(CFLAGS) -o $@

clean:
	rm -f laser
