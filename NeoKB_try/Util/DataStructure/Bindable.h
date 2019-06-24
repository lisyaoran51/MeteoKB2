#ifndef BINDABLE_H
#define BINDABLE_H


#include <vector>


using namespace std;


namespace Util {
namespace DataStructure {

	template<typename T>
	class Bindable {

	public:

		int SetValue(T v);
		
		T GetValue();

		int SetDisabled(bool d);

		bool GetDisabled();

		/// <summary>
		/// Binds outselves to another bindable such that they receive bi-directional updates.
		/// We will take on any value limitations of the bindable we bind width.
		/// </summary>
		/// <param name="other">The foreign bindable. This should always be the most permanent end of the bind (ie. a ConfigManager)</param>
		int BindTo(Bindable<T>* other);

		// 這個式一方加另一方，bindto式雙方互加
		int AddBindings(Bindable<T>* other);

	protected:

		/// <summary>
		/// 更新所有連接的bindings的值
		/// </summary>
		int TriggerValueChange();

		int TriggerDisabledChange();

	private:

		T value;
		T defaultValue;
		bool disabled;

		// Action<int(int)> ValueChanged;

		vector<Bindable<T>*> bindings;
		
	};



}}


#endif