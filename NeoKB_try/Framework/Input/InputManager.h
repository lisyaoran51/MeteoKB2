#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H


#include <vector>

#include "../../Framework/Allocation/Hierachal/Triggerable.h"


using namespace std;
using namespace Framework::Allocation::Hierachal;


namespace Framework {
namespace Input {

	/// <summary>
	/// 抓取input，並且發送到所有子物件
	/// </summary>
	class InputManager : public Triggerable {

	public:

		InputManager();

	protected:

		GameHost* host;

		/// <summary>
		/// 如果有state，會先update input queue，然後再找最底端的child來trigger
		/// </summary>
		int HandleNewState(InputState* state);

		vector<InputState*>* GetPendingState();

	private:

		/// <summary>
		/// 擺入gamehost
		/// </summary>
		int load();

		int Update();



		vector<InputState*>* createDistinctInputStates(vector<InputState*>* states);

		virtual int TransformState(InputState* inputState);
		
		vector<Triggerable*> gameInputQueue;

		vector<Triggerable*> pianoInputQueue;

		int updateInputQueue(InputState* inputState);

		int updateKeyboardEvents(InputState* inputState);

		int updatePanelEvents(InputState* inputState);

		int handleKeyDown(InputState* state, Key key);

		int propagateKeyDown(InputState* state, Key key);

		int handleKeyUp(InputState* state, Key key);

		int propagateKeyUp(InputState* state, Key key);

		int handleKnobCounterClockwise(InputState* state, Knob knob);

		int propagateKnobCounterClockwise(InputState* state, Knob knob);



	};


}}

#endif