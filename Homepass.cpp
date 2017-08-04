#include <windows.h>
#include <ctime>

#include "setup.h"
#include "setupNZMcD.h"
#include "setupNintendoSpotpass5.h"
#include "setupGameManiaZone.h"
#include "setupNintendoZonegamelife.h"
#include "setupNintendoZone1.h"
#include "setupNintendoZone2.h"
#include "setupNintendoZone3.h"
#include "setupNintendoZone4.h"

using Public_RhyzHomepass::setup;
using Public_RhyzHomepass::setupNZMcD;
using Public_RhyzHomepass::setupNintendoSpotpass5;
using Public_RhyzHomepass::setupGameManiaZone;
using Public_RhyzHomepass::setupNintendoZonegamelife;
using Public_RhyzHomepass::setupNintendoZone1;
using Public_RhyzHomepass::setupNintendoZone2;
using Public_RhyzHomepass::setupNintendoZone3;
using Public_RhyzHomepass::setupNintendoZone4;

bool checkElevated()
{
		bool elevated = true;
		HANDLE accessToken = NULL;
		TOKEN_ELEVATION Elevation;
		DWORD size = sizeof(TOKEN_ELEVATION);
		if(OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &accessToken)){ //open process token returns a bool if succeds or not
			if(GetTokenInformation(accessToken, TokenElevation, &Elevation, sizeof(Elevation), &size)){
				elevated = Elevation.TokenIsElevated != 0; //if returns anything not equal to 0 will not have admin privelges
			}
			if(accessToken){
				CloseHandle(accessToken);
			}
		}
		return elevated;
}

int main(int argc, char *argv[])
{	
	std::cout << "\nRhyzHomePass v2.0\n" << std::endl;
	
	//bunch of error checking
	if(!checkElevated()){
		std::cout << "You need to run this program as Administrator." << std::endl;
		return -1;
	}

	if(argc > 3){ 
		std::cout << "Error you have entered too many arguments" << std::endl << "The two arguments you should enter are Adapter Name followed by waiting time between Mac Address Changes";
		return -1;
	}else if(argc < 3){
		std::cout << "Error you have not entered enough arguments" << std::endl << "The two arguments you should enter are Adapter Name followed by waiting time between Mac Address Changes";
		return -1;
	}
	if(atoi(argv[2]) < 3){
		std::cout << "Cannot have a waiting time less than 3. (3 Minutes is quickest you can run at for maximum streetpasses, needs to be whole number entered too, eg 30 for 30 minutes)" << std::endl;
		return -1;
	}
	
	setup **homePass = new setup*[8]; //polymorphic array 

	homePass[0] = new setupNZMcD();
	homePass[1] = new setupNintendoSpotpass5();
	homePass[2] = new setupGameManiaZone();
	homePass[3] = new setupNintendoZonegamelife();
	homePass[4] = new setupNintendoZone1();
	homePass[5] = new setupNintendoZone2();
	homePass[6] = new setupNintendoZone3();
	homePass[7] = new setupNintendoZone4();
	
	std::string connectionBuffer = homePass[0]->searchAdapter(argv[1]); //checking to see if can find adapter given, doesnt matter what homepass object is used

	if(connectionBuffer != ""){ //check an adapter is returned
		std::cout << "Found Adapter!\n" << std::endl;
	}else{
		std::cout << "Couldn't find Adapter, check " << argv[1] << " is the correct network Adapter Name." << std::endl;
		return -1;
	} 

	//need to check here can enable and disbale adpater too stop people inputting wrong adapter

	//end of error checking
	
	srand((unsigned int)time(NULL)); //create good random seed
	
	while (2 > 1) { //loop forever

		for (int j = 0; j < 8; j++) {

			if (!homePass[j]->createMacList()) {
				return -1;
			}

			for (int i = 0; i < homePass[j]->macCount(); i++) {

				if (!homePass[j]->setUpNetwork()) { //set up network, do this each loop, as key isnt stored for more than one use and incase user plays around with the hostednetwork
					return -1;
				}

				if (homePass[j]->spoofMac(argv[1], i, connectionBuffer)) { //checking if mac address function changed mac address in registry at least
					Sleep(3000);
					homePass[j]->startNetwork();
					Sleep(homePass[j]->time(atoi(argv[2])));
					homePass[j]->stopNetwork();
					Sleep(3000);
				}
				else {
					return -1;
				}
			}
			homePass[j]->clearMacs();
		}
	}

	return 0;
}