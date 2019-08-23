#ifndef DISPLAY_H
#define DISPLAY_H

#include <vector>
#include "Peripheral.h"
#include "../Graphic/Map/Map.h"

using namespace std;
using namespace Framework::Graphic::Maps;


namespace Framework {
namespace IO {

	/// <summary>
	/// 抓取輸入的地方，書出的地方
	/// </summary>
	class Display : public Peripheral {

		

	public:

		Display();
		
		int Show(Map* m);

	protected:


	private:




	};


}}


#endif