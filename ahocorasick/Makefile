CFLAGS += -g --pedantic
CFLAGS += -fPIC

MULTIFAST_AHO := ../../multifast-v1.0.0/ahocorasick
CFLAGS += -I $(MULTIFAST_AHO)
LIBS   += -L $(MULTIFAST_AHO) -lahocorasick

OUT := lahocorasick.so

all: $(OUT)

$(OUT): lahocorasick.o
	$(CC) -shared $(CFLAGS) -o $@ $^  $(LIBS)

clean:
	rm -f *.so *.o

.PHONY: clean
