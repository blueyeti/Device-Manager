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
 *  DeviceManager.cpp
 *  DeviceManager
 *
 *  Created by Laurent Garnier on 03/06/09.
 *  Copyright 2009 __BlueYeti/LaBRI__. All rights reserved.
 *
 */

#include "DeviceManager.h"
#include "Device.h"
#include "PluginFactories.h"
#include "Plugins/Plugin.h"
#include "xmlParser.h"
#include "Namespace.h"

#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <sstream>


DeviceManager::DeviceManager(std::string aName)
{
	m_application_name = aName;
	
	deviceId = 0;
	factories = new PluginFactories();
	netDevices = new std::map<std::string, Device*>;
	netPlugins = new std::map<std::string, Plugin*>;
}

DeviceManager::~DeviceManager()
{
	netDevices->clear();
	netPlugins->clear();

	delete netDevices;
	delete netPlugins;
	delete factories;
}

/************************************************
 PLUGINS METHODS :
 a set of methods used to manage plugins.
 note : each method of this set is prepend by 'plugin'
 ************************************************/

void
DeviceManager::pluginLoad(std::string path)
{	
	factories->loadPlugins(path);

	//create an instance of each plugin available
	IteratorPluginNames it = factories->getPluginNames();
	
	while (it.hasNext()) {
		std::string pname = it.next();
		//std::cout << pname << std::endl;
		Plugin *p = factories->createPlugin(pname, this);
		if (p != 0) {
			netPlugins->insert((std::pair<std::string, Plugin*>(pname, p)));
		}
	}	

	//define parameters for each plugin available
	std::map<std::string, Plugin*>::iterator itr = netPlugins->begin();
	while (itr != netPlugins->end()) {		
		itr->second->commDefineParameters();	
		itr++;
	}
}

void 
DeviceManager::pluginLoadConfigXml(std::string filename)
{
	XMLNode xMainNode = XMLNode::openFileHelper(filename.c_str(), "deviceManager");
	XMLNode xNode = xMainNode.getChildNode("receiveDevices");
	int nChild = xNode.nChildNode();
	
	for (int i = 0; i < nChild; i++) {
		XMLNode deviceNode = xNode.getChildNode(i);
		int nAttr = deviceNode.nAttribute();

		for (int j = 0; j < nAttr; j++) {
			pluginSetCommParameter(deviceNode.getName(), deviceNode.getAttributeName(j), deviceNode.getAttributeValue(j));
		}
	}
}

void 
DeviceManager::pluginSetCommParameter(std::string pluginName, std::string parameterName, std::string parameterValue)
{
	std::map<std::string, Plugin*>::iterator it  = netPlugins->find(pluginName);

	if (it != netPlugins->end()) {
		it->second->commSetParameter(parameterName, parameterValue);
	} else {
		std::cout << "DeviceManager::pluginSetCommParameter No Plugin named : " << pluginName << std::endl;
	}
}

std::string
DeviceManager::pluginGetCommParameter(std::string pluginName, std::string parameterName)
{	
	std::map<std::string, Plugin*>::iterator it  = netPlugins->find(pluginName);
	
	if (it != netPlugins->end()) {
		return it->second->commGetParameter(parameterName);
	} else {
		std::cout << "DeviceManager::pluginGetCommParameter No Plugin named : " << pluginName << std::endl;
		return "ERROR";
	}
}

void 
DeviceManager::pluginLaunch()
{
	//run a reception thread for each plugin available
	std::map<std::string, Plugin*>::iterator itr = netPlugins->begin();
	while (itr != netPlugins->end()) {		
		itr->second->commRunReceivingThread();

		itr++;
	}
}

std::vector<std::string>
DeviceManager::pluginGetLoadedByName() 
{	
	std::map<std::string, Plugin*>::iterator it  = netPlugins->begin();
	
	std::vector<std::string> result;
	
	while (it != netPlugins->end()) {
		//std::cout << it->first << std::endl;
		result.push_back(it->first);
		++it;
	}
	
	return result;
}

bool
DeviceManager::pluginIsLoaded(std::string pluginName)
{
	std::map<std::string, Plugin*>::iterator itr = netPlugins->begin();
	
	while(itr != netPlugins->end())
	{		
		if(itr->first.compare(pluginName))
		{
			return true;
		}
		itr++;
	}	
	
	return false;
}


/************************************************
 DEVICES METHODS :
 a set of methods used to manage devices
 note : each method of this set is prepend by 'device'
 ************************************************/

void 
DeviceManager::deviceLoadConfigXml(std::string filename)
{
	XMLNode xMainNode = XMLNode::openFileHelper(filename.c_str(), "deviceManager");
	XMLNode xNode = xMainNode.getChildNode("sendDevices");
	int nChild = xNode.nChildNode();
	std::map<std::string, std::string> deviceParameters;

	for (int i = 0; i < nChild; i++) {
		XMLNode deviceNode = xNode.getChildNode(i);
		int nAttr = deviceNode.nAttribute();

		for (int j = 1; j < nAttr; j++) {
			deviceParameters[deviceNode.getAttributeName(j)] = deviceNode.getAttributeValue(j);
		}

		deviceAdd(deviceNode.getAttributeValue(0), deviceNode.getName(), &deviceParameters);
		deviceParameters.clear();
	}
}

void 
DeviceManager::deviceSetCurrent()
{
	//scan the network using each plugin available (by broadcast for osc, by scan for CopperLan?) to find the devices
	std::map<std::string, Plugin*>::iterator itr = netPlugins->begin();
	while( itr != netPlugins->end()){
		if (itr->second != NULL) {
			itr->second->deviceSetCurrent(netDevices);
		}
	
		itr++;
	}
}

std::map<std::string, Device*>*
DeviceManager::deviceGetCurrent()
{
	return netDevices;
}

Device* 
DeviceManager::deviceGet(std::string deviceName)
{
	std::map<std::string, Device*>::iterator it = this->netDevices->find(deviceName);
	
	if (it == netDevices->end())
		return NULL;
	
	return it->second;
}

int
DeviceManager::deviceAdd(std::string deviceName, std::string pluginToUse, std::map<std::string, std::string> *commParameters)
{	
	std::map<std::string, Device*>::iterator it1 = netDevices->find(deviceName);
	if (it1 != netDevices->end()) {
		std::cout << "DeviceManager::deviceAdd the device name already exists : " << deviceName << std::endl;
		return -1;
	}
	
	std::map<std::string, Plugin*>::iterator it2  = netPlugins->find(pluginToUse);
	
	if (it2 == netPlugins->end()) {
		std::cout << "DeviceManager::deviceAdd the plugin doesn't exist : " << pluginToUse << std::endl;
		return -1;
	} else if (it2->second != NULL) {
		it2->second->deviceAdd(deviceName, commParameters, netDevices);	
		return 0;
	} else {
		return -1;	
	}
}

void 
DeviceManager::deviceRemove(std::string deviceName)
{
	std::map<std::string, Device*>::iterator it = netDevices->find(deviceName);
	
	if (it != netDevices->end()) {
		netDevices->erase(deviceName);
	}
}

bool 
DeviceManager::deviceUnderstandDiscoverRequest(std::string deviceName)
{
	std::map<std::string, Device*>::iterator it = netDevices->find(deviceName);
	
	if (it != netDevices->end()) {
		Device* currentDevice = it->second;
		
		std::map<std::string, Plugin*>::iterator it2 = netPlugins->find(currentDevice->getPluginToUse());
		
		if (it2 != netPlugins->end()) {
			return it2->second->understandDiscoverRequest();
		} else {
			return false;
		}
	}
	
	return false;
}


int 
DeviceManager::deviceSendDiscoverRequest(std::string deviceName, Address address, std::vector<std::string>* returnedNodes, std::vector<std::string>* returnedLeaves, std::vector<std::string>* returnedAttributes)
{
	Device* currentDevice;
	std::map<std::string, Device*>::iterator it;
	std::map<std::string, Plugin*>::iterator it2;
	
	// find the device
	it = netDevices->find(deviceName);
	
	if (it != netDevices->end()) {
		
		currentDevice = it->second;
		
		// find the plugin
		it2 = netPlugins->find(currentDevice->getPluginToUse());
		
		if (it2 != netPlugins->end() && it2->second != NULL)
			return it2->second->deviceSendDiscoverRequest(currentDevice, address, returnedNodes, returnedLeaves, returnedAttributes);
		else
			std::cout << "DeviceManager::deviceSendDiscoverRequest : no plugin for the device named " << deviceName << std::endl;
	}else
		std::cout << "DeviceManager::deviceSendDiscoverRequest : no device named " << deviceName << std::endl;

	return NO_ANSWER;
}

int 
DeviceManager::deviceSendDiscoverRequest(std::string deviceAndAddress, std::vector<std::string>* returnedNodes, std::vector<std::string>* returnedLeaves, std::vector<std::string>* returnedAttributes, std::vector<Value>* returnedValues)
{
	int state;
	unsigned int i;
	Value returnedValue;
	std::string deviceName;
	Address address;
	
	parseDeviceAndAddress(deviceAndAddress, deviceName, address);	
	std::cout << "DeviceManager::deviceSendDiscoverRequest deviceName = " << deviceName << " address = " << address << std::endl;
	
	// send the request
	state = deviceSendDiscoverRequest(deviceName, address, returnedNodes, returnedLeaves, returnedAttributes);
	
	// DEBUG
	std::cout << "DeviceManager::deviceSendDiscoverRequest NODES" << std::endl;
	for (unsigned int i = 0; i < (*returnedNodes).size(); ++i)
		std::cout << "                                       " << (*returnedNodes)[i] << std::endl;

	std::cout << "DeviceManager::deviceSendDiscoverRequest LEAVES" << std::endl;
	for (unsigned int i = 0; i < (*returnedLeaves).size(); ++i)
		std::cout << "                                       " << (*returnedLeaves)[i] << std::endl;

	std::cout << "DeviceManager::deviceSendDiscoverRequest ATTRIBUTES" << std::endl;
	for (unsigned int i = 0; i < (*returnedAttributes).size(); ++i)
		std::cout << "                                       " << (*returnedAttributes)[i] << std::endl;

	
	if(state == ANSWER_RECEIVED){
		
		// for each attribute of the node : get the value
		//returnedValues = new vector<Value>();
		for(i = 0; i < returnedAttributes->size(); i++){
			
			state = deviceSendGetRequest(deviceName, address, returnedAttributes->at(i), &returnedValue);
			
			if(state == ANSWER_RECEIVED)
				returnedValues->push_back(returnedValue);
			else
				returnedValues->push_back(NAMESPACE_NO_VALUE);
		}
	}
	
	std::cout << "DeviceManager::deviceSendDiscoverRequest VALUE" << std::endl;
	for (unsigned int i = 0; i < (*returnedValues).size(); ++i)
		std::cout << "                                       " << (*returnedValues)[i] << std::endl;
	
	return state;
}

int
DeviceManager::deviceSendGetRequest(std::string deviceName, Address address, std::string attribute, Value* returnedValue)
{
	std::map<std::string, Device*>::iterator it;
	Device* currentDevice;
	
	// find the device
	it = netDevices->find(deviceName);
	
	if(it != netDevices->end())
	{
		currentDevice = netDevices->find(deviceName)->second;
		
		// send the request to the device
		return netPlugins->find(it->second->getPluginToUse())->second->deviceSendGetRequest(currentDevice, address, attribute, returnedValue);
	}
	else
		std::cout << "DeviceManager::deviceSendGetRequest no device named " << deviceName << std::endl;
	
	return NO_ANSWER;
}

int 
DeviceManager::deviceSendGetRequest(std::string deviceAndAddress, Value* returnedValue)
{
	int state;
	std::string deviceName;
	Address address;
	
	parseDeviceAndAddress(deviceAndAddress, deviceName, address);	
	std::cout << "DeviceManager::deviceSendGetRequest deviceName = " << deviceName << " address = " << address << std::endl;
	
	// send the request
	state = deviceSendGetRequest(deviceName, address, NAMESPACE_ATTR_VALUE, returnedValue);
	
	// DEBUG
	std::cout << "DeviceManager::deviceSendGetRequest VALUE = " << returnedValue << std::endl;
	
	return state;
}

void 
DeviceManager::deviceSendSetRequest(std::string deviceName, Address address, Value& newValue)
{
	std::map<std::string, Device*>::iterator it;
	Device* currentDevice;
	
	// find the device
	it = netDevices->find(deviceName);
	
	if(it != netDevices->end())
	{
		currentDevice = netDevices->find(deviceName)->second;
		
		// send the request to the device
		netPlugins->find(it->second->getPluginToUse())->second->deviceSendSetRequest(currentDevice, address, newValue);
	}
	else
		std::cout << "DeviceManager::deviceSendSetRequest no device named " << deviceName << std::endl;
}

void 
DeviceManager::deviceSendSetRequest(std::string deviceAndAddress, Value& newValue)
{
	std::string deviceName;
	Address address;
	
	parseDeviceAndAddress(deviceAndAddress, deviceName, address);	
	std::cout << "DeviceManager::deviceSendSetRequest deviceName = " << deviceName << " address = " << address << std::endl;
	
	// send the request
	deviceSendSetRequest(deviceName, address, newValue);
}

void 
DeviceManager::deviceSendSetRequest(std::string deviceAndAddressAndValue)
{
	std::string deviceAndAddress, deviceName;
	Address address;
	Value newValue;
	
	// Parse the value
	unsigned int nbOfSpace = std::count(deviceAndAddressAndValue.begin(), deviceAndAddressAndValue.end(), ' ');
	unsigned int spacePos = deviceAndAddressAndValue.find_first_of(' ');
	
	if((nbOfSpace) && (spacePos != deviceAndAddressAndValue.npos)){
		deviceAndAddress = deviceAndAddressAndValue.substr(0, spacePos);
		newValue = deviceAndAddressAndValue.substr(spacePos, deviceAndAddressAndValue.size());
	}
	else{
		deviceAndAddress = deviceAndAddressAndValue;
		newValue = "";
	}
	
	parseDeviceAndAddress(deviceAndAddress, deviceName, address);
	std::cout << "DeviceManager::deviceSendSetRequest deviceName = " << deviceName << " address = " << address << " value = " << newValue << std::endl;
	
	// send the request
	deviceSendSetRequest(deviceName, address, newValue);
}

void 
DeviceManager::deviceSendListenRequest(std::string deviceName, Address address, std::string attribute, bool enable)
{
	std::map<std::string, Device*>::iterator it;
	Device* currentDevice;
	
	// find the device
	it = netDevices->find(deviceName);
	
	if(it != netDevices->end())
	{
		currentDevice = netDevices->find(deviceName)->second;
		
		// send the request to the device
		netPlugins->find(it->second->getPluginToUse())->second->deviceSendListenRequest(currentDevice, address, attribute, enable);
	}
	else
		std::cout << "DeviceManager::deviceSendListenRequest no device named " << deviceName << std::endl;
}

void 
DeviceManager::deviceSendListenRequest(std::string deviceAndAddress, std::string attribute, bool enable)
{
	std::string deviceName;
	Address address;
	
	parseDeviceAndAddress(deviceAndAddress, deviceName, address);	
	std::cout << "DeviceManager::deviceSendListenRequest deviceName = " << deviceName << " address = " << address << std::endl;
	
	// send the request
	deviceSendListenRequest(deviceName, address, attribute, enable);
}

void 
DeviceManager::deviceSendListenAnswer(std::string deviceName, Address address, std::string attribute, Value& changedValue)
{
	std::map<std::string, Device*>::iterator it;
	Device* currentDevice;
	
	// find the device
	it = netDevices->find(deviceName);
	
	if(it != netDevices->end())
	{
		currentDevice = netDevices->find(deviceName)->second;
		
		// send the request to the device
		netPlugins->find(it->second->getPluginToUse())->second->deviceSendListenAnswer(currentDevice, address, attribute, changedValue);
	}
	else
		std::cout << "DeviceManager::deviceSendListenAnswer no device named " << deviceName << std::endl;
}

void 
DeviceManager::deviceSendListenAnswer(std::string deviceAndAddress, std::string attribute, Value& changedValue)
{
	std::string deviceName;
	Address address;
	
	parseDeviceAndAddress(deviceAndAddress, deviceName, address);	
	std::cout << "DeviceManager::deviceSendListenAnswer deviceName = " << deviceName << " address = " << address << std::endl;
	
	// send the request
	deviceSendListenAnswer(deviceName, address, attribute, changedValue);
}

std::vector<std::string> 
DeviceManager::deviceSnapshot(std::string deviceName, Address address)
{
	Device* currentDevice;
	std::vector<std::string> result;
	std::map<std::string, Device*>::iterator it;
	
	// find the device
	it = netDevices->find(deviceName);
	
	if(it != netDevices->end())
	{
		currentDevice = it->second;
		
		// find the plugin and wait answer from the device
		// Here we used a general recursive process built-in the controller
		result = snapshotProcess(netPlugins->find(currentDevice->getPluginToUse())->second, currentDevice, address);
		
		// edit snapshot by adding the device name before each address of the snapshot
		for(unsigned int i = 0; i < result.size(); ++i)
			result[i] = deviceName + result[i];
	} 
	else
		std::cout << "DeviceManager::deviceSnapshot no device named " << deviceName << std::endl;
	
	return result;
}

std::vector<std::string> 
DeviceManager::deviceSnapshot(std::string deviceAndAddress)
{
	std::vector<std::string> result;
	std::string deviceName;
	Address address;
	
	parseDeviceAndAddress(deviceAndAddress, deviceName, address);	
	std::cout << "DeviceManager::deviceSnapshot deviceName = " << deviceName << " address = " << address << std::endl;
	
	// send the request
	result = deviceSnapshot(deviceName, address);
	
	// DEBUG
	std::cout << "DeviceManager::deviceSnaphot" << std::endl;
	for (unsigned int i = 0; i < result.size(); ++i)
		std::cout << "			" << result[i] << std::endl;
	
	return result;
}

bool 
DeviceManager::deviceIsVisible(std::string deviceName)
{
	std::map<std::string, Device*>::iterator it = netDevices->find(deviceName);
	
	if (it != netDevices->end()) {
		return it->second->isVisible();
	} else {
		std::cout << "DeviceManager::deviceIsVisible no device named : " << deviceName << std::endl;
		return false;	
	}
}


/*********************************************************
 NAMESPACE METHODS :
 a set of methods used to manage the local namespace 
 note : each method of this set is prepend by 'namespace'
 *********************************************************/

void 
DeviceManager::namespaceInit()
{
	m_namespace = new Namespace();
	m_namespace->namespaceInit(m_application_name);
	m_namespace->namespaceRelease();
}

void 
DeviceManager::namespaceLoadXml(std::string filename)
{
	m_namespace->namespaceLoadFromXml(filename);
}

std::string
DeviceManager::namespaceApplicationName()
{
	return m_application_name;
}

void 
DeviceManager::namespaceDiscoverAddCallback(void* arg, void(*pt2Func)(void*, Address, std::vector<std::string>&, std::vector<std::string>&, std::vector<std::string>&))
{
	m_discover_callback = pt2Func;
	m_discover_arguments = arg;
}

void  
DeviceManager::namespaceDiscover(Address whereToDiscover, std::vector<std::string>& returnedNodes, std::vector<std::string>& returnedLeaves, std::vector<std::string>& returnedAttributes)
{
	if(m_discover_callback)
		m_discover_callback(m_discover_arguments , whereToDiscover, returnedNodes, returnedLeaves, returnedAttributes);
	else
		; // TODO : notify the DeviceManager there is no discoverer for this Namespace
}

void 
DeviceManager::namespaceGetAddCallback(void* arg, void(*pt2Func)(void*, Address, std::string, Value&))
{
	m_get_callback = pt2Func;
	m_get_arguments = arg;
}

void  
DeviceManager::namespaceGet(Address whereToGet, std::string attribute, Value& returnedValue)
{
	if(m_get_callback)
		m_get_callback(m_get_arguments , whereToGet, attribute, returnedValue);
	else
		; // TODO : notify the DeviceManager there is no getter for this Namespace
}

void 
DeviceManager::namespaceSetAddCallback(void* arg, void(*pt2Func)(void*, Address, std::string, Value&))
{
	m_set_callback = pt2Func;
	m_set_arguments = arg;
}

void  
DeviceManager::namespaceSet(Address whereToSet, std::string attribute, Value& newValue)
{
	if(m_set_callback)
		m_set_callback(m_set_arguments , whereToSet, attribute, newValue);
	else
		; // TODO : notify the DeviceManager there is no setter for this Namespace
}

void 
DeviceManager::namespaceListenAddCallback(void* arg, void(*pt2Func)(void*, std::string, Address, std::string, bool))
{
	m_listen_callback = pt2Func;
	m_listen_arguments = arg;
}

void  
DeviceManager::namespaceListen(Device* whereToSend, Address whereToListen, std::string attributeToListen, bool enableListening)
{
	if(m_listen_callback)
		m_listen_callback(m_listen_arguments, whereToSend->getName(), whereToListen, attributeToListen, enableListening);
	else
		; // TODO : notify the DeviceManager there is no linker for this Namespace
}


/************************************************
 
 PRIVATE METHODS
 used for internal and specific purposes
 
 ************************************************/
void 
DeviceManager::parseDeviceAndAddress(std::string deviceAndAddress, std::string& device, std::string& address)
{
	// Deal with device name starting with a slash
	// and add a slash in case there is only the device name
	unsigned int nbOfSlash = std::count(deviceAndAddress.begin(), deviceAndAddress.end(), '/');
	unsigned int slashPos = deviceAndAddress.find_first_of('/');
	
	// if deviceAndAddress is something like : "deviceName" or "/deviceName"
	//		add a slash at the end
	if(nbOfSlash == 0 || (nbOfSlash == 1 && (slashPos == 0)))
		deviceAndAddress += "/";
	
	// parse the devicename and the address
	slashPos = deviceAndAddress.find_first_of('/', 1);
	device = deviceAndAddress.substr(0, slashPos);
	address = deviceAndAddress.substr(slashPos, deviceAndAddress.size());
	address += "";
}

std::vector<std::string> 
DeviceManager::snapshotProcess(Plugin *plugin, Device *device, Address address)
{
	unsigned int maxTryAgain = 2;
	unsigned int currentTry = 0;
	unsigned int i;
	int state;
	std::vector<std::string> nodes;
	std::vector<std::string> leaves;
	std::vector<std::string> attributes;
	std::vector<std::string> snapshot;
	std::vector<std::string> snapshotBelow;
	Value getValueAnswer = "";
	std::string currentAttribute, currentLeaf, nodeAddress;
	
	// Ask the namespace at the address
	while(currentTry < maxTryAgain){	// is it necessary to make it several times ? --to
		
		state = plugin->deviceSendDiscoverRequest(device, address, &nodes, &leaves, &attributes);
		
		if(state == ANSWER_RECEIVED)
			currentTry = maxTryAgain;
		else
			++currentTry;
	}
	
	// look for a 'value' attribute and ask the value
	for(unsigned int i = 0; i < attributes.size(); ++i){
		
		currentAttribute = attributes.at(i);
		
		if(currentAttribute == NAMESPACE_ATTR_VALUE) {
			currentTry = 0;
			
			while(currentTry < maxTryAgain){	// is it necessary to make it several times ? --to
				
				state = plugin->deviceSendGetRequest(device, address, currentAttribute, &getValueAnswer);
				
				if(state == ANSWER_RECEIVED){
					
					// add the "address value" string to the snaphot
					if(getValueAnswer != "")
						snapshot.push_back(address + " " + getValueAnswer);
					
					// to the while loop
					currentTry = maxTryAgain;
				}
				else
					++currentTry;
			}
		}
	}
	
	// ask the value of each leaf
	for(unsigned int i = 0; i < leaves.size(); ++i){
		
		currentLeaf = leaves.at(i);
		
		currentTry = 0;
		while(currentTry < maxTryAgain){	// is it necessary to make it several times ? --to
			
			state = plugin->deviceSendGetRequest(device, address + "/" + currentLeaf, NAMESPACE_ATTR_VALUE, &getValueAnswer);
			
			if(state == ANSWER_RECEIVED){
				
				// add the "address/leaf value" string to the snaphot
				if(getValueAnswer != "")
					snapshot.push_back(address + "/" + currentLeaf + " " + getValueAnswer);
				
				// to the while loop
				currentTry = maxTryAgain;
			}
			else
				++currentTry;
		}
	}
	
	// Then do a snapshot for each nodes below the address
	for(i = 0; i < nodes.size(); ++i){
		
		// make the absolute address of the node : /address/node
		if (address != "/")
			nodeAddress = address + "/" + nodes.at(i);
		else
			nodeAddress = address + nodes.at(i);
		
		// do a snapshot on it
		snapshotBelow = snapshotProcess(plugin, device, nodeAddress);
		
		// add the snapshot below to the snapshot
		snapshot.insert(snapshot.end(), snapshotBelow.begin(), snapshotBelow.end());
	}
	
	return snapshot;
}


