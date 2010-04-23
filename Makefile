GCC = g++

all: libDeviceManager

libDeviceManager: PluginFactories.o Device.o DeviceManager.o
	ar -cr libDeviceManager.a PluginFactories.o Device.o DeviceManager.o
	rm -f *~ *.o *.dylib 

PluginFactories.o: PluginFactories.cpp PluginFactories.h 
	$(GCC) -fPIC  -c PluginFactories.cpp 

Device.o: Device.cpp Device.h
	$(GCC) -c Device.cpp 

DeviceManager.o: DeviceManager.cpp DeviceManager.h
	$(GCC) -c DeviceManager.cpp 	


clean:
	rm -f *~ *.o libDeviceManager.a
