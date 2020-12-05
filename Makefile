OBJNAME = cq-ev-kart
OBJFILE = $(OBJNAME)

# 開発環境
CC = v850-elf-gcc
CXX = v850-elf-g++
AS = v850-elf-as
LD = v850-elf-ld
AR = v850-elf-ar
NM = v850-elf-nm
RANLIB = v850-elf-ranlib
OBJCOPY = v850-elf-objcopy
OBJDUMP = v850-elf-objdump

LINK = $(CC)


# コンパイラオプションの定義
COPTS := $(COPTS)  -mv850e -mdisable-callt -g -Wall -O2
#COPTS := $(COPTS)  -mv850e -mdisable-callt -g -Wall -O0
CDEFS := $(CDEFS) 
INCLUDES := -I. $(INCLUDES)
CFLAGS = $(COPTS) $(CDEFS) $(INCLUDES)

# リンカオプション
LDSCRIPT = v850fg3.ld
LDFLAGS := $(LDFLAGS) -nostartfiles -lgcc -lc -msoft-float -T $(LDSCRIPT)

ALL_OBJS = start.o main.o interrupt_vector.o interrupt.o nop_interrupt.o clock.o system_init.o port.o e1_debug_memory_alloc.o timer.o ad.o motor.o serial.o


all: $(OBJFILE)

start.o: start.S
	$(CC) -c $(CFLAGS) start.S

main.o: main.c
	$(CC) -c $(CFLAGS) main.c

interrupt_vector.o: interrupt_vector.S
	$(CC) -c $(CFLAGS) interrupt_vector.S

interrupt.o: interrupt.S
	$(CC) -c $(CFLAGS) interrupt.S

nop_interrupt.o: nop_interrupt.c
	$(CC) -c $(CFLAGS) nop_interrupt.c

clock.o: clock.c
	$(CC) -c $(CFLAGS) clock.c

system_init.o: system_init.c
	$(CC) -c $(CFLAGS) system_init.c

port.o: port.c
	$(CC) -c $(CFLAGS) port.c

e1_debug_memory_alloc.o: e1_debug_memory_alloc.S
	$(CC) -c $(CFLAGS) e1_debug_memory_alloc.S

timer.o: timer.c
	$(CC) -c $(CFLAGS) timer.c

ad.o: ad.c
	$(CC) -c $(CFLAGS) ad.c

motor.o: motor.c
	$(CC) -c $(CFLAGS) motor.c

serial.o: serial.c
	$(CC) -c $(CFLAGS) serial.c


#
#  全体のリンク
#
$(OBJFILE): $(ALL_OBJS)
	$(LINK) $(CFLAGS) $(LDFLAGS) -o $(OBJFILE) $(ALL_OBJS)
	$(NM) -n $(OBJFILE) > $(OBJNAME).syms
	$(OBJCOPY) -O srec -S $(OBJFILE) $(OBJNAME).srec

	
.PHONY: clean
clean:
	rm -f *.o
	rm -f $(OBJFILE) $(OBJNAME).syms $(OBJNAME).srec
