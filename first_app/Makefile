FREERTOS_DIR := $(abspath FreeRTOS)

# source
INCLUDE_DIRS := -I.
SRCS := $(wildcard *.c)

# FreeRTOS Posix porting
SRCS += $(wildcard $(FREERTOS_DIR)/*.c)
SRCS += $(FREERTOS_DIR)/portable/MemMang/heap_3.c
SRCS += $(FREERTOS_DIR)/portable/ThirdParty/GCC/Posix/utils/wait_for_event.c
SRCS += $(FREERTOS_DIR)/portable/ThirdParty/GCC/Posix/port.c
INCLUDE_DIRS += -I$(FREERTOS_DIR)/include
INCLUDE_DIRS += -I$(FREERTOS_DIR)/portable/ThirdParty/GCC/Posix
INCLUDE_DIRS += -I$(FREERTOS_DIR)/portable/ThirdParty/GCC/Posix/utils

CFLAGS := -Wall -Wextra -std=c17 -D_GNU_SOURCE -DprojCOVERAGE_TEST=0
LDFLAGS += -pthread

ifndef DISABLE_SANITIZER
CFLAGS += -g -fsanitize=address -fsanitize=undefined -fsanitize=leak -fsanitize=alignment 
endif

# target binary
BUILD_DIR := build
OBJS := $(addprefix $(BUILD_DIR)/, $(SRCS:.c=.o))
TARGET := $(BUILD_DIR)/hello

.PHONY: all run clean prepare

all: $(TARGET)

run: all
	./$(TARGET)

clean:
	-@$(RM) -r $(BUILD_DIR)

prepare: $(FREERTOS_DIR)

$(FREERTOS_DIR):
	curl -L https://github.com/FreeRTOS/FreeRTOS-Kernel/releases/download/V10.5.0/FreeRTOS-KernelV10.5.0.zip -O
	unzip FreeRTOS-KernelV10.5.0.zip
	-@$(RM) -r FreeRTOS-KernelV10.5.0.zip
	mv -T FreeRTOS-KernelV10.5.0 $@

$(BUILD_DIR)/%.o : %.c
	-mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(CFLAGS) $(INCLUDE_DIRS) $(LDFLAGS)
