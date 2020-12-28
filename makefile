PROGRAM := program
CC := clang++

.PHONY: all
all: $(PROGRAM)

$(PROGRAM): main.o
	$(CC) -o $@ $^

%.o: %.cc %.hh
	$(CC) -c $<
%.o: %.cc
	$(CC) -c $<

.PHONY: clean
clean:
	rm -f $(PROGRAM)
	rm -f *.o
