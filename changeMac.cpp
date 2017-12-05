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
	
	return resetAdapter(AdapterName); //check adapter has been reset correctly

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
		size_t size = strlen(AdapterName) + 1;
		wchar_t* wAdapterName = new wchar_t[size];

		size_t outSize;

		mbstowcs_s(&outSize, wAdapterName, size, AdapterName, size - 1); //convert adapter name to wchar_t for comparison

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

bool changeMac::checkMac(const char * AdapterNameBuffer, const char * MAC) //looks up in Windows WMI for MAC Address, as registry wont say if mac correct or not
{
	HRESULT hres;

	hres = CoInitializeEx(0, COINIT_MULTITHREADED); // Initialize COM
	if (FAILED(hres))
	{
		std::cout << "Error Checking MAC Address Changed Correctly - Failed to initialize COM library. Error code = 0x" << std::hex << hres << std::endl;
		return false;  
	}

	hres = CoInitializeSecurity( //Set general COM security levels
		NULL,
		-1,                          // COM authentication
		NULL,                        // Authentication services
		NULL,                        // Reserved
		RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication 
		RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation  
		NULL,                        // Authentication info
		EOAC_NONE,                   // Additional capabilities 
		NULL                         // Reserved
	);


	if (FAILED(hres))
	{
	std::cout << "Error Checking MAC Address Changed Correctly - Failed to initialize security. Error code = 0x" << std::hex << hres << std::endl;
		CoUninitialize();
		return false;               
	}

	IWbemLocator *pLoc = NULL; // Obtain the initial locator to WMI

	hres = CoCreateInstance(
		CLSID_WbemLocator,
		0,
		CLSCTX_INPROC_SERVER,
		IID_IWbemLocator, (LPVOID *)&pLoc);

	if (FAILED(hres))
	{
		std::cout << "Error Checking MAC Address Changed Correctly - Failed to create IWbemLocator object. Error code = 0x" << std::hex << hres << std::endl;
		CoUninitialize();
		return false;              
	}

	// Connect to WMI through the IWbemLocator::ConnectServer method

	IWbemServices *pSvc = NULL;

	// Connect to the root\cimv2 namespace with
	// the current user and obtain pointer pSvc
	// to make IWbemServices calls.
	hres = pLoc->ConnectServer(
		_bstr_t(L"ROOT\\CIMV2"), // Object path of WMI namespace
		NULL,                    // User name. NULL = current user
		NULL,                    // User password. NULL = current
		0,                       // Locale. NULL indicates current
		NULL,                    // Security flags.
		0,                       // Authority (for example, Kerberos)
		0,                       // Context object 
		&pSvc                    // pointer to IWbemServices proxy
	);

	if (FAILED(hres))
	{
		std::cout << "Error Checking MAC Address Changed Correctly - Could not connect. Error code = 0x" << std::hex << hres << std::endl;
		pLoc->Release();
		CoUninitialize();
		return false;               
	}

	// Set security levels on the proxy 

	hres = CoSetProxyBlanket(
		pSvc,                        // Indicates the proxy to set
		RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
		RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
		NULL,                        // Server principal name 
		RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx 
		RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
		NULL,                        // client identity
		EOAC_NONE                    // proxy capabilities 
	);

	if (FAILED(hres))
	{
		std::cout << "Error Checking MAC Address Changed Correctly - Could not set proxy blanket. Error code = 0x" << std::hex << hres << std::endl;
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return false;
	}

	// Use the IWbemServices pointer to make requests of WMI

	IEnumWbemClassObject* pEnumerator = NULL;
	hres = pSvc->ExecQuery(
		bstr_t("WQL"),
		bstr_t("SELECT * FROM Win32_NetworkAdapterConfiguration WHERE SettingID = '" + bstr_t(AdapterNameBuffer) + "'"),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		NULL,
		&pEnumerator);

	if (FAILED(hres))
	{
		std::cout << "Error Checking MAC Address Changed Correctly - Query failed. Error code = 0x" << std::hex << hres << std::endl;
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return false; 
	}

	// Get the data from the query

	IWbemClassObject *pclsObj = NULL;
	ULONG uReturn = 0;
	bool correctMac;

	HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1,
			&pclsObj, &uReturn);

	if (uReturn != 0) {
		VARIANT vtProp;
		hr = pclsObj->Get(L"MACAddress", 0, &vtProp, 0, 0);
		if (0 == wcscmp(vtProp.bstrVal, bstr_t((MAC)))) {
			std::cout << "MAC Changed Correctly" << std::endl;
			correctMac = true;
		}else{
			std::cout << "MAC did not change correctly, Skipping this MAC\n" << std::endl;
			correctMac = false;
		}
		VariantClear(&vtProp);
		pclsObj->Release();
	}
	else {
		std::cout << "Error Checking MAC Address Changed Correctly - Unable to Locate Network Details" << std::endl;
	}

	// Cleanup

	pSvc->Release();
	pLoc->Release();
	pEnumerator->Release();
	CoUninitialize();

	return correctMac;

}

}