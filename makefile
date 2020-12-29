PROGRAM := program
CC := clang++
FLAGS := -std=c++17

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
