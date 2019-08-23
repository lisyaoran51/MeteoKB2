#ifndef PANEL_H
#define PANEL_H



#include <vector>
#include "Peripheral.h"
#include "../../Util/DataStructure/Action.h"
#include "../Input/InputState.h"

using namespace std;
using namespace Util::DataStructure;
using namespace Framework::Input;

namespace Framework {
namespace IO {

	/// <summary>
	/// 抓取輸入的地方，書出的地方
	/// </summary>
	class Panel : public Peripheral {


	public:

		template<class _Type>
		int AddOnButtonDown(_Type* callableObject, function<int(InputState*)> callback, string name);

		template<class _Type>
		int AddOnKnobTurn(_Type* callableObject, function<int(InputState*)> callback, string name);

		template<class _Type>
		int AddOnSliderMove(_Type* callableObject, function<int(InputState*)> callback, string name);

	protected:


	private:

		Action<int(InputState*)> OnButtonDown;

		Action<int(InputState*)> OnKnobTurn;

		Action<int(InputState*)> OnSliderMove;


	};


}}









#endif