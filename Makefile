TARGET = vm_riskxvii

CC = gcc

CFLAGS     = -c -Wall -Wvla -pie -Werror -Oz -O0 -std=c11
SRC        = vm_riskxvii.c helper.c operations.c heaps.c
OBJ        = $(SRC:.c=.o)

all:$(TARGET)

$(TARGET):$(OBJ)
	$(CC) $(ASAN_FLAGS) -o $@ $(OBJ)

.SUFFIXES: .c .o

.c.o:
	 $(CC) $(CFLAGS) $(ASAN_FLAGS) $<

run:
	./$(TARGET)

test:
	echo what are we testing?!

clean:
	rm -f *.o *.obj $(TARGET)
