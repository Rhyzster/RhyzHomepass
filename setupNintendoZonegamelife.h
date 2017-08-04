#ifndef SETUPNINTENDOZONEGAMELIFE_H
#define SETUPNINTENDOZONEGAMELIFE_H

#include "setup.h"

namespace Public_RhyzHomepass {



	class setupNintendoZonegamelife : public setup
	{

	public:

		setupNintendoZonegamelife();

		bool setUpNetwork();

		bool createMacList();

	};

}

#endif