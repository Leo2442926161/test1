# Links the nRF hook session with the host behavior objects.
# Not a west build. Does not write MANIFEST or a flash hash.
CC      ?= gcc
CFLAGS  ?= -std=c11 -Wall -Wextra -Werror -pedantic -I../include -I../../include
BUILD   := build
BIN     := $(BUILD)/check_hook_session
SRCS    := check_hook_session.c ../src/hook_session.c \
           ../../src/fw_node.c ../../src/fw_ux.c ../../src/fw_status.c

.PHONY: test clean

test: $(BIN)
	./$(BIN)
	grep -q 'fw_node.c' ../CMakeLists.txt
	grep -q 'fw_ux.c' ../CMakeLists.txt
	grep -q 'fw_status.c' ../CMakeLists.txt
	if grep -q 'fw_sim.c' ../CMakeLists.txt; then echo 'fw_sim.c must stay out of the image'; exit 1; fi

$(BIN): $(SRCS) ../include/hook_session.h ../../include/fw.h
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -o $@ $(SRCS)

clean:
	rm -rf $(BUILD)
