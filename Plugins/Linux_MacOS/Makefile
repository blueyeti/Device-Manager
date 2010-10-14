#give your OS on the command line :
#make linux
#make macos

#OS by default :
OS = LINUX

GCC = g++

.PHONY: linux
linux:
	make all OS=LINUX
        
.PHONY: mac
mac:
	make all OS=MAC

all: 
ifeq ($(OS), MAC)
	$(GCC) -dynamiclib -undefined dynamic_lookup Minuit.cpp Minuit/*.o NetworkCommunication/osc/*.o NetworkCommunication/ip/*.o ../../xmlParser.o ../../Device.o ../../DeviceManager.o ../../PluginFactories.o -o Minuit.dylib
	$(GCC) -dynamiclib -undefined dynamic_lookup OSC.cpp Minuit/*.o NetworkCommunication/osc/*.o NetworkCommunication/ip/*.o ../../xmlParser.o ../../Device.o ../../DeviceManager.o ../../PluginFactories.o -o OSC.dylib
	test -d ./Plugins || mkdir ./Plugins
	mv *.dylib ./Plugins
	rm -f *~ *.o *.dylib
else
	$(GCC) -c -fPIC Minuit.cpp
	$(GCC) -shared Minuit.o Minuit/*.o NetworkCommunication/osc/*.o NetworkCommunication/ip/*.o ../../xmlParser.o ../../Device.o ../../DeviceManager.o ../../PluginFactories.o -o Minuit.so
	$(GCC) -c -fPIC OSC.cpp
	$(GCC) -shared OSC.o Minuit/*.o NetworkCommunication/osc/*.o NetworkCommunication/ip/*.o ../../xmlParser.o ../../Device.o ../../DeviceManager.o ../../PluginFactories.o -o OSC.so
	test -d ./Plugins || mkdir ./Plugins
	mv *.so ./Plugins
	rm -f *~ *.o *.so
endif

clean:
	rm -f *~ *.o *.dylib *.so Plugins/*.dylib Plugins/*.so

