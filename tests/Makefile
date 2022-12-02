FREERTOS_DIR := $(abspath FreeRTOS)
CATCH_DIR := $(abspath catch)

# source
INCLUDE_DIRS := -I.
SRCS := $(wildcard *.c)

# test source
INCLUDE_DIRS += -I$(CATCH_DIR)
TEST_SRCS := $(wildcard *.cpp)

# FreeRTOS Posix porting
SRCS += $(wildcard $(FREERTOS_DIR)/*.c)
SRCS += $(FREERTOS_DIR)/portable/MemMang/heap_3.c
SRCS += $(FREERTOS_DIR)/portable/ThirdParty/GCC/Posix/utils/wait_for_event.c
SRCS += $(FREERTOS_DIR)/portable/ThirdParty/GCC/Posix/port.c
INCLUDE_DIRS += -I$(FREERTOS_DIR)/include
INCLUDE_DIRS += -I$(FREERTOS_DIR)/portable/ThirdParty/GCC/Posix
INCLUDE_DIRS += -I$(FREERTOS_DIR)/portable/ThirdParty/GCC/Posix/utils

CFLAGS := -Wall -Wextra -std=c17 -D_GNU_SOURCE -DprojCOVERAGE_TEST=0
CXXFLAGS := -Wall -Wextra -std=c++17 -D_GNU_SOURCE -DprojCOVERAGE_TEST=0
LDFLAGS += -pthread

ifndef DISABLE_SANITIZER
CFLAGS += -g -fsanitize=address -fsanitize=undefined -fsanitize=leak -fsanitize=alignment 
CXXFLAGS += -g -fsanitize=address -fsanitize=undefined -fsanitize=leak -fsanitize=alignment 
endif

# target binary
BUILD_DIR := build
OBJS := $(addprefix $(BUILD_DIR)/, $(SRCS:.c=.o)) $(addprefix $(BUILD_DIR)/, $(TEST_SRCS:.cpp=.o))
TARGET := $(BUILD_DIR)/test

.PHONY: all run clean prepare

all: $(TARGET)

run: all
	./$(TARGET)

clean:
	-@$(RM) -r $(BUILD_DIR)

prepare: $(FREERTOS_DIR) $(CATCH_DIR)/catch.hpp

$(FREERTOS_DIR):
	curl -L https://github.com/FreeRTOS/FreeRTOS-Kernel/releases/download/V10.5.0/FreeRTOS-KernelV10.5.0.zip -O
	unzip FreeRTOS-KernelV10.5.0.zip
	-@$(RM) -r FreeRTOS-KernelV10.5.0.zip
	mv -T FreeRTOS-KernelV10.5.0 $@

$(CATCH_DIR)/catch.hpp:
	mkdir -p catch
	curl -L https://raw.githubusercontent.com/catchorg/Catch2/v2.x/single_include/catch2/catch.hpp -o $(CATCH_DIR)/catch.hpp

$(BUILD_DIR)/%.o : %.c
	-mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) -c $< -o $@

$(BUILD_DIR)/%.o : %.cpp
	-mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(CXXFLAGS) $(INCLUDE_DIRS) $(LDFLAGS)
