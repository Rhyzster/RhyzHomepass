#include "setupNintendoZone3.h"

namespace Public_RhyzHomepass {

	setupNintendoZone3::setupNintendoZone3()
	{
		SSID = "NintendoZone3";
	}

	bool setupNintendoZone3::setUpNetwork()
	{
		DWORD version;
		HANDLE clientHandle;

		//setting ssid

		if (WlanOpenHandle(2, NULL, &version, &clientHandle) == ERROR_SUCCESS) {

			//setting hostednetwork mode to allow
			BOOL mode = true;
			WLAN_HOSTED_NETWORK_REASON failReason;

			if (WlanHostedNetworkSetProperty(clientHandle, wlan_hosted_network_opcode_enable, sizeof(BOOL), &mode, &failReason, NULL) == ERROR_SUCCESS) {
				std::cout << "Network Mode Set to Allow!" << std::endl;
			}
			else {
				std::cout << "Error setting Networks Mode to Allow, Error Code: " << failReason << std::endl;
				return false;
			}

			//setting hostednetwork SSID to NZ@McD
			WLAN_HOSTED_NETWORK_CONNECTION_SETTINGS networkSettings;
			networkSettings.dwMaxNumberOfPeers = 20;
			networkSettings.hostedNetworkSSID.uSSIDLength = strlen(SSID);
			memcpy(networkSettings.hostedNetworkSSID.ucSSID, SSID, strlen(SSID));

			if (WlanHostedNetworkSetProperty(clientHandle, wlan_hosted_network_opcode_connection_settings, sizeof(WLAN_HOSTED_NETWORK_CONNECTION_SETTINGS), &networkSettings, &failReason, NULL) == ERROR_SUCCESS) {
				std::cout << "Network SSID Set to " << SSID << "!" << std::endl;
			}
			else {
				std::cout << "Error setting Networks SSID, Error Code: " << failReason << std::endl;
				return false;
			}

			UCHAR key[32] = { 0xE4, 0xAA, 0x7C, 0x40, 0x20, 0xEE, 0x07, 0xB5, 0x4E, 0x36, 0xF3, 0xE9, 0x64, 0x96, 0xC5, 0x05, 0xFB, 0x40, 0x3A, 0xFF, 0x33, 0x4F, 0x75, 0xB7, 0xC4, 0x24, 0x25, 0x33, 0x0F, 0xE3, 0x94, 0xCA };

			if (WlanHostedNetworkSetSecondaryKey(clientHandle, 32, key, FALSE, FALSE, &failReason, NULL) == ERROR_SUCCESS) {
				std::cout << SSID << " Password Set Successfully!" << std::endl;
			}
			else {
				std::cout << "Error setting Network Secruity Key, Error Code: " << failReason << std::endl;
				return false;
			}

		}
		else {
			std::cout << "Error setting up network. (Unable to get Client WLAN Handle)" << std::endl;
			return false;
		}
		WlanCloseHandle(clientHandle, NULL);

		return true;
	}

	bool setupNintendoZone3::createMacList()
	{
		std::string macs;

		std::ifstream macFile("Macs/NintendoZone3.txt");
		if (macFile.fail()) {
			std::cout << "Error. Cannot Open " << SSID << ".txt File, Please Check it Exists and is in Correct Directory";
			return false;
		}
		else if (macFile.peek() == EOF) {
			std::cout << "Error. The " << SSID << ".txt File is Empty, Please Add some Mac Addresses to it";
			return false;
		}
		else {
			while (macFile) {
				macFile >> macs;
				std::transform(macs.begin(), macs.end(), macs.begin(), ::toupper);
				macList->push_back(macs);
			}
			macFile.close();

			shuffle();

			return true;
		}
	}

}