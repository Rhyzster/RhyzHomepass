#include "setupNintendoZone1.h"

namespace Public_RhyzHomepass {

	setupNintendoZone1::setupNintendoZone1()
	{
		SSID = "NintendoZone1";
	}

	bool setupNintendoZone1::setUpNetwork()
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

			UCHAR key[32] = { 0x91, 0x1F, 0x9D, 0x8C, 0x83, 0xA9, 0x60, 0xF1, 0xDF, 0xC8, 0x9A, 0x31, 0x9F, 0xA3, 0xAA, 0x56, 0xDE, 0x51, 0x3F, 0x9D, 0xED, 0x90, 0x8B, 0xA7, 0x61, 0xCA, 0xEE, 0xA5, 0x3F, 0x15, 0x7D, 0xA8 };

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

	bool setupNintendoZone1::createMacList()
	{
		std::string macs;
		std::map<std::string, int> macsMap;

		std::ifstream macFile("Macs/NintendoZone1.txt");
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
				if (macs.length() != 12 && macs.length() != 0) {
					std::cout << "Error: The MAC " << macs << " is not 12 characters long. Please Check " << SSID << ".txt file.\n" << std::endl;
				}
				else {
					if (macsMap.find(macs) == macsMap.end()) {
						//add to map
						macsMap.insert(std::pair<std::string, int>(macs, 0));
						macList->push_back(macs);
					}
				}
			}
			macFile.close();

			shuffle();

			

			return true;
		}
	}

}