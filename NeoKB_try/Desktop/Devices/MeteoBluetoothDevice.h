#ifndef METEO_BLUETOOTH_DEVICE_H
#define METEO_BLUETOOTH_DEVICE_H

#include "../../Framework/Devices/BluetoothDevice.h"
#include "MeteoBluetoothPhoneV1.h"

using namespace Framework::Devices;


/*
 * 有很多人在討論virtual template function要怎麼寫
 * https://stackoverflow.com/questions/5871722/how-to-achieve-virtual-template-function-in-c/5872226
 */


namespace Desktop {
namespace Devices {

	class MeteoBluetoothDevice : public BluetoothDevice {

		MeteoBluetoothPhoneV1* meteoBluetoothPhone = nullptr;

	public:

		MeteoBluetoothDevice(MeteoBluetoothPhoneV1* mBluetoothPhone);

		/// <summary>
		/// 寫入sm檔時的callback
		/// </summary>
		virtual int AddOnStartWritingSmFile(MtoObject* callableObject, function<int(string)> callback, string name);
		virtual int AddOnWriteSmFileSuccess(MtoObject* callableObject, function<int(string)> callback, string name);

		/// <summary>
		/// 寫入新音色時的callback
		/// </summary>
		virtual int AddOnStartWritingSoundFilePackage(MtoObject* callableObject, function<int(string)> callback, string name);
		virtual int AddOnWriteSoundFilePackageSuccess(MtoObject* callableObject, function<int(string)> callback, string name);

		/// <summary>
		/// 寫入新主程式時的callback
		/// </summary>
		virtual int AddOnStartWritingProgram(MtoObject* callableObject, function<int(string)> callback, string name);
		virtual int AddOnWriteProgramSuccess(MtoObject* callableObject, function<int(string)> callback, string name);

	protected:

		virtual int readFromDevice();

		virtual int passToDevice();

	};

}}





#endif