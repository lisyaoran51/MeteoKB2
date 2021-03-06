#ifndef PLAYFIELD_EVENT_PROCESSOR_H
#define PLAYFIELD_EVENT_PROCESSOR_H


#include "../../../Scheduler/Event/EventProcessor.h"
#include "PlayfieldControllers/PlayfieldController.h"


using namespace Games::Schedulers::Events;
using namespace Games::Schedulers::Events::PlayfieldEvents::PlayfieldControllers;


namespace Games {
namespace Schedulers {
namespace Events {
namespace PlayfieldEvents {

	enum class PlayfieldControlType {
		Once,
		Timed,
		Continuous
	};

	class PlayfieldEventProcessorInterface : virtual public EventProcessor<Event> {

	public:

		virtual int RegisterPlayfieldController(PlayfieldControllerInterface* pControllerInterface) = 0;

		virtual int Process();
			
		virtual int ControlPlayfield() = 0;

		virtual int UndoControlPlayfield() = 0;

		virtual bool GetIsControllable() = 0;

		virtual int SetIsControlled(bool value) = 0;

	};

	template<typename T>
	class PlayfieldEventProcessor : public PlayfieldEventProcessorInterface {

	public:

		virtual int RegisterPlayfieldController(PlayfieldControllerInterface* pControllerInterface) {
			if (MtoObject::CanCast<PlayfieldController<T>>(pControllerInterface)) {
				playfieldController = MtoObject::Cast<PlayfieldController<T>>(pControllerInterface);
				return 0;
			}
			// TODO: �Q���~
			return -1;
		}

		virtual int ControlPlayfield() {
			if(!playfieldController)
				LOG(LogLevel::Error) << "PlayfieldEventProcessor::ControlPlayfield : no playfield controller.";
				
			if (playfieldController && GetIsControllable()) {

				SetIsControlled(true);
				playfieldController->ControlPlayfield(this);

			}
			return 0;
		}

		virtual int UndoControlPlayfield() {
			if (!playfieldController)
				LOG(LogLevel::Error) << "PlayfieldEventProcessor::UndoControlPlayfield : no playfield controller.";

			if (playfieldController) {

				SetIsControlled(false);
				playfieldController->UndoControlPlayfield(this);

			}
			return 0;
		}

		T* GetPlayfieldEvent() { return dynamic_cast<T*>(event); }

		virtual int SetIsControlled(bool value) {
			if (value == true) {
				if (playfieldControlType == PlayfieldControlType::Once)
					isControllable = false;
				else if (playfieldControlType == PlayfieldControlType::Timed) {
					controlTime--;
					if (controlTime == 0)
						isControllable = false;
				}
			}
			else {
				if (playfieldControlType == PlayfieldControlType::Once)
					isControllable = true;
				else if (playfieldControlType == PlayfieldControlType::Timed) {
					if (controlTime == 0)
						isControllable = true;
					controlTime++;
					
				}
			}
		}

		virtual bool GetIsControllable() {
			return isControllable;
		}

	protected:

		bool isControllable = true;

		int controlTime = 1;

		PlayfieldControlType playfieldControlType = PlayfieldControlType::Once;

		PlayfieldController<T>* playfieldController = nullptr;

	};


}}}}




#endif