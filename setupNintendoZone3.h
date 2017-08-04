#ifndef SETUPNINTENDOZONE3_H
#define SETUPNINTENDOZONE3_H

#include "setup.h"

namespace Public_RhyzHomepass {

	class setupNintendoZone3 : public setup
	{

	public:

		setupNintendoZone3();

		bool setUpNetwork();

		bool createMacList();

	};

}

#endif