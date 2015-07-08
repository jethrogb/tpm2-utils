SOURCES=tpm2-test.c
OBJECTS=$(SOURCES:.c=.o)
OUTPUT=tpm2-test

all: $(SOURCES) $(OUTPUT) Makefile
	
$(OUTPUT): $(OBJECTS)
	gcc -Wall $(OBJECTS) -o $@

.c.o:
	gcc -Wall -c $< -o $@

clean:
	rm -f $(OBJECTS) $(OUTPUT)
