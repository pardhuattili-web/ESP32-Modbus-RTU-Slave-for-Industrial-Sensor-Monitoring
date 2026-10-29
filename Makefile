CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Werror -pedantic -O2
CPPFLAGS += -Icomponents/modbus/include -Icomponents/crc16/include
.PHONY: all test clean
all: test
build:
	mkdir -p build
build/test_modbus: components/crc16/crc16.c components/modbus/modbus_rtu.c tests/test_modbus.c | build
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $@
test: build/test_modbus
	./build/test_modbus
clean:
	rm -rf build
