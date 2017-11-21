#ifndef SETUP_H
#define SETUP_H

#include <windows.h>
#include <sstream>
#include <fstream>
#include <vector>
#include <locale>
#include <stdlib.h>
#include <wlanapi.h>
#include <algorithm>
#pragma comment(lib, "Wlanapi.lib")

#include "changeMac.h"

namespace Public_RhyzHomepass{
	
class setup
{
	public:
	
	setup();
	
	void startNetwork();
	
	void stopNetwork();

	bool spoofMac(const char * network, int index, std::string networkBuffer);

	bool checkMac(std::string AdapterNameBuffer, int index);
	
	void shuffle();
	
	std::string searchAdapter(const char * AdapterName);
	
	int time(int time);
	
	int macCount();

	void clearMacs();
	
	virtual bool setUpNetwork() = 0;
	
	virtual bool createMacList() = 0;
	
	protected:

	std::vector<std::string> *macList;

	const char * SSID;

	private:
	
	changeMac *newMac;
};
	
}
#endif