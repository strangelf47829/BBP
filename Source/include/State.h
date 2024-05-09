#ifndef BBP_STD_STATE_H
#define BBP_STD_STATE_H

namespace BBP
{
	namespace std
	{

		class State
		{
		public:
			// Set the state to a known initialized state
			virtual void init() = 0;

			// Resets the state to a known state (duh)
			virtual void Reset() = 0;
		};

	}
}

#endif