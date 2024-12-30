#compiler/folder settings
MAIN ?= default
FILE_EXT = $(suffix $(MAIN))
BOOST = "F:/C_C++ Coding/boost_1_87_0/"

ifeq ($(FILE_EXT),.c)
	CC = gcc
	CFLAGS = -g -O2 -Wall -Wpedantic 
else ifeq ($(FILE_EXT),.cpp)
	CC = g++
	CFLAGS = -g -O2 -Wall -Werror -Wpedantic -std=c++20 -I$(BOOST)
else
	$(error Unsupported file extension: $(FILE_EXT))
endif

TARGET = KataApp
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
