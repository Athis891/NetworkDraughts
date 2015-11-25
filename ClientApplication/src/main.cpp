#include "Draughts.h"


int main()
{
	Draughts application;

	if (application.startup() == true)
	{
		while (application.update() == true)
		{
			application.draw();
		}

		application.shutdown();
	}

	return 0;
}
