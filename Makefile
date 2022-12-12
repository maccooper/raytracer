CC= g++

TARGETS = raytracer

.PHONY: all clean

OBJ = $(patsubst %,obj/%.o,$(TARGETS))
SRC = ./%.cpp

all: raytracer 

obj/%.o: $(SRC) | obj
	$(CC) -c -o $@ $< $(CFLAGS)

obj:
	mkdir -p $@

raytracer: $(OBJ)
	$(CC) -o raytracer.exe $^ $(CFLAGS) 


clean:
	-rm -rf obj/ raytracer.exe
