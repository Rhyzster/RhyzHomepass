#ifndef SETUPGAMEMANIAZONE_H
#define SETUPGAMEMANIAZONE_H

#include "setup.h"

namespace Public_RhyzHomepass {

	class setupGameManiaZone : public setup
	{

	public:

		setupGameManiaZone();

		bool setUpNetwork();

		bool createMacList();

	};

}

#endif
