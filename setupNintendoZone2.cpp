#include "setupNintendoZone2.h"

namespace Public_RhyzHomepass {

	setupNintendoZone2::setupNintendoZone2()
	{
		SSID = "NintendoZone2";
	}

	bool setupNintendoZone2::setUpNetwork()
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

			UCHAR key[32] = { 0x28, 0x1D, 0X13, 0x02, 0x93, 0xFC, 0x0E, 0xF1, 0x7E, 0x5A, 0xD9, 0x3B, 0xE4, 0x6F, 0xBB, 0xBF, 0xFE, 0x78, 0x0E, 0x45, 0x75, 0x20, 0x6F, 0x73, 0xC9, 0x1B, 0xB5, 0x53, 0xDA, 0x20, 0x2C, 0x4F };

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

	bool setupNintendoZone2::createMacList()
	{
		std::string macs;

		std::ifstream macFile("Macs/NintendoZone2.txt");
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