#include "setupNZMcD.h"

namespace Public_RhyzHomepass
{
	setupNZMcD::setupNZMcD()
	{
		SSID = "NZ@McD";
	}

	bool setupNZMcD::setUpNetwork()
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

			UCHAR key[32] = { 0x1E, 0xD7, 0xD2, 0x39, 0X0E, 0x76, 0x67, 0xA4, 0xAE, 0xE1, 0xF4, 0xAB, 0x3B, 0x16, 0x45, 0x02, 0x8D, 0x04, 0x10, 0xEE, 0x80, 0x53, 0xCF, 0xDB, 0x71, 0x2D, 0x7C, 0x30, 0x00, 0x46, 0xDD, 0xF6 };

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

	bool setupNZMcD::createMacList()
	{
		long long MACS = 86119736626944;
		std::stringstream macs;

		/*for (int i = 0; i < 160; i++) {
			macs << std::uppercase << std::hex << MACS;
			macList->push_back(macs.str());
			macs.str("");
			MACS++;
		}
		*/
		std::ifstream macFile("Macs/NZ@McD.txt");
		if (macFile.good()) { //txt file exists
			std::string macs;
			while (macFile) {
				macFile >> macs;
				std::transform(macs.begin(), macs.end(), macs.begin(), ::toupper);
				macList->push_back(macs);
			}
		}
		macFile.close();

		shuffle();

		return true;
	}
}