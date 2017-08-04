#ifndef SETUPNINTENDOZONE4_H
#define SETUPNINTENDOZONE4_H

#include "setup.h"

namespace Public_RhyzHomepass {

	class setupNintendoZone4 : public setup
	{

	public:

		setupNintendoZone4();

		bool setUpNetwork();

		bool createMacList();

	};

}

#endif