The DeviceManager is a multi-platform library for managing communication protocols as plugins.

To use it without compiling you can get the lib in /Examples/"your os"/Example/Lib directory according to your OS.
The compiled plugins are available in /Examples/"your os"/Example/ReceiveTestApp/Plugins according to your OS too.

You can compile it with the Makefile and include the libDeviceManager.a(LINUX/MacOS) or DeviceManager.lib(Windows) in your project.
To compile it on a specific platform, you have to execute one (by default platform is linux):
	make linux
	make mac
	make windows
	
The DeviceManager header contains all the methods you should call to use the DeviceManager. 
A Doxygen describes these methods.

Two plugins are provided too, Minuit and OSC with which you can communicate through the DeviceManager.
You can generate them using a Makefile too but before this you can execute the scripts cleanMinuit.sh and compileMinuit.sh (or cleanMinuit.bat and compileMinuit.bat) 
which work on low level network files.
 
To use these plugins you have to put them in a directory as /MacintoshHD/Library/Application Support/"YourAppName" for example.
Then you have to give this path as argument when you instantiate the DeviceManager.

If you want to know how to start with the DeviceManager on a specific platform please have a look on
the first step examples given on Linux, MacOSX OS and Windows7(32).

For any question you can contact me at lo.garnier@yahoo.fr.


