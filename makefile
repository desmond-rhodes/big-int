PROGRAM := big-int
cc := clang

.PHONY: all
all: $(PROGRAM)

$(PROGRAM): main.o
	$(cc) -o $@ $^

%.o: %.c %.h
	$(cc) -c $<
%.o: %.c
	$(cc) -c $<

.PHONY: clean
clean:
	rm -f $(PROGRAM)
	rm -f *.o
