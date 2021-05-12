#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <sstream>
#include <vector>
#include <mutex>


// �Ѧ� https://stackoverflow.com/questions/6168107/how-to-implement-a-good-debug-logging-feature-in-a-project

/* consider adding boost thread id since we'll want to know whose writting and
* won't want to repeat it for every single call */

/* consider adding policy class to allow users to redirect logging to specific
* files via the command line
*/

namespace Util {

	enum class LogLevel
	{
		None,		// �L
		Error, 		// ���~
		Warning, 	// ĵ�i
		Info, 		// �@���T (�إߪ�����)
		Debug, 		// �����θ�T
		Fine, 		// �ӱ���T (���檺function)
		Finer, 		// ��ӱ���T (function���Ӹ`)
		Finest, 	// �̲ӱ���T (�B�⪺�Ӹ`)
		All,		// �Ҧ���T
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

			std::vector<std::string> temp;

			isBufferToSaveLocked = true;
			temp.assign(bufferToSave.begin(), bufferToSave.end());
			bufferToSave.clear();
			isBufferToSaveLocked = false;

			std::fstream stream;

			//stream.open("./RuntimeLog");

		}

	private:
		std::ostringstream _buffer;
		static std::vector<std::string> bufferToSave;
		static bool isBufferToSaveLocked;
	};

	extern LogLevel logLevel;

	// TODO: �@log category�A�psystem�Bkernel�Bbluetooth�Bgame...
#define LOG(level) \
if (level > logLevel) ; \
else LogIt(level)

}



#endif