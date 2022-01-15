#include <MPG.h>

class Gamepad : public MPG {
	public:
		Gamepad(int debounceMS = 5) : MPG(debounceMS)
		{
		}

		void setup() override;

		/**
		 * @brief Retrieve the inputs and save to the current state. Derived classes must overide this member.
		 */
		void read() override;
};
