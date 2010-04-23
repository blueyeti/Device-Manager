The DeviceManager is a library for managing communication protocols as plugins.

You can compile it with the Makefile and include the libDeviceManager.a in your project.
The DeviceManager header contains all the methods you should call to use the DeviceManager. 
A Doxygen describes these methods.

Two plugins are provided too, Minuit and OSC with which you can communicate through the DeviceManager.
You can generate them using a Makefile too but before this you can execute the scripts cleanMinuit.sh and compileMinuit.sh which work on low level network files.
 
To use these plugins you have to put them in a directory as /MacintoshHD/Library/Application Support/"YourAppName" for example.
Then you have to give this path as argument when you instantiate the DeviceManager.

Two first step examples are given on Linux and on MacOSX OS (the windows version is coming).

For any question you can contact me at lo.garnier@yahoo.fr.


