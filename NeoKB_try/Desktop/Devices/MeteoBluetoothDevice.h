#ifndef METEO_BLUETOOTH_DEVICE_H
#define METEO_BLUETOOTH_DEVICE_H

#include "../../Framework/Devices/BluetoothDevice.h"
#include "MeteoBluetoothPhoneV2.h"

using namespace Framework::Devices;


/*
 * ���ܦh�H�b�Q��virtual template function�n���g
 * https://stackoverflow.com/questions/5871722/how-to-achieve-virtual-template-function-in-c/5872226
 */


namespace Desktop {
namespace Devices {

	class MeteoBluetoothDevice : public BluetoothDevice {

		MeteoBluetoothPhoneV2* meteoBluetoothPhone = nullptr;

		int _debugCount = 0;

	public:

		MeteoBluetoothDevice(MeteoBluetoothPhoneV2* mBluetoothPhone);

		/// <summary>
		/// �ݦ��S���S�᧹��file segment message�A�o���٨S�g�A����n�s��MeteoBluetoothPhoneV1�h�ˬd
		/// </summary>
		virtual bool CheckFileSegmentMessageOutputClear();

		int AddOnConnect(MtoObject * callableObject, function<int()> callback, string name = "MeteoBluetoothDevice::HandleConnect");

		int AddOnDisconnect(MtoObject * callableObject, function<int()> callback, string name = "MeteoBluetoothDevice::HandleDisconnect");

		/// <summary>
		/// �g�Jsm�ɮɪ�callback�A�令�\�brequest��
		/// </summary>
		//virtual int AddOnStartWritingSmFile(MtoObject* callableObject, function<int(string)> callback, string name);
		//virtual int AddOnWriteSmFileSuccess(MtoObject* callableObject, function<int(string)> callback, string name);

		/// <summary>
		/// �g�J�s����ɪ�callback�A�令�\�brequest��
		/// </summary>
		//virtual int AddOnStartWritingSoundFilePackage(MtoObject* callableObject, function<int(string)> callback, string name);
		//virtual int AddOnWriteSoundFilePackageSuccess(MtoObject* callableObject, function<int(string)> callback, string name);

		/// <summary>
		/// �g�J�s�D�{���ɪ�callback�A�令�\�brequest��
		/// </summary>
		//virtual int AddOnStartWritingProgram(MtoObject* callableObject, function<int(string)> callback, string name);
		//virtual int AddOnWriteProgramSuccess(MtoObject* callableObject, function<int(string)> callback, string name);

	protected:

		virtual int readFromDevice();

		virtual int passToDevice();

	};

}}





#endif