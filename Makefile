#
# Build system using GNU Make/GCC
#
# Copyright (C) 2014 KO Myung-Hun <komh@chollian.net>
#
# This program is free software. It comes without any warranty, to
# the extent permitted by applicable law. You can redistribute it
# and/or modify it under the terms of the Do What The Fuck You Want
# To Public License, Version 2, as published by Sam Hocevar. See
# http://www.wtfpl.net/ for more details.
#

.SUFFIXES : .c .cpp .o .exe

CC       := gcc
CXX      := g++
LD       := g++
CFLAGS   := -Wall
CXXFLAGS := -Wall
LDFLAGS  :=
LDLIBS   :=

DIRS := misc time signal system
SRCS := $(foreach d,$(DIRS),$(wildcard $(d)/*.c) $(wildcard $(d)/*.cpp))
DEPS := $(foreach s,$(SRCS),$(s:$(suffix $(s))=.d))
OBJS := $(DEPS:.d=.o)
EXES := $(addsuffix .exe,$(filter %-test,$(basename $(SRCS))))

%.d : %.c
	$(CC) $(CFLAGS) -MM -MP -MT "$(@:.d=.o) $@" -MF $@ $<

%.d : %.cpp
	$(CXX) $(CXXFLAGS) -MM -MP -MT "$(@:.d=.o) $@" -MF $@ $<

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%-test.exe : %-test.o %.o
	$(LD) $(LDFLAGS) -o $@ $^ $(LDLIBS)

.PHONY : all clean

all : $(EXES)

clean :
	rm -f $(DEPS:.d=.bak)
	rm -f $(DEPS)
	rm -f $(OBJS)
	rm -f $(EXES)

ifeq ($(filter %clean,$(MAKECMDGOALS)),)
-include $(DEPS)
endif

