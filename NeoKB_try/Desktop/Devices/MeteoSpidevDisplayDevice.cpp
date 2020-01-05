#include "MeteoSpidevDisplayDevice.h"

#include <thread>

using namespace Desktop::Devices;
using namespace std;



MeteoSpidevDisplayDevice::MeteoSpidevDisplayDevice(int w, int h)
{
	meteoLightBoard = new MeteoLightBoardV1(w, h);
	width = w;
	height = h;

	lightMatrix = (uint8_t**)new uint8_t*[width];
	uint8_t* pData = (uint8_t*)new uint8_t[width * height];
	for (int i = 0; i < width; i++, pData += height)
		lightMatrix[i] = pData;

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			lightMatrix[i][j] = 0;
		}
	}

	LOG(LogLevel::Fine) << "MeteoSpidevDisplayDevice::MeteoSpidevDisplayDevice() : start led driver thread.";

	thread t(&MeteoLightBoardV1::Draw, meteoLightBoard);
	t.detach();
}

int MeteoSpidevDisplayDevice::Show(Map * m)
{
	LOG(LogLevel::Finest) << "MeteoSpidevDisplayDevice::Show() : copying map.";

	uint8_t** matrix = m->GetMatrix();

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			lightMatrix[i][j] = matrix[i][j];
		}
	}

	return meteoLightBoard->Display(lightMatrix);
	return 0;
}

int MeteoSpidevDisplayDevice::passToDevice()
{
	return 0;
}
