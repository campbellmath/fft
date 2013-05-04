###############################################################################
# compiler setting
###############################################################################
CC       = gcc
CXX      = g++
CFLAGS   = -g -Wall
CXXFLAGS = $(CFLAGS) -Weffc++ -pedantic-errors
LIBS     = -lm
INCPATH  =

###############################################################################
# source files setting
###############################################################################
ifeq ($(OS),Windows_NT)
C_SOURCES   =
CXX_SOURCES = FixedPoint.cpp main.cpp
else
C_SOURCES   = $(shell find . -name "*.c")
CXX_SOURCES = $(shell find . -name "*.cpp")
endif
C_OBJS      = $(patsubst %.c,%.o,$(wildcard $(C_SOURCES)))
CXX_OBJS    = $(patsubst %.cpp,%.o,$(wildcard $(CXX_SOURCES)))
OBJS        = $(C_OBJS) $(CXX_OBJS)
EXEC        = ifft snr

###############################################################################
.PHONY : clean clean_all
###############################################################################
all: $(EXEC)

%.o:%.c
	$(CC) -c $(CFLAGS) $(INCPATH) $< -o $@
%.o:%.cpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) $< -o $@

ifft: FixedPoint.o main.o
	$(CXX) FixedPoint.o main.o -o ifft  $(LIBS)

snr: snr.o
	$(CXX) snr.o -o snr  $(LIBS)
###############################################################################
clean:
	@rm -vfr $(OBJS)
clean_all: clean
	@rm -vfr $(EXEC)
###############################################################################
