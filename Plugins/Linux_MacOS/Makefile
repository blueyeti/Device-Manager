GCC = g++

all: 
	$(GCC) -dynamiclib -undefined dynamic_lookup Minuit.cpp Minuit/*.o NetworkCommunication/osc/*.o NetworkCommunication/ip/*.o -o Minuit.dylib
	$(GCC) -dynamiclib -undefined dynamic_lookup OSC.cpp Minuit/*.o NetworkCommunication/osc/*.o NetworkCommunication/ip/*.o -o OSC.dylib
	test -d ./Plugins || mkdir ./Plugins
	mv *.dylib ./Plugins
	rm -f *~ *.o *.dylib

clean:
	rm -f *~ *.o *.dylib Plugins/*.dylib
