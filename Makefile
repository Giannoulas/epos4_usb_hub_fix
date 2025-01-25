CC = g++
CFLAGS = -I.
TARGET = main
LIBS = -lEposCmd

all: $(TARGET)

$(TARGET): main.o value_assigner.o
	$(CC) -o $(TARGET) main.o value_assigner.o $(CFLAGS) $(LIBS)

main.o: main.cpp
	$(CC) $(CFLAGS) $(LIBS) -c main.cpp -o main.o

value_assigner.o: value_assigner.cpp
	$(CC) $(CFLAGS) $(LIBS) -c value_assigner.cpp -o value_assigner.o

clean:
	$(RM) main.o value_assigner.o $(TARGET)
