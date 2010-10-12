GCC = g++

all: ReceiveTestApp

ReceiveTestApp: main.cpp
	$(GCC) main.cpp -o ReceiveTestApp -L../Lib -I../Header -lDeviceManager -ldl -lpthread -rdynamic

clean:
	rm -f *~ ReceiveTestApp

