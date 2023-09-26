CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

SRCDIR = src
INCDIR = include
BINDIR = bin
OBJDIR = obj
TESTDIR = test

SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC))

TEST_SRC = $(wildcard $(TESTDIR)/*.c)
TEST_OBJ = $(patsubst $(TESTDIR)/%.c,$(BINDIR)/%,$(TEST_SRC))

all: $(OBJ) $(BINDIR)/test

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(BINDIR)/test: $(TEST_OBJ) $(OBJ)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: all clean