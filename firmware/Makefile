# Host sim for ICD v0.1.7 hush / cascade / SED. No nRF Connect SDK required.
CC      ?= gcc
CFLAGS  ?= -std=c11 -Wall -Wextra -Werror -pedantic -Iinclude
BUILD   := build
SRCS    := src/fw_node.c src/fw_ux.c src/fw_status.c
OBJS    := $(BUILD)/fw_node.o $(BUILD)/fw_ux.o $(BUILD)/fw_status.o
SIM     := $(BUILD)/fw_sim
TESTBIN := $(BUILD)/test_fw

.PHONY: all test clean manifest verify

all: $(SIM) $(TESTBIN)

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/%.o: src/%.c include/fw.h | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(SIM): src/fw_sim.c $(OBJS) include/fw.h | $(BUILD)
	$(CC) $(CFLAGS) -o $@ src/fw_sim.c $(OBJS)

$(TESTBIN): test/test_fw.c $(OBJS) include/fw.h | $(BUILD)
	$(CC) $(CFLAGS) -o $@ test/test_fw.c $(OBJS)

test: all
	./$(TESTBIN)
	sh test/cli_hooks.sh

clean:
	rm -rf $(BUILD)

manifest:
	python3 tools/gen_manifest.py

verify:
	python3 tools/gen_manifest.py --check
