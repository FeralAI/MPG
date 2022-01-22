#ifndef TUFGAMEPAD_H_
#define TUFGAMEPAD_H_

#include <MPGS.h>

class TUFGamepad : public MPGS
{
	public:
		TUFGamepad(int debounceMS = 5) : MPGS(debounceMS)
		{

		}

		void setup() override;
		void read() override;
};

#endif
