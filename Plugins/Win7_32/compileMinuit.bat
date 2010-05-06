cd minuit
cl.exe /EHsc /c *.cpp
cd ../NetworkCommunication/ip
cl.exe /EHsc /c *.cpp
cd ../osc
cl.exe /EHsc /c /DWIN32 *.cpp
cd ../..
cl.exe /c /EHsc /Fo../../Device.obj ../../Device.cpp
cl.exe /c /EHsc /Fo../../DeviceManager.obj ../../DeviceManager.cpp
cl.exe /c /EHsc /Fo../../PluginFactories.obj ../../PluginFactories.cpp
cl.exe /c /EHsc /DWIN32_DLL /Ipthread\include /FoMinuit.obj Minuit.cpp
link.exe /dll /out:Minuit.dll /implib:Minuit.lib Minuit.obj Minuit/*.obj NetworkCommunication/ip/*.obj NetworkCommunication/osc/*.obj ../../Device.obj ../../DeviceManager.obj ../../PluginFactories.obj Winmm.lib ws2_32.lib pthreadVC2.lib
del ../../*.obj
cl.exe /c /EHsc /DWIN32_DLL /Ipthread\include /FoOSC.obj OSC.cpp
link.exe /dll /out:OSC.dll /implib:OSC.lib OSC.obj Minuit/*.obj NetworkCommunication/ip/*.obj NetworkCommunication/osc/*.obj ../../Device.obj ../../DeviceManager.obj ../../PluginFactories.obj Winmm.lib ws2_32.lib pthreadVC2.lib
del ../../*.obj