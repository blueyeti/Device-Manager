GCC = g++

all: SendTestApp

SendTestApp: main.cpp
	$(GCC) main.cpp -o SendTestApp -L../Lib -I../Header -lDeviceManager -ldl -lpthread -rdynamic

clean:
	rm -f *~ SendTestApp

