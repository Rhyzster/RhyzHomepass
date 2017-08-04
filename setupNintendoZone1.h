#ifndef SETUPNINTENDOZONE1_H
#define SETUPNINTENDOZONE1_H

#include "setup.h"

namespace Public_RhyzHomepass {

	class setupNintendoZone1 : public setup
	{

	public:

		setupNintendoZone1();

		bool setUpNetwork();

		bool createMacList();

	};

}

#endif