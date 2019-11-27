#ifndef LOADABLE_H
#define LOADABLE_H

#include "Cachable.h"
#include "../../../Util/MtoType.h"
#include<vector>
#include<string>
#include<functional>
#include "HasParent.h"
#include <mutex>

using namespace std;

/*
 * pointer of every variable
 * https://stackoverflow.com/questions/19710601/any-type-of-pointer-can-point-to-anything
 *
 * ���UprivateLoad����k
 * �bconstructor�̭���@��registerPrivateLoader("���O�W��", privateLoad);
 */

/*
 * functional��punction pointer��k
 * https://kheresy.wordpress.com/2010/11/12/function_object_tr1/
 */


/*
 * when child class call parent class's protected function to access private variable, it only
 * access his own private variable, but not access child class's private variable
 *
 * c++ can't automatically register private function. so we do it manually by put 
 * "registerPrivateLoader()" into every constructor of child classes. So that
 * we could find a vector of loaders of each class inherited from Loadable
 * and then call them
 *
 */


namespace Framework {
namespace Allocation {
namespace Hierachal{

	enum class LoadState;

	class Loadable: public Cachable {

		/// <summary>
		/// for register_load() to ergister every loaders into this vector
		/// </summary>
		vector<MTO_FUNC_POINTER> loaders;

		/// <summary>
		/// �u���i��load���a��
		/// </summary>
		int load();

		mutable mutex stateHandlerMutex;

	public:

		Loadable();

		virtual ~Loadable() = default;

		/// <summary>
		/// call every parent class's private load() function
		/// 1. take out all loaders and all parameters
		/// 2. call every loaders with those parameter
		/// �o�Ӧ���public�����رM�ΡA�q�`�@���C�����u���@�ӡA�|��parent�]���n�[�J�����
		/// </summary>
		int Async();

		/// <summary>
		/// ��component��cache�]�n�A�M��async
		/// </summary>
		virtual int LoadAsync(DependencyContainer* dContainer);

		LoadState GetLoadState();

		/// <summary>
		/// override HasParent���禡�A�b�̭��[�Whandler
		/// </summary>
		virtual int SetParent(HasParent* p);


	protected:

		/// <summary>
		/// register loader, must do in every loadable derived class 
		/// </summary>
		/// <param name="l">private loader</param>
		int registerLoad(function<int(void)> f);

		/// <summary>
		/// ��Ҧ���load��������@��
		/// �o�Ӧ���childaddable�Ϊ�
		/// </summary>
		int Load();

		/// <summary>
		/// �bload�������ɭԡA�������A�OReady�A�|�b�Ĥ@��update�ɰ���A�M��⪬�A�ഫ��loaded
		/// </summary>
		virtual int LoadComplete() final;

		/// <summary>
		/// �bload�������ɭԡA�ɰO�n���檺�u�@
		/// </summary>
		virtual int LoadOnComplete();

	private:

		/// <summary>
		/// ��ڰ���setParent�A��LoadStateHandler�M�w�O�_�n����o�Ӱʧ@
		/// </summary>
		int setParent(HasParent* p);


		class LoadStateHandler {
		public:
			LoadStateHandler(Loadable& l);
			virtual LoadState GetLoadState() = 0;
			virtual int HandleLoad() = 0;
			virtual int Async() = 0;
			virtual int SetParent(HasParent* p) = 0;
			virtual int HandleLoadComplete() = 0;
		protected:
			Loadable& loadable;
		};

		class NoParentHandler : public LoadStateHandler {
		public:
			NoParentHandler(Loadable& l);
			virtual LoadState GetLoadState();
			virtual int HandleLoad();
			virtual int Async();
			virtual int SetParent(HasParent* p);
			virtual int HandleLoadComplete();
		};

		class NotLoadedHandler : public LoadStateHandler {
		public:
			NotLoadedHandler(Loadable& l);
			virtual LoadState GetLoadState();
			virtual int HandleLoad();
			virtual int Async();
			virtual int SetParent(HasParent* p);
			virtual int HandleLoadComplete();
		};

		class LoadingHandler : public LoadStateHandler {
		public:
			LoadingHandler(Loadable& l);
			virtual LoadState GetLoadState();
			virtual int HandleLoad();
			virtual int Async();
			virtual int SetParent(HasParent* p);
			virtual int HandleLoadComplete();
		};

		class ReadyHandler : public LoadStateHandler {
		public:
			ReadyHandler(Loadable& l);
			virtual LoadState GetLoadState();
			virtual int HandleLoad();
			virtual int Async();
			virtual int SetParent(HasParent* p);
			virtual int HandleLoadComplete();
		};

		class LoadedHandler : public LoadStateHandler {
		public:
			LoadedHandler(Loadable& l);
			virtual LoadState GetLoadState();
			virtual int HandleLoad();
			virtual int Async();
			virtual int SetParent(HasParent* p);
			virtual int HandleLoadComplete();
		};

		LoadStateHandler* loadStateHandler;
		NoParentHandler noParentHandler;
		NotLoadedHandler notLoadedHandler;
		LoadingHandler loadingHandler;
		ReadyHandler readyHandler;
		LoadedHandler loadedHandler;

	};


	/// <summary>
	/// Possible states of a <see cref="Drawable"/> within the loading pipeline.
	/// </summary>
	enum class LoadState {
		/// <summary>
		/// Not loaded, and no load has been initiated yet.
		/// </summary>
		NoParent,
		/// <summary>
		/// Not loaded, and no load has been initiated yet.
		/// </summary>
		NotLoaded,
		/// <summary>
		/// Currently loading (possibly and usually on a background
		/// thread via <see cref="Drawable.LoadAsync(Game, Drawable, ActionList)"/>).
		/// </summary>
		Loading,
		/// <summary>
		/// Loading is complete, but has not yet been finalized on the update thread
		/// (<see cref="Drawable.LoadComplete"/> has not been called yet, which
		/// always runs on the update thread and requires <see cref="Drawable.IsAlive"/>).
		/// </summary>
		Ready,
		/// <summary>
		/// Loading is fully completed and the Drawable is now part of the scene graph.
		/// </summary>
		Loaded
	};



}}}




#endif