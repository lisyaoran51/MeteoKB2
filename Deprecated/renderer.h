#ifndef RENDERER_H
#define RENDERER_H

#include <map>
#include <string>
#include "../Map/Map.h"
//#include "MapAlgorithm\HardwareMapAlgorithm.h"
#include "../../../Util/Hierachal/ChildAddable.h"
#include "../../Config/FrameworkConfigManager.h"
#include "../../../Devices/HardwareInfo.h"
#include "../../../Util/Update/Updater.h"
#include "../../../Util/Update/Updatable.h"



using namespace std;
using namespace Base::Graphic::Maps;
//using namespace Base::Graphic::Renderers::MapAlgorithms;
using namespace Util::Hierachal;
using namespace Base::Config;
using namespace Devices;
using namespace Util::Update;


namespace Base {
namespace Graphic {
namespace Renderers {

	class Renderer : public Updatable,  public ChildAddable{

		int load();

		int load(Updater* u, FrameworkConfigManager* f);

		/// <summary>
		/// key:�w�骩�� value:renderer�W��
		///	</summary>
		static map<int, string> renderers;

		static bool initialized;

		/// <summary>
		/// �غc�l�|�۰ʰ���o�Ө�O�A�}�|����@���A��Ҧ���rendererŪ�i�o��renderer��
		///	</summary>
		static bool initialize();


	public:

		Renderer();

		int SetHardwareVersion(int hwVersion);

		int RegisterMap(Map* m);

		static Renderer* GetRenderer(int hwVersion);

		virtual int Elapse(MTO_FLOAT elapsedTime);

		virtual int Render() = 0;

		virtual int SendToDriver() = 0;

		virtual int CleanMap();

	protected:

		/// <summary>
		/// �w�骩���A����[�Jmap algo�Mled driver���ӳ��O�ھڳo��
		///	</summary>
		int hardwareVersion;

		Map* lightMap;

		//�����γo�ӪF��A�Pı�S�������
		//HardwareMapAlgo* hwMapAlgo;

		/// <summary>
		/// �ռơB�w���B�ثe�Ҹg�L�ɶ�
		///	</summary>
		MTO_FLOAT frameLength, currentFrameLength;
		int frameRate;

		/// <summary>
		/// HWMapAlgo���ӬO�Mrenderer�O���j�w ������
		///	</summary>
		//int RegisterHWMapAlgo(HardwareMapAlgo* hwma);

		/// <summary>
		/// ���Ӥ]�M�����j�w? ������
		///	</summary>
		//int RegisterLedDriver(LedDriver* ld);
	};

	


}}}


#endif