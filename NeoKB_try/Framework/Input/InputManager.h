#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H


#include <vector>

using namespace std;


namespace Framework {
namespace Input {

	/// <summary>
	/// ���input�A�åB�o�e��Ҧ��l����
	/// </summary>
	class InputManager : ChildAddable {

	public:

	protected:

		GameHost Host;

		/// <summary>
		/// �p�G��state�A�|��update input queue�A�M��A��̩��ݪ�child��trigger
		/// </summary>
		int HandleNewState(InputState* state);

		vector<InputState*>* GetPendingState();

	private:

		/// <summary>
		/// �\�Jgamehost
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

	};


}}

#endif