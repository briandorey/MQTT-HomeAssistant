# ------------------------------------------------
# House FS Makefile

# Date  : 2018-02-19


# project name (generate executable with this name)
TARGET   = temp_sensors_mqtt_client

CC       = gcc
# compiling flags here
CFLAGS   = -std=c99 -Wall -D_BSD_SOURCE -D_DEFAULT_SOURCE -l paho-mqtt3c -I.
LDFLAGS  = -pthread
LINKER   = gcc
# linking flags here
LFLAGS   = -Wall -D_BSD_SOURCE -D_DEFAULT_SOURCE  -l paho-mqtt3c -I. -lm
# change these to proper directories where each file should be
SRCDIR   = src
OBJDIR   = obj
BINDIR   = bin

SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(SRCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
rm        = rm -f


$(BINDIR)/$(TARGET): $(OBJECTS)
	@$(LINKER) $(OBJECTS) $(LFLAGS) -o $@ $(LDFLAGS)
	@echo "Linking complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS)
	@echo "Compiled "$<" successfully!"

.PHONY: clean
clean:
	@$(rm) $(OBJECTS)
	@echo "Cleanup complete!"

.PHONY: remove
remove: clean
	@$(rm) $(BINDIR)/$(TARGET)
	@echo "Executable removed!"