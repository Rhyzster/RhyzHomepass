#include "setup.h"

namespace Public_RhyzHomepass{

setup::setup()
{
	newMac = new changeMac();
	macList = new std::vector<std::string>;
}

void setup::startNetwork()
{
	DWORD version;
	HANDLE clientHandle;
	PWLAN_HOSTED_NETWORK_REASON failReason = {};
	
	if(WlanOpenHandle(2, NULL, &version, &clientHandle) == ERROR_SUCCESS){
		if(WlanHostedNetworkForceStart(clientHandle, failReason, NULL) == ERROR_SUCCESS){
			std::cout << std::endl << SSID << " Network Started" << std::endl;
		}else{
			std::cout << "Error Starting Network, Error Code: " << failReason << std::endl;
			WlanCloseHandle(clientHandle, NULL);
			return;
		}
	}else{
		std::cout << "Error Starting Network. (Unable to get Client WLAN Handle)" << std::endl;
		return;
	}
	WlanCloseHandle(clientHandle, NULL);
}

void setup::stopNetwork()
{
   DWORD version;
   HANDLE clientHandle;
   PWLAN_HOSTED_NETWORK_REASON failReason = {};
   
   if(WlanOpenHandle(2, NULL, &version, &clientHandle) == ERROR_SUCCESS){
	   if(WlanHostedNetworkForceStop(clientHandle, failReason, NULL) == ERROR_SUCCESS){
		   std::cout << std::endl << SSID << " Network Stopped\n" << std::endl;
	   }else{
		   std::cout << "Error Stopping Network, Error Code: " << failReason << std::endl;
		   WlanCloseHandle(clientHandle, NULL);
		   return;
	   }
   }else{
	   std::cout << "Error Stopping Network. (Unable to get Client WLAN Handle)" << std::endl;
	   return;
   }
   WlanCloseHandle(clientHandle, NULL);
}

int setup::time(int time)
{
	//wanna add file l8r to dynmically update
	if(time > 0){
		time = time * 60000; //milliseconds to minutes
		return time;
	}else{
		return 130000; //30mins
	}
}

int setup::macCount()
{
	return macList->size();
}

void setup::clearMacs()
{
	macList->clear();
}

void setup::shuffle()
{
	macList->erase(macList->begin()+macCount()-1); //last mac in the txt file gets added twice

	for(int i = 0; i < (int) macList->size(); i++){	
		int index = rand() % (i + 1);
		
		std::string mac = macList->at(index);
		macList->at(index) = macList->at(i);
		macList->at(i) = mac;		
	}
}

bool setup::spoofMac(const char * network, int index, std::string networkBuffer)
{
	return newMac->setMac(network, macList->at(index).c_str(), networkBuffer.c_str());
}

bool setup::checkMac(std::string AdapterNameBuffer, int index)
{
	std::string MAC = macList->at(index);

	for (int i = 0; i < 14; i++) {
		i = i + 2;
		MAC.insert(i, ":");
	}

	return newMac->checkMac(AdapterNameBuffer.c_str(), MAC.c_str());
}

std::string setup::searchAdapter(const char * AdapterName)
{
	return newMac->searchAdapter(AdapterName);
}

}