#ifndef CHANGEMAC_H
#define CHANGEMAC_H
#define _WIN32_DCOM

#include <windows.h>
#include <objbase.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <ctype.h>
#include <WbemCli.h>
#include <comdef.h>
#include <WbemIdl.h>
#include <netcon.h>

#pragma comment(lib, "wbemuuid.lib")

namespace Public_RhyzHomepass{

	class changeMac
	{
	
		public:
		
		std::string searchAdapter(const char * AdapterName);
		
		bool setMac(const char * AdapterBuffer, const char * NewMAC, const char * AdapterNameBuffer);

		bool resetAdapter(const char * AdapterName);

		bool checkMac(const char * AdapterNameBuffer, const char * MAC);

	};
	
}
#endif