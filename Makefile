# ==============================================================================
# Lancelot Axe - Project Build Automation
# ==============================================================================

CC        := gcc
CFLAGS    := -Wall -Wextra -Werror -std=c11 -pedantic -Iinclude -MMD -MP
SANFLAGS  := -g -fsanitize=address,undefined -fno-omit-frame-pointer

# Diretorios
SRC_DIR   := src
INC_DIR   := include
BUILD_DIR := build
BIN_DIR   := bin

# Arquivos fonte e objetos
SRCS      := $(wildcard $(SRC_DIR)/*.c)
OBJS      := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/normal/%.o, $(SRCS))
OBJS_SAN  := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/san/%.o, $(SRCS))
DEPS      := $(OBJS:.o=.d) $(OBJS_SAN:.o=.d)

# Nomes dos binarios
TARGET     := $(BIN_DIR)/lancelot_axe
TARGET_SAN := $(BIN_DIR)/bin_test_asan

# Colorization helper
NO_COLOR   := \033[0m
OK_COLOR   := \033[32;01m
WARN_COLOR := \033[33;01m
ERROR_COLOR:= \033[31;01m

.PHONY: all sanitize run test clean help

# Targets padrao
all: $(TARGET)

sanitize: $(TARGET_SAN)

# Build Padrão
$(TARGET): $(OBJS) | $(BIN_DIR)
	@echo "$(OK_COLOR)[LINK] Linking $@$(NO_COLOR)"
	@$(CC) $(CFLAGS) $^ -o $@

$(BUILD_DIR)/normal/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)/normal
	@echo "$(OK_COLOR)[CC]   Compiling $<$(NO_COLOR)"
	@$(CC) $(CFLAGS) -O2 -c $< -o $@

# Build com Sanitizers (ASan / UBSan)
$(TARGET_SAN): $(OBJS_SAN) | $(BIN_DIR)
	@echo "$(WARN_COLOR)[LINK] Linking $@ (Sanitizers Enabled)$(NO_COLOR)"
	@$(CC) $(CFLAGS) $(SANFLAGS) $^ -o $@

$(BUILD_DIR)/san/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)/san
	@echo "$(WARN_COLOR)[CC]   Compiling $< (Sanitizers Enabled)$(NO_COLOR)"
	@$(CC) $(CFLAGS) $(SANFLAGS) -c $< -o $@

# Criacao das pastas de output
$(BIN_DIR) $(BUILD_DIR)/normal $(BUILD_DIR)/san:
	@mkdir -p $@

# Execucao dos binarios
run: $(TARGET)
	@echo "$(OK_COLOR)[RUN] Executing $(TARGET)...$(NO_COLOR)"
	@./$(TARGET)

test: $(TARGET_SAN)
	@echo "$(WARN_COLOR)[TEST] Executing $(TARGET_SAN) with ASan/UBSan...$(NO_COLOR)"
	@./$(TARGET_SAN)

# Limpeza completa de artefatos
clean:
	@echo "$(ERROR_COLOR)[CLEAN] Removing build artifacts...$(NO_COLOR)"
	@rm -rf $(BUILD_DIR) $(BIN_DIR)

# Inclui arquivos de dependencia gerados automaticamente pelo GCC (-MMD -MP)
-include $(DEPS)

# Ajuda basica
help:
	@echo "Lancelot Axe Build System"
	@echo "  make          - Compila o projeto em modo otimizado (O2)"
	@echo "  make sanitize - Compila o projeto com ASan e UBSan ativados"
	@echo "  make run      - Compila e executa o binario padrao"
	@echo "  make test     - Compila e executa o binario de teste com sanitizers"
	@echo "  make clean    - Remove as pastas 'build/' e 'bin/'"
