#include "MeteoDisplayDevice.h"

#include <thread>

using namespace Desktop::Devices;
using namespace std;



MeteoDisplayDevice::MeteoDisplayDevice(int w, int h)
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

	LOG(LogLevel::Fine) << "MeteoDisplayDevice::MeteoDisplayDevice() : start led driver thread.";

	//thread t(&MeteoLightBoardV1::Draw, meteoLightBoard);
	//t.detach();
}

int MeteoDisplayDevice::Show(Map * m)
{
	uint8_t** matrix = m->GetMatrix();

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			lightMatrix[i][j] = matrix[i][j];
		}
	}

	return meteoLightBoard->Display(lightMatrix);
}

int MeteoDisplayDevice::ProcessOutput()
{
	return 0;
}
