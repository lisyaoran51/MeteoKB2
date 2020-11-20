#ifndef FILE_ACCESS_H
#define FILE_ACCESS_H

namespace Framework {
namespace IO {

	enum class FileAccess {

		//
		// 摘要:
		//     檔案的讀取權限。 可以從檔案讀取資料。 結合 Write 的讀取/寫入存取。
		Read,
		//
		// 摘要:
		//     檔案的寫入存取權。 資料可以寫入至檔案。 結合 Read 的讀取/寫入存取。
		Write,
		//
		// 摘要:
		//     讀取和寫入檔案的存取權。 資料可以寫入和讀取檔案。
		ReadWrite

	};


}}



#endif