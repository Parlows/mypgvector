CC := gcc
COMPILER_FLAGS := -Wall -g

# Build check_size test
.PHONY: check_size
check_size: check_size.out

check_size.out: size check_size.c
	$(CC) $(COMPILER_FLAGS) -o $@ $^

# Build parsing test
.PHONY: parsing
parsing: parsing.out

parsing.out: size parsing.c
	$(CC) $(COMPILER_FLAGS) -o $@ $^

# Build to_string test
.PHONY: to_string
to_string: to_string.out

# Creates the program's binaries
to_string.out: size to_string.c lib/ryu/ryu/f2s.o
	$(CC) $(COMPILER_FLAGS) -o $@ $^

# Only executes on first make, or when file 'size' is deleted/modified
size: lib/ryu/ryu/f2s.o
	size $^
	touch size

# Phony target to clean binaries
.PHONY: clean
clean:
	rm -f *.out *.o

# Phony target to clean binaries and other files
.PHONY: full-clean
full-clean: clean
	rm -f size
