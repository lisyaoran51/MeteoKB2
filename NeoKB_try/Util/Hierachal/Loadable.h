#ifndef LOADABLE_H
#define LOADABLE_H

#include "Cachable.h"
#include "../MtoType.h"
#include<vector>
#include<string>
#include<functional>
#include "HasParent.h"

using namespace std;

/*
 * pointer of every variable
 * https://stackoverflow.com/questions/19710601/any-type-of-pointer-can-point-to-anything
 *
 * 註冊privateLoad的方法
 * 在constructor裡面放一個registerPrivateLoader("類別名稱", privateLoad);
 */

/*
 * functional的punction pointer方法
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



namespace Util {
namespace Hierachal{

	enum class LoadState;

	class Loadable: public Cachable {

		/// <summary>
		/// for register_load() to ergister every loaders into this vector
		/// </summary>
		vector<MTO_FUNC_POINTER> loaders;

		/// <summary>
		/// 真正進行load的地方
		/// </summary>
		int load();

	public:

		Loadable();

		virtual ~Loadable() = default;

		/// <summary>
		/// call every parent class's private load() function
		/// 1. take out all loaders and all parameters
		/// 2. call every loaders with those parameter
		/// 這個式給public的物建專用，通常一場遊戲中只有一個，會把parent設成要加入的資料
		/// </summary>
		int Async();

		LoadState GetLoadState();

		/// <summary>
		/// override HasParent的函式，在裡面加上handler
		/// </summary>
		int SetParent(HasParent* p);

	protected:

		/// <summary>
		/// register loader, must do in every loadable derived class 
		/// </summary>
		/// <param name="l">private loader</param>
		int registerLoad(function<int(void)> f);

		/// <summary>
		/// 把所有的load全不執行一次
		/// 這個式給childaddable用的
		/// </summary>
		int Load();

	private:


		class LoadStateHandler {
		public:
			LoadStateHandler(Loadable& l);
			virtual LoadState GetLoadState() = 0;
			virtual int HandleLoad() = 0;
			virtual int Async() = 0;
			virtual int SetParent(HasParent* p) = 0;
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
		};

		class NotLoadedHandler : public LoadStateHandler {
		public:
			NotLoadedHandler(Loadable& l);
			virtual LoadState GetLoadState();
			virtual int HandleLoad();
			virtual int Async();
			virtual int SetParent(HasParent* p);
		};

		class LoadingHandler : public LoadStateHandler {
		public:
			LoadingHandler(Loadable& l);
			virtual LoadState GetLoadState();
			virtual int HandleLoad();
			virtual int Async();
			virtual int SetParent(HasParent* p);
		};

		class ReadyHandler : public LoadStateHandler {
		public:
			ReadyHandler(Loadable& l);
			virtual LoadState GetLoadState();
			virtual int HandleLoad();
			virtual int Async();
			virtual int SetParent(HasParent* p);
		};

		LoadStateHandler* loadStateHandler;
		NoParentHandler noParentHandler;
		NotLoadedHandler notLoadedHandler;
		LoadingHandler loadingHandler;
		ReadyHandler readyHandler;

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
		/// thread via <see cref="Drawable.LoadAsync(Game, Drawable, Action)"/>).
		/// </summary>
		Loading,
		/// <summary>
		/// Loading is complete, but has not yet been finalized on the update thread
		/// (<see cref="Drawable.LoadComplete"/> has not been called yet, which
		/// always runs on the update thread and requires <see cref="Drawable.IsAlive"/>).
		/// </summary>
		Ready
		/// <summary>
		/// Loading is fully completed and the Drawable is now part of the scene graph.
		/// </summary>
		// LoadStateLoaded
	};



}}




#endif