#include "changeMac.h"

namespace Public_RhyzHomepass{
	
std::string changeMac::searchAdapter(const char * AdapterName)
{
	HKEY hListKey = NULL;
	HKEY hKey = NULL;
	char connectionBuffer[255]; 
	char fullConnectionBuffer[255];
	DWORD bufferSize = 255;
	FILETIME lastWriteTime;
	int i = 0;
	bool found = false;
	
	RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Network\\{4D36E972-E325-11CE-BFC1-08002BE10318}", 0, KEY_READ, &hListKey); //opens specific registry
	if(!hListKey){ 
		std::cout << "Error, opening registry";
		return "";
	}
	while(RegEnumKeyEx(hListKey, i++, connectionBuffer, &bufferSize, NULL, NULL, NULL, &lastWriteTime) == ERROR_SUCCESS){ //go through all subkeys of above registry
		snprintf(fullConnectionBuffer, 255, "%s\\Connection", connectionBuffer); //create directory to open
		RegOpenKeyEx(hListKey, fullConnectionBuffer, 0, KEY_READ, &hKey); //open each of these subkeys and check names
		if(hKey){
			//check can get reg value
			if(RegQueryValueEx(hKey, "Name", NULL, NULL, (LPBYTE)fullConnectionBuffer, &bufferSize) == ERROR_SUCCESS && strcmp(fullConnectionBuffer, AdapterName) == 0){
				found = true;
				break;
			} 
		RegCloseKey(hKey);
		}
		bufferSize = 255;
	}
	RegCloseKey(hKey);
	RegCloseKey(hListKey);
	if(!found){
		return "";
	}else{
		std::string ConnectionBuffer(connectionBuffer);
		return ConnectionBuffer;
	}
}

bool changeMac::setMac(const char * AdapterName, const char * NewMAC, const char * AdapterNameBuffer)
{
	HKEY hListKey = NULL;
	HKEY hKey = NULL;
	char fullConnectionBuffer[255];
	DWORD bufferSize = 255;
	FILETIME lastWriteTime;
	int i = 0;
	char c;
	bool found = false;

	std::cout << "\nChanging Mac Address to ";
	while(NewMAC[i]){
		c=NewMAC[i];
		putchar (toupper(c));
		i++;
	}
	std::cout << std::endl;
	RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E972-E325-11CE-BFC1-08002BE10318}", 0, KEY_READ, &hListKey);
	if(!hListKey){
		std::cout << "Couldn't change MAC Address. Error, opening registry";
		return found;
	}
	i = 0;
	while(RegEnumKeyEx(hListKey, i++, fullConnectionBuffer, &bufferSize, NULL, NULL, NULL, &lastWriteTime) == ERROR_SUCCESS){
		RegOpenKeyEx(hListKey, fullConnectionBuffer, 0, KEY_READ | KEY_SET_VALUE, &hKey);	
		if(hKey){
			bufferSize = 255;
			if(RegQueryValueEx(hKey, "NetCfgInstanceId", NULL, NULL, (LPBYTE)fullConnectionBuffer, &bufferSize) == ERROR_SUCCESS && strcmp(fullConnectionBuffer, AdapterNameBuffer) == 0){
				RegSetValueEx(hKey, "NetworkAddress", 0, REG_SZ, (LPBYTE)NewMAC, strlen(NewMAC) + 1); //changing registry entry NetworkAddress
				found = true; //has found right registry entry and made bool equal to true
				break; //break out of while loop
			}
		RegCloseKey(hKey);
		}
		bufferSize = 255;
	}
	RegCloseKey(hKey);
	RegCloseKey(hListKey);
	if(!found){
		std::cout << "Can't change MAC Address" << std::endl;
		return found;
	}
	
	return resetAdapter(AdapterName);
}

bool changeMac::resetAdapter(const char * AdapterName)
{
	bool found = false; //mainly used to test whether or not network is found during enumeration of networks
	CoInitialize(0);
	//create uninitialized object of network conneciton manager
	INetConnectionManager* pConnectionManager; //pointer to network connection manager
	HRESULT hr = CoCreateInstance(CLSID_ConnectionManager, NULL, CLSCTX_SERVER, IID_INetConnectionManager, (void**)&pConnectionManager);
	if (hr != S_OK) {
		std::cout << "Error Resetting Adapter: INetConnectionManager Object Error"; //error making the Network connection manager object
		return found;
	}
	//std::cout << "here";
	IEnumNetConnection* pEnum; //pointer to enumerate through all network connections 
	hr = pConnectionManager->EnumConnections(NCME_DEFAULT, &pEnum);
	if (hr != S_OK) {
		std::cout << "Error Resetting Adapter: Enumeration Error";
		return found;
	}
	else {
		INetConnection* pNetConnection;
		ULONG fetched;
		NETCON_PROPERTIES* pNetProperties;
		wchar_t wAdapterName[50];

		mbstowcs(wAdapterName, AdapterName, strlen(AdapterName) + 1); //convert adapter name to wchar_t for comparison

		while (pEnum->Next(1, &pNetConnection, &fetched) == S_OK) { //go through all connections
			pNetConnection->GetProperties(&pNetProperties); //get propterites of connections
			if (wcscmp(pNetProperties->pszwName, wAdapterName) == 0) { //check if the correct connection
				pNetConnection->Disconnect(); //disconnect
				pNetConnection->Connect(); //connect
				found = true;
				break; //break out of while loop
			}
		}
		CoTaskMemFree(pNetProperties); //free memory
		pEnum->Release();
		pConnectionManager->Release();
	}
	CoUninitialize();
	if (!found) {
		std::cout << "Error Resetting Adapter: Error Finding Network During Enumeration";
	}
	return found;
}

}