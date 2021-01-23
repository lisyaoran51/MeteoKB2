#ifndef FILE_SEGMENT_MAP_H
#define FILE_SEGMENT_MAP_H


#include <map>
#include <fstream>


using namespace std;


namespace Util {
namespace DataStructure{

	/// <summary>
	/// 用一個map來讓檔案片段連接成一個完整的檔案
	/// 寫入檔案的話可以用packet converter來座，這裡面部用座
	/// 輸出成message來給device丟出去的話，可以寫額外的builder或是直接寫code轉成message list，所以也不用寫在這裡面
	/// </summary>
	class FileSegmentMap {
	public:

		FileSegmentMap(int sSize);
		~FileSegmentMap();

		string fileName;

		/// <summary>
		/// map(order, pair(data, size))
		/// </summary>
		map<int, pair<char*, int>> fileSegmentMap;

		int segmentAmount;

		/// <summary>
		/// 輸入你要第幾個segment，就會把資料讀進去file segment李，然後回傳這個segment的大小
		/// </summary>
		int GetFileSegment(int index, char** fileSegment);

		int GetSegmentSize();

		/// <summary>
		/// 只會知道每一個segment之間有沒有空缺，部會之道segment總數對不對
		/// </summary>
		bool CheckFullFilled();

		/// <summary>
		/// 部會close fstream，執行完畢還要自己去close stream
		/// </summary>
		int WriteFile(fstream* fStream);
		
		/// <summary>
		/// 部會close fstream，執行完畢還要自己去close stream
		/// </summary>
		int ReadFile(fstream* fStream);

	protected:

		int segmentSize;

	};

}}







#endif
