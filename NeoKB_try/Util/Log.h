#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <sstream>
#include <vector>
#include <mutex>


// 參考 https://stackoverflow.com/questions/6168107/how-to-implement-a-good-debug-logging-feature-in-a-project

/* consider adding boost thread id since we'll want to know whose writting and
* won't want to repeat it for every single call */

/* consider adding policy class to allow users to redirect logging to specific
* files via the command line
*/

namespace Util {

	enum class LogLevel
	{
		None,		// 無
		Error, 		// 錯誤
		Warning, 	// 警告
		Info, 		// 一般資訊 (建立的物建)
		Debug, 		// 除錯用資訊
		Fine, 		// 細捷資訊 (執行的function)
		Finer, 		// 更細捷資訊 (function的細節)
		Finest, 	// 最細捷資訊 (運算的細節)
		All,		// 所有資訊
		Depricated
	};

	class LogIt
	{
	public:
		LogIt(LogLevel _loglevel = LogLevel::Error) {
			_buffer << static_cast<int>(_loglevel) << " :"
				<< std::string(
					_loglevel > LogLevel::Debug
					? (static_cast<int>(_loglevel) - static_cast<int>(LogLevel::Debug)) * 4
					: 1
					, ' ');
		}

		template <typename T>
		LogIt & operator<<(T const & value)
		{
			_buffer << value;
			return *this;
		}

		~LogIt()
		{
			_buffer << std::endl;
			// This is atomic according to the POSIX standard
			// http://www.gnu.org/s/libc/manual/html_node/Streams-and-Threads.html
			std::cerr << _buffer.str();

			//while (isBufferToSaveLocked);
			//bufferToSave.push_back(_buffer.str());
		}

		static int Initialize() {
			bufferToSave.clear();
			isBufferToSaveLocked = false;
		}

		static int FlushBuffer() {

			if (bufferToSave.size() == 0)
				return 0;

			vector<string> temp;

			isBufferToSaveLocked = true;
			temp.assign(bufferToSave.begin(), bufferToSave.end());
			bufferToSave.clear();
			isBufferToSaveLocked = false;

			fstream stream;

			//stream.open("./RuntimeLog");

		}

	private:
		std::ostringstream _buffer;
		static std::vector<string> bufferToSave;
		static bool isBufferToSaveLocked;
	};

	extern LogLevel logLevel;

	// TODO: 作log category，如system、kernel、bluetooth、game...
#define LOG(level) \
if (level > logLevel) ; \
else LogIt(level)

}



#endif