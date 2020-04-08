CC = g++ -std=c++17
SRCDIR = .
BUILDDIR = obj
TARGET = server
 
SRCEXT = cpp

SOURCES = $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))

OBJECTS = $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

INC = -I inc -I apps/inc

$(TARGET): $(OBJECTS)
	@echo "$(CC) $(INC) $^ -o $(TARGET)"; $(CC) $(INC) $^ -o $(TARGET)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	$(CC) $(INC) -c -o $@ $<

clean:
	@echo "find . -type f -name '*.o' -delete"; find . -type f -name '*.o' -delete
	@echo "rm ./$(TARGET)"; rm ./$(TARGET)

run:
	./$(TARGET) $(FLAGS) $(HOST)

makefiledebug:
	@echo "MAKEFILE VARIABLES: ";
	@echo "|CC| => $(CC)"
	@echo "|SRCDIR| => $(SRCDIR)"
	@echo "|BUILDDIR| => $(BUILDDIR)"
	@echo "|TARGET| => $(TARGET)"
	@echo "|SRCEXT| => $(SRCEXT)"
	@echo "|SOURCES| => $(SOURCES)"
	@echo "|OBJECTS| => $(OBJECTS)"
	@echo "|LIB| => $(LIB)"
	@echo "|INC| => $(INC)"

.PHONY: clean