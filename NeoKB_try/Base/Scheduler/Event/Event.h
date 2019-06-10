#ifndef EVENT_H
#define EVENT_H

#include <string>
#include <fstream>
#include <vector>
#include "../../../Util/TemplateConstraint.h"
#include "../../../Util/MtoType.h"



using namespace std;
using namespace Util;

namespace Base {
namespace Schedulers {
namespace Events {
	
	enum class EventStartType;
	enum class EventLifeType;


	class Event
	{

	public:

		Event();
		Event(MTO_FLOAT s, MTO_FLOAT l);
		Event(const Event&);

		EventStartType GetStartType();
		EventLifeType GetLifeType();

		int SetStartTime(MTO_FLOAT s);
		int SetLifeTime(MTO_FLOAT l);
		MTO_FLOAT GetStartTime() const;
		MTO_FLOAT GetLifeTime() const;

		bool operator<(const Event &rhs) const;

		// 一定要每次都override!!
		virtual string GetTypeName();

		template<typename T>
		bool CanCast()
		{
			if (T* t = dynamic_cast< T* >(this)) {
				return true;
			}
			else {
				return false;
			}
		}

		template<typename T>
		T* Cast() 
		{
			return dynamic_cast< T* >(this);
		}

		virtual Event* Clone();

	protected:

		int eid;

		EventStartType startType;
		EventLifeType lifeType;

		/// <summary>
		/// 開始時間，如果式即時事件就是-1
		/// </summary>
		MTO_FLOAT startTime;

		/// <summary>
		/// 結束時間 如果是瞬間事件，就是0，如果是由訊號通知開關，就是-1
		/// </summary>
		MTO_FLOAT lifeTime;

		Event(const Event*);
	};

	enum class EventStartType {
		Reserved,
		Immediate,
	};

	enum class EventLifeType {
		Timed,
		Immediate,
		Infinite
	};
	

}}}



#endif