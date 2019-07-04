#ifndef MAIN_INTERFACE_H
#define MAIN_INTERFACE_H

#include <vector>

using namespace std;


namespace Framework {
namespace IO {

	/// <summary>
	/// 抓取輸入的地方，書出的地方
	/// </summary>
	class MainInterface {

		Display* mainDisplay;
		

	public:

		MainInterface();
		
		int RegisterDisplay(Display* d);
		Display* GetDisplay();

	protected:


	private:




	};


}}


#endif