TARGET_EXEC := snake

BUILD_DIR := ./build
LIB_DIR := ./lib
SRC_DIRS := ./src
INC_DIRS := ./include 

SRCS := $(shell find $(SRC_DIRS) -name '*.c' -or -name '*.s')
LIBS := $(shell find $(LIB_DIR) -name '*.a')
INC_DIRS += $(shell find $(LIB_DIR) -name 'include')
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

CC := gcc
INC_FLAGS := $(addprefix -I,$(INC_DIRS))
CFLAGS := $(INC_FLAGS) -MMD -MP -Wall -Wextra -O3 -g
LDFLAGS := $(LIBS) -lm -ldl -lpthread

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)

.PHONY: run
run:
	make
	./$(BUILD_DIR)/$(TARGET_EXEC)

.PHONY: install
install:
	mkdir -p $(LIB_DIR)
	wget -c -O raylib.tar.gz https://github.com/raysan5/raylib/releases/download/4.2.0/raylib-4.2.0_linux_amd64.tar.gz
	tar -xf raylib.tar.gz
	rm -rf raylib.tar.gz
	cp -r raylib-4.2.0_linux_amd64 $(LIB_DIR)/raylib
	rm -rf raylib-4.2.0_linux_amd64

-include $(DEPS)