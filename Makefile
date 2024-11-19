CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS =

# Главный файл
TARGET = main

# Исходники
SOURCE = model formula vars cJSON
SRCS = $(TARGET).o $(SOURCE:=.o) 
OBJS = $(addprefix $(BUILD_DIR)/, $(SRCS:.c=.o))

# Заголовочные файлы
HEADERS = $(SOURCE:=.h)

BUILD_DIR = build

all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $^

build/%.o: %.c $(HEADERS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

clean_build:
	rm -rf $(BUILD_DIR)
