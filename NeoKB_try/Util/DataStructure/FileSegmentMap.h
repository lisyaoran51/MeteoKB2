#ifndef FILE_SEGMENT_MAP_H
#define FILE_SEGMENT_MAP_H


#include <map>
#include <fstream>


using namespace std;


namespace Util {
namespace DataStructure{

	/// <summary>
	/// �Τ@��map�����ɮפ��q�s�����@�ӧ��㪺�ɮ�
	/// �g�J�ɮת��ܥi�H��packet converter�Ӯy�A�o�̭����ήy
	/// ��X��message�ӵ�device��X�h���ܡA�i�H�g�B�~��builder�άO�����gcode�নmessage list�A�ҥH�]���μg�b�o�̭�
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
		/// ��J�A�n�ĴX��segment�A�N�|����Ū�i�hfile segment���A�M��^�ǳo��segment���j�p
		/// </summary>
		int GetFileSegment(int index, char** fileSegment);

		int GetSegmentSize();

		/// <summary>
		/// �u�|���D�C�@��segment�������S���ůʡA���|���Dsegment�`�ƹ藍��
		/// </summary>
		bool CheckFullFilled();

		/// <summary>
		/// ���|close fstream�A���槹���٭n�ۤv�hclose stream
		/// </summary>
		int WriteFile(fstream* fStream);
		
		/// <summary>
		/// ���|close fstream�A���槹���٭n�ۤv�hclose stream
		/// </summary>
		int ReadFile(fstream* fStream);

	protected:

		int segmentSize;

	};

}}







#endif
