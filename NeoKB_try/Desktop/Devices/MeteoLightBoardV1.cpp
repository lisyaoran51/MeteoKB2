#include "MeteoLightBoardV1.h"

#include "../../Util/Log.h"
#include <string.h>
#include <chrono>

using namespace Desktop::Devices;
using namespace Util;
using namespace std;
using namespace std::chrono;

MeteoLightBoardV1::MeteoLightBoardV1(int w, int h)
{
	LOG(LogLevel::Fine) << "MeteoLightBoardV1::MeteoLightBoardV1(w,h) : constructor.";

	if (!bcm2835_init()) {
		LOG(LogLevel::Error) << "MeteoLightBoardV1::MeteoLightBoardV1(w,h) : Failed to initialize bcm2835.";
	}

	LOG(LogLevel::Fine) << "MeteoLightBoardV1::MeteoLightBoardV1(w,h) : bcm2835 inited.";
	bcm2835_gpio_fsel(PA_PIN, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(PB_PIN, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(NA_PIN, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(NB_PIN, BCM2835_GPIO_FSEL_OUTP);

	width = w;
	height = h;


	//unique_lock<mutex> uLock(matrixMutex);

	LOG(LogLevel::Fine) << "MeteoLightBoardV1::MeteoLightBoardV1(w,h) : setting matrix.";

	matrix = (uint8_t**)new uint8_t*[height];
	uint8_t* pData = (uint8_t*)new uint8_t[width / 8 * height];
	for (int i = 0; i < height; i++, pData += width / 8)
		matrix[i] = pData;

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width / 8; j++) {
			matrix[i][j] = 0xFF;
		}
	}

	//uLock.unlock();

	LOG(LogLevel::Fine) << "MeteoLightBoardV1::MeteoLightBoardV1(w,h) : Open spi device.";

	spi_fd = open("/dev/spidev0.0", O_RDWR);
	if (spi_fd < 0) {
		LOG(LogLevel::Error) << "MeteoLightBoardV1::MeteoLightBoardV1 : Fail to open spi device.";
	}

	// spi mode
	// https://os.mbed.com/users/hainjedaf/code/SPI_4x7LED/file/f16387ae15af/main.cpp/
	uint8_t mode = 3;
	uint8_t bits = 8;
	uint32_t speed = 10000000;

	LOG(LogLevel::Fine) << "MeteoLightBoardV1::MeteoLightBoardV1(w,h) : setting spi parameters.";

	// spi mode
	if (ioctl(spi_fd, SPI_IOC_WR_MODE, &mode) < 0)
	{
		LOG(LogLevel::Error) << "MeteoLightBoardV1::MeteoLightBoardV1 : Fail to set spi mode.";
	}
	if (ioctl(spi_fd, SPI_IOC_RD_MODE, &mode) < 0)
	{
		LOG(LogLevel::Error) << "MeteoLightBoardV1::MeteoLightBoardV1 : Fail to set spi mode.";
	}

	// Bits per word
	if (ioctl(spi_fd, SPI_IOC_WR_BITS_PER_WORD, &bits) < 0)
	{
		LOG(LogLevel::Error) << "MeteoLightBoardV1::MeteoLightBoardV1 : Fail to set spi bits.";
	}
	if (ioctl(spi_fd, SPI_IOC_RD_BITS_PER_WORD, &bits) < 0)
	{
		LOG(LogLevel::Error) << "MeteoLightBoardV1::MeteoLightBoardV1 : Fail to set spi bits.";
	}

	// Max speed Hz
	if (ioctl(spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0)
	{
		LOG(LogLevel::Error) << "MeteoLightBoardV1::MeteoLightBoardV1 : Fail to set spi speed.";
	}
	if (ioctl(spi_fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed) < 0)
	{
		LOG(LogLevel::Error) << "MeteoLightBoardV1::MeteoLightBoardV1 : Fail to set spi speed.";
	}

	startTime = system_clock::now();

}


int MeteoLightBoardV1::switchRow(int row)
{
	switch (row) {
	case 0:
		bcm2835_gpio_write(PA_PIN, LOW);
		bcm2835_gpio_write(PB_PIN, LOW);
		bcm2835_gpio_write(NA_PIN, LOW);
		bcm2835_gpio_write(NB_PIN, LOW);
		break;

	case 1:
		bcm2835_gpio_write(PA_PIN, LOW);
		bcm2835_gpio_write(PB_PIN, LOW);
		bcm2835_gpio_write(NA_PIN, HIGH);
		bcm2835_gpio_write(NB_PIN, LOW);
		break;

	case 2:
		bcm2835_gpio_write(PA_PIN, LOW);
		bcm2835_gpio_write(PB_PIN, LOW);
		bcm2835_gpio_write(NA_PIN, HIGH);
		bcm2835_gpio_write(NB_PIN, HIGH);
		break;

	case 3:
		bcm2835_gpio_write(PA_PIN, LOW);
		bcm2835_gpio_write(PB_PIN, LOW);
		bcm2835_gpio_write(NA_PIN, LOW);
		bcm2835_gpio_write(NB_PIN, HIGH);
		break;

	case 4:
		bcm2835_gpio_write(PA_PIN, HIGH);
		bcm2835_gpio_write(PB_PIN, LOW);
		bcm2835_gpio_write(NA_PIN, LOW);
		bcm2835_gpio_write(NB_PIN, LOW);
		break;

	case 5:
		bcm2835_gpio_write(PA_PIN, HIGH);
		bcm2835_gpio_write(PB_PIN, LOW);
		bcm2835_gpio_write(NA_PIN, HIGH);
		bcm2835_gpio_write(NB_PIN, LOW);
		break;

	case 6:
		bcm2835_gpio_write(PA_PIN, HIGH);
		bcm2835_gpio_write(PB_PIN, LOW);
		bcm2835_gpio_write(NA_PIN, HIGH);
		bcm2835_gpio_write(NB_PIN, HIGH);
		break;

	case 7:
		bcm2835_gpio_write(PA_PIN, HIGH);
		bcm2835_gpio_write(PB_PIN, LOW);
		bcm2835_gpio_write(NA_PIN, LOW);
		bcm2835_gpio_write(NB_PIN, HIGH);
		break;

	case 8:
		bcm2835_gpio_write(PA_PIN, HIGH);
		bcm2835_gpio_write(PB_PIN, HIGH);
		bcm2835_gpio_write(NA_PIN, LOW);
		bcm2835_gpio_write(NB_PIN, LOW);
		break;

	case 9:
		bcm2835_gpio_write(PA_PIN, HIGH);
		bcm2835_gpio_write(PB_PIN, HIGH);
		bcm2835_gpio_write(NA_PIN, HIGH);
		bcm2835_gpio_write(NB_PIN, LOW);
		break;

	case 10:
		bcm2835_gpio_write(PA_PIN, HIGH);
		bcm2835_gpio_write(PB_PIN, HIGH);
		bcm2835_gpio_write(NA_PIN, HIGH);
		bcm2835_gpio_write(NB_PIN, HIGH);
		break;

	case 11:
		bcm2835_gpio_write(PA_PIN, HIGH);
		bcm2835_gpio_write(PB_PIN, HIGH);
		bcm2835_gpio_write(NA_PIN, LOW);
		bcm2835_gpio_write(NB_PIN, HIGH);
		break;

	case 12:
		bcm2835_gpio_write(PA_PIN, LOW);
		bcm2835_gpio_write(PB_PIN, HIGH);
		bcm2835_gpio_write(NA_PIN, LOW);
		bcm2835_gpio_write(NB_PIN, LOW);
		break;

	case 13:
		bcm2835_gpio_write(PA_PIN, LOW);
		bcm2835_gpio_write(PB_PIN, HIGH);
		bcm2835_gpio_write(NA_PIN, HIGH);
		bcm2835_gpio_write(NB_PIN, LOW);
		break;

	case 14:
		bcm2835_gpio_write(PA_PIN, LOW);
		bcm2835_gpio_write(PB_PIN, HIGH);
		bcm2835_gpio_write(NA_PIN, HIGH);
		bcm2835_gpio_write(NB_PIN, HIGH);
		break;

	case 15:
		bcm2835_gpio_write(PA_PIN, LOW);
		bcm2835_gpio_write(PB_PIN, HIGH);
		bcm2835_gpio_write(NA_PIN, LOW);
		bcm2835_gpio_write(NB_PIN, HIGH);
		break;
	}

	return 0;
}

int MeteoLightBoardV1::switchRowSequencely(int row)
{
	switch (row) {
	case 0:
		//bcm2835_gpio_write(PA_PIN, LOW);
		bcm2835_gpio_write(PB_PIN, LOW);
		//bcm2835_gpio_write(NA_PIN, LOW);
		bcm2835_gpio_write(NB_PIN, LOW);
		break;

	case 1:
		//bcm2835_gpio_write(PA_PIN, LOW);
		//bcm2835_gpio_write(PB_PIN, LOW);
		bcm2835_gpio_write(NA_PIN, HIGH);
		//bcm2835_gpio_write(NB_PIN, LOW);
		break;

	case 2:
		//bcm2835_gpio_write(PA_PIN, LOW);
		//bcm2835_gpio_write(PB_PIN, LOW);
		//bcm2835_gpio_write(NA_PIN, HIGH);
		bcm2835_gpio_write(NB_PIN, HIGH);
		break;

	case 3:
		//bcm2835_gpio_write(PA_PIN, LOW);
		//bcm2835_gpio_write(PB_PIN, LOW);
		bcm2835_gpio_write(NA_PIN, LOW);
		//bcm2835_gpio_write(NB_PIN, HIGH);
		break;

	case 4:
		bcm2835_gpio_write(PA_PIN, HIGH);
		//bcm2835_gpio_write(PB_PIN, LOW);
		//bcm2835_gpio_write(NA_PIN, LOW);
		bcm2835_gpio_write(NB_PIN, LOW);
		break;

	case 5:
		//bcm2835_gpio_write(PA_PIN, HIGH);
		//bcm2835_gpio_write(PB_PIN, LOW);
		bcm2835_gpio_write(NA_PIN, HIGH);
		//bcm2835_gpio_write(NB_PIN, LOW);
		break;

	case 6:
		//bcm2835_gpio_write(PA_PIN, HIGH);
		//bcm2835_gpio_write(PB_PIN, LOW);
		//bcm2835_gpio_write(NA_PIN, HIGH);
		bcm2835_gpio_write(NB_PIN, HIGH);
		break;

	case 7:
		//bcm2835_gpio_write(PA_PIN, HIGH);
		//bcm2835_gpio_write(PB_PIN, LOW);
		bcm2835_gpio_write(NA_PIN, LOW);
		//bcm2835_gpio_write(NB_PIN, HIGH);
		break;

	case 8:
		//bcm2835_gpio_write(PA_PIN, HIGH);
		bcm2835_gpio_write(PB_PIN, HIGH);
		//bcm2835_gpio_write(NA_PIN, LOW);
		bcm2835_gpio_write(NB_PIN, LOW);
		break;

	case 9:
		//bcm2835_gpio_write(PA_PIN, HIGH);
		//bcm2835_gpio_write(PB_PIN, HIGH);
		bcm2835_gpio_write(NA_PIN, HIGH);
		//bcm2835_gpio_write(NB_PIN, LOW);
		break;

	case 10:
		//bcm2835_gpio_write(PA_PIN, HIGH);
		//bcm2835_gpio_write(PB_PIN, HIGH);
		//bcm2835_gpio_write(NA_PIN, HIGH);
		bcm2835_gpio_write(NB_PIN, HIGH);
		break;

	case 11:
		//bcm2835_gpio_write(PA_PIN, HIGH);
		//bcm2835_gpio_write(PB_PIN, HIGH);
		bcm2835_gpio_write(NA_PIN, LOW);
		//bcm2835_gpio_write(NB_PIN, HIGH);
		break;

	case 12:
		bcm2835_gpio_write(PA_PIN, LOW);
		//bcm2835_gpio_write(PB_PIN, HIGH);
		//bcm2835_gpio_write(NA_PIN, LOW);
		bcm2835_gpio_write(NB_PIN, LOW);
		break;

	case 13:
		//bcm2835_gpio_write(PA_PIN, LOW);
		//bcm2835_gpio_write(PB_PIN, HIGH);
		bcm2835_gpio_write(NA_PIN, HIGH);
		//bcm2835_gpio_write(NB_PIN, LOW);
		break;

	case 14:
		//bcm2835_gpio_write(PA_PIN, LOW);
		//bcm2835_gpio_write(PB_PIN, HIGH);
		//bcm2835_gpio_write(NA_PIN, HIGH);
		bcm2835_gpio_write(NB_PIN, HIGH);
		break;

	case 15:
		//bcm2835_gpio_write(PA_PIN, LOW);
		//bcm2835_gpio_write(PB_PIN, HIGH);
		bcm2835_gpio_write(NA_PIN, LOW);
		//bcm2835_gpio_write(NB_PIN, HIGH);
		break;
	}
	return 0;
}


int MeteoLightBoardV1::Draw()
{
	system_clock::time_point nowTime;

	LOG(LogLevel::Fine) << "MeteoLightBoardV1::Draw() : Start drawing.";

	struct spi_ioc_transfer tr;
	uint8_t rx[width / 8] = { 0, };

	memset(&tr, 0, sizeof(struct spi_ioc_transfer));
	tr.rx_buf = (unsigned long)rx;

	//system_clock::time_point startTime = system_clock::now();
	//system_clock::time_point temp = system_clock::now();
	//microseconds difference;

	while (1) {
		frameCount++;
		nowTime = system_clock::now();
		if (duration_cast<milliseconds>(nowTime - startTime).count() / 1000 > nowSecond) {
			nowSecond++;
			LOG(LogLevel::Debug) << "MeteoLightBoardV1::Draw() : frame rate = [" << frameCount << "].";
			frameCount = 0;
		}

		LOG(LogLevel::Finest) << "MeteoLightBoardV1::Draw() : Draw a new frame.";

		for (int i = 0; i < height; i++) {

			unique_lock<mutex> uLock(matrixMutex);

			tr.tx_buf = (unsigned long)matrix[i];
			tr.len = width / 8;

			if (ioctl(spi_fd, SPI_IOC_MESSAGE(1), &tr) < 0) {
				LOG(LogLevel::Error) << "MeteoLightBoardV1::Draw() : cannot send spi message.";
			}
			
			uLock.unlock();
			
			switchRowSequencely(i);




			//int count = 0;
			//startTime = system_clock::now();
			//do {
			//	temp = system_clock::now();
			//	count++;
			//} while (duration_cast<microseconds>(temp - startTime).count() < 10);
			//LOG(LogLevel::Finest) << "MeteoLightBoardV1::Draw() : count = " << count;
			usleep(300);
		}
	}


	return 0;
}

int MeteoLightBoardV1::Display(uint8_t ** m)
{

	LOG(LogLevel::Finest) << "MeteoLightBoardV1::Display() : Displaying light matrix...";


	unique_lock<mutex> uLock(matrixMutex);

	for (int i = 0; i < width / 8; i++) {
		for (int j = 0; j < height; j++) {
			matrix[j][i] = 0xFF;
		}
	}
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (LOW_ACTIVE_74595) {
				if (m[i][j] == 0) {
					LOG(LogLevel::Finest) << "MeteoLightBoardV1::Display() : output (" << i << "," << j << ")";
					matrix[height - 1 - j][(width - i - 1) / 8] ^= 1 << (i % 8);
				}
			}
			else {
				if (m[i][j] > 0) {
					LOG(LogLevel::Finest) << "MeteoLightBoardV1::Display() : output (" << i << "," << j << ")";
					matrix[height - 1 - j][(width - i - 1) / 8] ^= 1 << (i % 8);
				}
			}
		}
	}
	
	uLock.unlock();

	return 0;
}