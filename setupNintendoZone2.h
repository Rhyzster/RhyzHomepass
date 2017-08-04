#ifndef SETUPNINTENDOZONE2_H
#define SETUPNINTENDOZONE2_H

#include "setup.h"

namespace Public_RhyzHomepass {

	class setupNintendoZone2 : public setup
	{

	public:

		setupNintendoZone2();

		bool setUpNetwork();

		bool createMacList();

	};

}

#endif