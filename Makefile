#compiler/folder settings
MAIN = default
TARGET = KataApp
CC = clang++
CFLAGS = -g -O2 -Wall -Werror -Wpedantic -std=c++20
DIR = $(CURDIR)
BUILD = build
OBJECTS = $(BUILD)/$(basename $(MAIN)).o

#compiling
run: $(TARGET)

$(BUILD)/%.o: $(MAIN)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

#cleanup objects and executables
clean:
	@powershell rm *.exe
	@powershell rm build/*
	@echo "Cleaning up"

.PHONY: run clean
