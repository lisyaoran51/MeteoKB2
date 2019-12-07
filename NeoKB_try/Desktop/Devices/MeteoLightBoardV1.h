#ifndef METEO_LIGHT_BOARD_V1_H
#define METEO_LIGHT_BOARD_V1_H

#include <stdint.h>
#include "bcm2835.h"
#include <unistd.h>
#include <cstdlib>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <mutex>
#include <chrono>


#define PA_PIN RPI_BPLUS_GPIO_J8_11
#define PB_PIN RPI_BPLUS_GPIO_J8_13
#define NA_PIN RPI_BPLUS_GPIO_J8_15
#define NB_PIN RPI_BPLUS_GPIO_J8_16

#define LOW_ACTIVE_74595 1
#define OE_74595 RPI_BPLUS_GPIO_J8_22



using namespace std;
using namespace std::chrono;


namespace Desktop {
namespace Devices {

	class MeteoLightBoardV1 {

		int width, height;

		uint8_t** matrix;

		int spi_fd;

		mutable mutex matrixMutex;

		bool threadLock;

		int switchRow(int row);

		int switchRowSequencely(int row);

		system_clock::time_point startTime;

		int frameCount = 0;

		int nowSecond = 0;

	public:

		MeteoLightBoardV1(int w, int h);

		int Draw();

		int Display(uint8_t** m);


	};


}}



#endif