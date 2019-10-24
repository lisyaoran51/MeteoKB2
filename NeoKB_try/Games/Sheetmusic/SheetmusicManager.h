#ifndef SHEETMUSIC_MANAGER_H
#define SHEETMUSIC_MANAGER_H

#include <string>
#include <fstream>
#include <vector>
#include "WorkingSheetmusic.h"
#include "../../Util/TemplateConstraint.h"
#include "SheetmusicInfo.h"
#include "Sheetmusic.h"
#include "IO/FileReader.h"
#include "Format/SmDecoder.h"
#include "../Ruleset/Ruleset.h"
#include "../Ruleset/RulesetInfo.h"



using namespace std;
using namespace Util;
using namespace Games::Sheetmusics::IO;
using namespace Games::Sheetmusics::Format;
using namespace Games::Rulesets;



namespace Games {
namespace Sheetmusics {

	/// where T : effect_group
	/// 
	class SmManager
	{
		// ??���T�w�O�_�ݭn
		//static int copyTo(ifstream* from, ifstream* to);

	public:

		SmManager();

		~SmManager();

		int RegisterRulesetInfo(RulesetInfo* r);

		int Import(vector<string>* paths);

		/// <summary>
		/// �q�t�ιw�]�����|�h���ɮ�
		/// </summary>
		int ImportFromStable();

		vector<SmInfo*>* GetSmInfos();

		WorkingSm* GetWorkingSm(SmInfo* s);

	protected:

		vector<RulesetInfo*>* rulesetInfos;

		vector<SmInfo*>* smInfos;

		

		/// <summary>
		/// �|��o�Ǹ��v�̭����ɮץ���Ū�X�ӡA�åB��sm set�����@�դ@�ռ��СA�M����s��Y�Ӧa��(��Ʈw�άO?)
		/// ���O�ثe�S������o�ӳ����A�u����и�ƥ���i�h�@��list�̡A�ԫ�osu���@�k
		/// </summary>
		vector<SmInfo*>* import(FileReader& fileReader);

		/// <summary>
		/// �o�Ӫ��\����ܧ�sm�̭������ɸ��Ū�X�ӡA�̭���event�٤��ݭn�A�̭���event�n����C���}�l�ɦAŪ
		/// </summary>
		//vector<SmInfo*>* importToStorage(FileReader& fileReader); //�ثe�S���b�s�^storage�ҥH����

		// WorkingSm(sm_info_t* s);

		//virtual Sm<Event>* createSm() = 0;

	};


}}



#endif