#ifndef PERIPHERAL_H
#define PERIPHERAL_H



#include <vector>

using namespace std;


namespace Framework {
namespace IO {

	/// <summary>
	/// 抓取輸入的地方，書出的地方
	/// </summary>
	class Peripheral {


	public:

		virtual int TriggerOnInput() = 0;

	protected:


	private:




	};


}}









#endif