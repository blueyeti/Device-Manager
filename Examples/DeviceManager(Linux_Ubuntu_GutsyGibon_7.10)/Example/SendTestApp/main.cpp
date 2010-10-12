/*
 Copyright: 
 LaBRI (http://www.labri.fr) - BlueYeti (http://www.blueyeti.fr) - GMEA (http://www.gmea.net)
 
 Author(s): Laurent Garnier, Theo Delahogue
 Last modification: 08/03/2010
 
 Adviser(s): 
 Myriam Desainte-Catherine (myriam.desainte-catherine@labri.fr)
 
 This software is a computer program whose purpose is to propose
 a library for interactive scores edition and execution.
 
 This software is governed by the CeCILL-C license under French law and
 abiding by the rules of distribution of free software.  You can  use, 
 modify and/ or redistribute the software under the terms of the CeCILL-C
 license as circulated by CEA, CNRS and INRIA at the following URL
 "http://www.cecill.info". 
 
 As a counterpart to the access to the source code and  rights to copy,
 modify and redistribute granted by the license, users are provided only
 with a limited warranty  and the software's author,  the holder of the
 economic rights,  and the successive licensors  have only  limited
 liability. 
 
 In this respect, the user's attention is drawn to the risks associated
 with loading,  using,  modifying and/or developing or reproducing the
 software by the user in light of its specific status of free software,
 that may mean  that it is complicated to manipulate,  and  that  also
 therefore means  that it is reserved for developers  and  experienced
 professionals having in-depth computer knowledge. Users are therefore
 encouraged to load and test the software's suitability as regards their
 requirements in conditions enabling the security of their systems and/or 
 data to be ensured and,  more generally, to use and operate it in the 
 same conditions as regards security.
 
 The fact that you are presently reading this means that you have had
 knowledge of the CeCILL-C license and that you accept its terms.
 */

/*
 *  main.cpp
 *  DeviceManager
 *
 *  Created by Laurent Garnier on 03/06/09.
 *  Copyright 2009 __BlueYeti/LaBRI__. All rights reserved.
 *
 */

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <DeviceManager.h>

using namespace std;

int 
main(int argc, char **argv) 
{
	/*Load Plugins*/
	DeviceManager *deviceManager = new DeviceManager("SendTestApp");	
	deviceManager->pluginLoad("Plugins");

	deviceManager->pluginSetCommParameter("Minuit", "pluginReceptionPort", "9998");
	deviceManager->pluginSetCommParameter("OSC", "pluginReceptionPort", "9999");

	deviceManager->pluginLaunch();
	cout << endl;
	
	vector<string> plugins = deviceManager->pluginGetLoadedByName();
	cout << "Current loaded plugins : " << endl;
	for (int i = 0; i < plugins.size(); i++) {
		cout << plugins.at(i) << endl;
	}
	cout << endl;
	/***************/

	/*Add Devices*/
	//Add 1 Minuit device
	std::map<std::string, std::string> *deviceParameters = new std::map<std::string, std::string>;
	deviceParameters->insert(std::pair<std::string, std::string>("ip", "127.0.0.1"));
	deviceParameters->insert(std::pair<std::string, std::string>("port", "7002"));	

	deviceManager->deviceAdd("ReceiveTestApp", "Minuit", deviceParameters);

	map<string, Device*> *devices = deviceManager->deviceGetCurrent();
	cout << "Current devices : " << endl;
	map<string, Device*>::iterator it = devices->begin();
	while(it != devices->end()){
		cout << it->first << endl;
		it++;
	}
	cout << endl;
	/***************/
	
	/*test sendMessage*/
	Address address = "/video/play";
	Value value = "1";
	deviceManager->deviceSendSetRequest("ReceiveTestApp", address, value);
	/***************/

	delete devices;
	delete deviceParameters;
	delete deviceManager;

	return EXIT_SUCCESS;
}

