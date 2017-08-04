#ifndef SETUPNINTENDOSPOTPASS5_H
#define SETUPNINTENDOSPOTPASS5_H

#include "setup.h"

namespace Public_RhyzHomepass {


	class setupNintendoSpotpass5 : public setup
	{
	public:

		setupNintendoSpotpass5();

		bool setUpNetwork();

		bool createMacList();

	};

}


#endif


