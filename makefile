UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	CC := g++
endif
ifeq ($(UNAME_S),Darwin)
	CC := clang++
endif
FLAGS := -std=c++17
PROGRAM := program

.PHONY: all
all: $(PROGRAM)

$(PROGRAM): main.o ap_n.o
	$(CC) $(FLAGS) -o $@ $^

main.o: ap_n.hh

%.o: %.cc %.hh
	$(CC) $(FLAGS) -c $<
%.o: %.cc
	$(CC) $(FLAGS) -c $<

.PHONY: clean
clean:
	rm -f $(PROGRAM)
	rm -f *.o
