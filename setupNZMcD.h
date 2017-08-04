#ifndef SETUPNZMCD_H
#define SETUPNZMCD_H

#include "setup.h"

namespace Public_RhyzHomepass
{

	class setupNZMcD : public setup
	{
	public:

		setupNZMcD();

		bool setUpNetwork();

		bool createMacList();

	};

}

#endif