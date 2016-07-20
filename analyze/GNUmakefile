# -*- mode: Makefile -*-

CC = g++

ROOT_INCLUDES = `root-config --cflags`
ROOT_LIBS = `root-config --glibs` -lGeom
RAT_INCLUDES = -I$(RATROOT)/include
RAT_LIBS = -L$(RATROOT)/lib -lRATEvent -lrat
LOCAL_INC = -I. 

CFLAGS = -g -fPIC
CFLAGS += $(ROOT_INCLUDES) $(RAT_INCLUDES) $(LOCAL_INC)
LDFLAGS += `root-config --ldflags` 
LDLIBS = $(RAT_LIBS) $(ROOT_LIBS) -lstdc++

all: analyzeRUT

analyzeRUT: analyzeRUT.cc
	$(CC) -o $@  $^ $(CFLAGS) $(LDLIBS) $(LDFLAGS)

clean: 
	rm analyzeRUT
