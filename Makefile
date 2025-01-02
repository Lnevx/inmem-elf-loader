TARGET := loader

CFLAGS += -Wall -Wextra -fPIC -Iinclude -Os -std=c99 -DELFCLASS=ELFCLASS64
LDFLAGS += -nostartfiles -nodefaultlibs -nostdlib -static-pie -Wl,-T,loader.ld

OBJS := $(TARGET).o
OBJS += $(patsubst %.c,%.o, $(shell find src -name "*.c"))
OBJS += $(patsubst %.S,%.o, $(shell find src -name "*.S"))


.PHONY: all
all: $(TARGET)

.PHONY: clean
clean:
	rm -rf $(OBJS) $(TARGET)

loader: $(OBJS)
