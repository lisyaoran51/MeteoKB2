#include "MeteoBitbangDisplayDevice.h"

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


/*
 * 要去Drivers/LedDriver3去sg driver.sh，才能用
 * 不能開spi，會衝突
 */

using namespace Desktop::Devices;


MeteoBitbangDisplayDevice::MeteoBitbangDisplayDevice(int w, int h)
{

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

	lightBoardFileDescriptor = open("/dev/meteo_lightboard_v1", O_WRONLY);
	if (lightBoardFileDescriptor < 0) {

		LOG(LogLevel::Error) << "MeteoBitbangDisplayDevice::MeteoBitbangDisplayDevice() : open driver failed.";
		throw runtime_error("MeteoBitbangDisplayDevice::MeteoBitbangDisplayDevice() : open driver failed.");

	}


}

int MeteoBitbangDisplayDevice::Show(Map * m)
{
	uint8_t** matrix = m->GetMatrix();

	for (int i = 0; i < 96; i++) {
		lightMatrixMessage[i] = 0x0;
	}

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {

			if (matrix[i][height - 1 - j] > 0)
				lightMatrixMessage[j * 6 + i / 8] |= (0x01 << (i % 8));

		}
	}

	int result = write(lightBoardFileDescriptor, lightMatrixMessage, 96);

	if(result < 96)
		LOG(LogLevel::Error) << "MeteoBitbangDisplayDevice::MeteoBitbangDisplayDevice() : write return " << result;

	LOG(LogLevel::Finest) << "MeteoBitbangDisplayDevice::MeteoBitbangDisplayDevice() : write return " << result;

	return 0;
}

int MeteoBitbangDisplayDevice::passToDevice()
{
	return 0;
}
