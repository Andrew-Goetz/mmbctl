CC:=gcc
CFLAGS:=-O2 -g -Wall -Wextra -std=c99
LDLIBS:=-lddcutil

TARGET:=mmbctl

all:	$(TARGET)

$(TARGET):	mmbctl.c

clean:
	$(RM) $(TARGET)
