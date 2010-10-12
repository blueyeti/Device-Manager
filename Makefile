#give your OS here, WIN32, MAC, or LINUX
OS = MAC

ifeq ($(OS), WIN32)
	GCC = cl.exe /c /EHsc
else
	GCC = g++ -c -Wall
endif


all: libDeviceManager

libDeviceManager: PluginFactories.obj Device.obj xmlParser.obj DeviceManager.obj
ifeq ($(OS), WIN32) 
	lib.exe /out:DeviceManager.lib PluginFactories.obj Device.obj xmlParser.obj DeviceManager.obj
	rm -f *~ *.obj
else 
	ar -cr libDeviceManager.a PluginFactories.o Device.o xmlParser.o DeviceManager.o
	rm -f *~ *.o
endif


test: main.obj PluginFactories.obj Device.obj xmlParser.obj DeviceManager.obj
	g++ *.obj -o test 

main.obj: main.cpp
	$(GCC) main.cpp

PluginFactories.obj: PluginFactories.cpp PluginFactories.h 
ifeq ($(OS), LINUX)
	$(GCC) -fPIC PluginFactories.cpp
else
	$(GCC) PluginFactories.cpp 
endif

Device.obj: Device.cpp Device.h
	$(GCC) Device.cpp

xmlParser.obj: xmlParser.cpp xmlParser.h
	$(GCC) xmlParser.cpp

DeviceManager.obj: DeviceManager.cpp DeviceManager.h
	$(GCC) DeviceManager.cpp


clean:
ifeq ($(OS), WIN32) 
	rm -f *~ *.obj DeviceManager.lib
else
	rm -f *~ *.obj libDeviceManager.a
endif