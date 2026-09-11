CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -Iinclude
LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

SRC_DIR = src
BUILD_DIR = build/normal
BIN_DIR = bin

# Módulos compartilhados do sistema
CORE_SRCS = $(SRC_DIR)/arena.c \
            $(SRC_DIR)/ring_buffer.c \
            $(SRC_DIR)/device.c \
            $(SRC_DIR)/module_defense.c \
            $(SRC_DIR)/module_navigation.c

CORE_OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(CORE_SRCS))

# Binários finais
GUI_TARGET = $(BIN_DIR)/gui_launcher
CLI_TARGET = $(BIN_DIR)/cli_tests

all: $(GUI_TARGET) $(CLI_TARGET)

# Compilação da interface gráfica Raylib
$(GUI_TARGET): $(CORE_OBJS) $(BUILD_DIR)/gui_main.o | $(BIN_DIR)
	@mkdir -p $(BIN_DIR)
	@echo "\033[32;01m[LINK] Linking $@\033[0m"
	$(CC) $^ -o $@ $(LDFLAGS)

# Compilação do teste de integração via CLI
$(CLI_TARGET): $(CORE_OBJS) $(BUILD_DIR)/main.o | $(BIN_DIR)
	@mkdir -p $(BIN_DIR)
	@echo "\033[32;01m[LINK] Linking $@\033[0m"
	$(CC) $^ -o $@ $(LDFLAGS)

# Regra de compilação dos objetos .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)
	@echo "\033[32;01m[CC]   Compiling $<\033[0m"
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	@echo "\033[31;01m[CLEAN] Removing build artifacts...\033[0m"
	rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: all clean
