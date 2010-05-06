#give your OS here, WIN32, MAC, or LINUX
OS = WIN32

ifeq ($(OS), WIN32)
	GCC = cl.exe /c /EHsc /D_WIN32
else
	GCC = g++ -c -Wall
endif


all: libDeviceManager

libDeviceManager: PluginFactories.obj Device.obj DeviceManager.obj
ifeq ($(OS), WIN32) 
	lib.exe /out:DeviceManager.lib PluginFactories.obj Device.obj DeviceManager.obj
	del *~ *.obj
else 
	ar -cr libDeviceManager.a PluginFactories.o Device.o DeviceManager.o
	rm -f *~ *.o
endif

PluginFactories.obj: PluginFactories.cpp PluginFactories.h 
	$(GCC) PluginFactories.cpp 

Device.obj: Device.cpp Device.h
	$(GCC) Device.cpp 

DeviceManager.obj: DeviceManager.cpp DeviceManager.h
	$(GCC) DeviceManager.cpp 	


clean:
ifeq ($(OS), WIN32) 
	del *~ *.obj DeviceManager.lib
else
	rm -f *~ *.o libDeviceManager.a
endif