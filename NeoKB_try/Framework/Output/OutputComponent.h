#ifndef OUTPUT_COMPONENT_H
#define OUTPUT_COMPONENT_H


#include "../IO/MainInterface.h"


using namespace Framework::IO;


namespace Framework {
namespace Output{

	class OutputComponent {

	public:

		virtual int PushMessage(OutputMessage* outputMessage) = 0;

		/// <summary>
		/// �����bAdd item�ɰ���A���M�|�S���\��
		/// </summary>
		virtual int SetupPeripheral(MainInterface* mainInterface) = 0;

	};

	template<typename T>
	class TOutputComponent : virtual public OutputComponent {

	public:

		

		virtual int PushMessage(OutputMessage* outputMessage) {
			if (dynamic_cast<T*>(outputMessage)) {
				pushMessage(dynamic_cast<T*>(outputMessage));
			}
			return 0;
		}

		

	protected:

		virtual int pushMessage(T* outputMessag) = 0;

	};


}}






#endif
