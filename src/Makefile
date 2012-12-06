CFLAGS += -g --pedantic
CFLAGS += -fPIC

MULTIFAST_AHO := ../../multifast-v1.0.0/ahocorasick
CFLAGS += -I $(MULTIFAST_AHO)
LIBS   += -L $(MULTIFAST_AHO) -lahocorasick

OUT := lahocorasick.so
PREFIX   := /usr
LUA_CDIR := $(PREFIX)/lib/lua/5.1/

all: $(OUT)

$(OUT): lahocorasick.o
	$(CC) -shared $(CFLAGS) -o $@ $^  $(LIBS)

install:
	install $(OUT) $(LUA_CDIR)

clean:
	rm -f *.so *.o

.PHONY: install clean
