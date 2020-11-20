#ifndef FILE_MODE_H
#define FILE_MODE_H

namespace Framework {
namespace IO {

	enum class FileMode {

		//
		// 摘要:
		//     指定作業系統應該建立新的檔案。 這需要 System.Security.Permissions.FileIOPermissionAccess.Write
		//     權限。 如果檔案已經存在， System.IO.IOException 擲回例外狀況。
		CreateNew,
		//
		// 摘要:
		//     指定作業系統應該建立新的檔案。 如果檔案已經存在，則會覆寫。 這需要 System.Security.Permissions.FileIOPermissionAccess.Write
		//     權限。 FileMode.Create 相當於要求的檔案不存在，如果使用 System.IO.FileMode.CreateNew; 否則，請使用 System.IO.FileMode.Truncate。
		//     如果檔案已經存在，但是隱藏的檔案， System.UnauthorizedAccessException 擲回例外狀況。
		Create,
		//
		// 摘要:
		//     指定作業系統應該開啟現有的檔案。 開啟檔案的功能取決於所指定的值 System.IO.FileAccess 列舉型別。 A System.IO.FileNotFoundException
		//     如果檔案不存在，會擲回例外狀況。
		Open,
		//
		// 摘要:
		//     指定作業系統應該開啟檔案，是否有的話，否則，您應該建立新的檔案。 如果與開啟的檔案 FileAccess.Read, ，System.Security.Permissions.FileIOPermissionAccess.Read
		//     需要的權限。 如果檔案存取是 FileAccess.Write, ，System.Security.Permissions.FileIOPermissionAccess.Write
		//     需要的權限。 如果與開啟的檔案 FileAccess.ReadWrite, ，這兩個 System.Security.Permissions.FileIOPermissionAccess.Read
		//     和 System.Security.Permissions.FileIOPermissionAccess.Write 不需要權限。
		OpenOrCreate,
		//
		// 摘要:
		//     指定作業系統應該開啟現有的檔案。 開啟檔案時，應該截斷，因此其大小為零個位元組。 這需要 System.Security.Permissions.FileIOPermissionAccess.Write
		//     權限。 嘗試從檔案讀取以開啟 FileMode.Truncate 造成 System.ArgumentException 例外狀況。
		Truncate,
		//
		// 摘要:
		//     如果存在和尋找檔案的結尾，以便或建立新的檔案，請開啟檔案。 這需要 System.Security.Permissions.FileIOPermissionAccess.Append
		//     權限。 FileMode.Append 只用於搭配 FileAccess.Write。 嘗試搜尋至檔案則會擲回結尾之前的位置 System.IO.IOException
		//     例外狀況，以及任何嘗試讀取會失敗，則擲回 System.NotSupportedException 例外狀況。
		Append

	};


}}



#endif