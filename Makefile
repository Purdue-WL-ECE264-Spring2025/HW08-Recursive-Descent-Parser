CFLAGS = -std=c99 -g -Wall -Wshadow -Wvla -Werror -Wunreachable-code
DFLAGS = 
OBJS = main.o tokenizer.o hw08.o hw08_sol.o
HEADERS = hw08.h
APP = hw08.out

ifneq ($(filter E ,$(ISOLS)),)
    DFLAGS += -DINSTRUCTOR_EXPR
endif

ifneq ($(filter T ,$(ISOLS)),)
    DFLAGS += -DINSTRUCTOR_TERM
endif

ifneq ($(filter F ,$(ISOLS)),)
    DFLAGS += -DINSTRUCTOR_FACTOR
endif

.PHONY: clean

$(APP): $(OBJS)
	$(CC) $(CFLAGS) $(DFLAGS) *.o -o $(APP)

clean:
	/bin/rm -rf *.o
	/bin/rm -rf *.out

%.o: %.c $(HEADERS)
	$(CC) -c $< -o $@ $(CFLAGS) $(DFLAGS)
