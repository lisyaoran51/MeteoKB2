#ifndef BLE_REQUEST_EXCEPTION_H
#define BLE_REQUEST_EXCEPTION_H

#include "../../../Framework/IO/Communications/CommunicationRequest.h"


using namespace Framework::IO::Communications;


namespace Games {
namespace IO{
namespace Communications{

	enum class BleResponseCode {
		None = 0,

		//
		// 摘要:
		//     相當於 HTTP 狀態 100。 System.Net.HttpStatusCode.Continue 表示用戶端可以繼續使用其要求。
		Continue = 100,
		//
		// 摘要:
		//     相當於 HTTP 狀態 101。 System.Net.HttpStatusCode.SwitchingProtocols 指出要變更通訊協定的通訊協定版本。
		SwitchingProtocols = 101,
		//
		// 摘要:
		//     相當於 HTTP 狀態 200。 System.Net.HttpStatusCode.OK 表示要求成功，而且所需的資訊會在回應中。 這是要接收的最常見狀態碼。
		OK = 200,
		//
		// 摘要:
		//     相當於 HTTP 狀態 201。 System.Net.HttpStatusCode.Created 代表要求在回應傳送之前建立新的資源。
		Created = 201,
		//
		// 摘要:
		//     相當於 HTTP 狀態 202。 System.Net.HttpStatusCode.Accepted 表示已接受要求進行進一步處理。
		Accepted = 202,
		//
		// 摘要:
		//     相當於 HTTP 狀態 203。 System.Net.HttpStatusCode.NonAuthoritativeInformation 表示傳回的
		//     metainformation 是從快取的複本，而不是原始伺服器，因此可能不正確。
		NonAuthoritativeInformation = 203,
		//
		// 摘要:
		//     相當於 HTTP 狀態 204。 System.Net.HttpStatusCode.NoContent 表示已成功處理要求和回應是特意保留空白。
		NoContent = 204,
		//
		// 摘要:
		//     相當於 HTTP 狀態 205。 System.Net.HttpStatusCode.ResetContent 表示用戶端應該重設 （而不重新載入） 目前的資源。
		ResetContent = 205,
		//
		// 摘要:
		//     相當於 HTTP 狀態 206。 System.Net.HttpStatusCode.PartialContent 表示回應要求的 GET 要求，其中包含的位元組範圍是部分回應。
		PartialContent = 206,
		//
		// 摘要:
		//     相當於 HTTP 狀態 300。 System.Net.HttpStatusCode.MultipleChoices 表示所要求的資訊有多種表示。 預設動作是這個狀態視為重新導向，並遵循與這個回應相關聯的
		//     Location 標頭的內容。
		MultipleChoices = 300,
		//
		// 摘要:
		//     相當於 HTTP 狀態 300。 System.Net.HttpStatusCode.Ambiguous 表示所要求的資訊有多種表示。 預設動作是這個狀態視為重新導向，並遵循與這個回應相關聯的
		//     Location 標頭的內容。
		Ambiguous = 300,
		//
		// 摘要:
		//     相當於 HTTP 狀態 301。 System.Net.HttpStatusCode.MovedPermanently 表示所要求的資訊已被移至的位置標頭中指定的
		//     URI。 收到此狀態時的預設動作是遵循與回應相關聯的 Location 標頭。
		MovedPermanently = 301,
		//
		// 摘要:
		//     相當於 HTTP 狀態 301。 System.Net.HttpStatusCode.Moved 表示所要求的資訊已被移至的位置標頭中指定的 URI。 收到此狀態時的預設動作是遵循與回應相關聯的
		//     Location 標頭。 當原始要求方法是 POST 時，重新導向的要求會使用 GET 方法。
		Moved = 301,
		//
		// 摘要:
		//     相當於 HTTP 狀態 302。 System.Net.HttpStatusCode.Found 表示所要求的資訊是位於指定位置標頭的 URI。 收到此狀態時的預設動作是遵循與回應相關聯的
		//     Location 標頭。 當原始要求方法是 POST 時，重新導向的要求會使用 GET 方法。
		Found = 302,
		//
		// 摘要:
		//     相當於 HTTP 狀態 302。 System.Net.HttpStatusCode.Redirect 表示所要求的資訊是位於指定位置標頭的 URI。 收到此狀態時的預設動作是遵循與回應相關聯的
		//     Location 標頭。 當原始要求方法是 POST 時，重新導向的要求會使用 GET 方法。
		Redirect = 302,
		//
		// 摘要:
		//     相當於 HTTP 狀態 303。 System.Net.HttpStatusCode.SeeOther 自動將用戶端重新導向至指定位置標頭的 POST 結果的
		//     URI。 位置標頭所指定的資源的要求將可使用 GET。
		SeeOther = 303,
		//
		// 摘要:
		//     相當於 HTTP 狀態 303。 System.Net.HttpStatusCode.RedirectMethod 自動將用戶端重新導向至指定位置標頭的
		//     POST 結果的 URI。 位置標頭所指定的資源的要求將可使用 GET。
		RedirectMethod = 303,
		//
		// 摘要:
		//     相當於 HTTP 狀態 304。 System.Net.HttpStatusCode.NotModified 表示用戶端的快取的副本是最新狀態。 不會傳送資源的內容。
		NotModified = 304,
		//
		// 摘要:
		//     相當於 HTTP 狀態 305。 System.Net.HttpStatusCode.UseProxy 表示要求，應該使用 proxy 伺服器的位置標頭中指定的
		//     uri。
		UseProxy = 305,
		//
		// 摘要:
		//     相當於 HTTP 狀態 306。 System.Net.HttpStatusCode.Unused 是建議的延伸模組未完整指定 HTTP/1.1 規格。
		Unused = 306,
		//
		// 摘要:
		//     相當於 HTTP 狀態 307。 System.Net.HttpStatusCode.TemporaryRedirect 表示要求的資訊是位於指定位置標頭的
		//     URI。 收到此狀態時的預設動作是遵循與回應相關聯的 Location 標頭。 當原始要求方法為 POST 時，重新導向的要求也會使用 POST 方法。
		TemporaryRedirect = 307,
		//
		// 摘要:
		//     相當於 HTTP 狀態 307。 System.Net.HttpStatusCode.RedirectKeepVerb 表示要求的資訊是位於指定位置標頭的
		//     URI。 收到此狀態時的預設動作是遵循與回應相關聯的 Location 標頭。 當原始要求方法為 POST 時，重新導向的要求也會使用 POST 方法。
		RedirectKeepVerb = 307,
		//
		// 摘要:
		//     相當於 HTTP 狀態 400。 System.Net.HttpStatusCode.BadRequest 表示要求可能無法辨識伺服器。 System.Net.HttpStatusCode.BadRequest
		//     寄送適用，沒有其他的錯誤時，或如果確切的錯誤是未知或不需要自己的錯誤程式碼。
		BadRequest = 400,
		//
		// 摘要:
		//     相當於 HTTP 狀態 401。 System.Net.HttpStatusCode.Unauthorized 表示要求的資源需要驗證。 WWW 驗證標頭包含如何執行驗證的詳細資料。
		Unauthorized = 401,
		//
		// 摘要:
		//     相當於 HTTP 狀態 402。 System.Net.HttpStatusCode.PaymentRequired 是保留供未來使用。
		PaymentRequired = 402,
		//
		// 摘要:
		//     相當於 HTTP 狀態 403。 System.Net.HttpStatusCode.Forbidden 表示伺服器拒絕要求。
		Forbidden = 403,
		//
		// 摘要:
		//     相當於 HTTP 狀態 404。 System.Net.HttpStatusCode.NotFound 表示要求的資源不存在伺服器上。
		NotFound = 404,
		//
		// 摘要:
		//     與 HTTP 狀態 405 相同。 System.Net.HttpStatusCode.MethodNotAllowed 指示要求的資源不允許要求方法 （POST
		//     或 GET）。
		MethodNotAllowed = 405,
		//
		// 摘要:
		//     相當於 HTTP 狀態 406。 System.Net.HttpStatusCode.NotAcceptable 表示用戶端已使用 Accept 標頭，也不會接受任何可用的表示法的資源。
		NotAcceptable = 406,
		//
		// 摘要:
		//     相當於 HTTP 狀態 407。 System.Net.HttpStatusCode.ProxyAuthenticationRequired 表示要求的
		//     proxy 需要驗證。 Proxy 驗證標頭包含如何執行驗證的詳細資料。
		ProxyAuthenticationRequired = 407,
		//
		// 摘要:
		//     與 HTTP 狀態 408 相同。 System.Net.HttpStatusCode.RequestTimeout 表示用戶端未在此伺服器已收到要求的時間內傳送要求。
		RequestTimeout = 408,
		//
		// 摘要:
		//     相當於 HTTP 狀態 409。 System.Net.HttpStatusCode.Conflict 表示要求可能不會執行因為伺服器上的衝突。
		Conflict = 409,
		//
		// 摘要:
		//     與 HTTP 狀態 410 相同。 System.Net.HttpStatusCode.Gone 表示要求的資源已無法再使用。
		// 更新: 代表呼叫該request的物件已經消失，因此該request也要消失
		Gone = 410,
		//
		// 摘要:
		//     相當於 HTTP 狀態 411。 System.Net.HttpStatusCode.LengthRequired 表示遺漏必要的內容長度標頭。
		LengthRequired = 411,
		//
		// 摘要:
		//     相當於 HTTP 狀態 412。 System.Net.HttpStatusCode.PreconditionFailed 表示條件設定失敗，此要求，而且無法執行要求。
		//     如果比對，例如條件式標頭設定條件 If-none-Match，或如果未修改自。
		PreconditionFailed = 412,
		//
		// 摘要:
		//     相當於 HTTP 狀態 413。 System.Net.HttpStatusCode.RequestEntityTooLarge 表示要求太大，伺服器處理。
		RequestEntityTooLarge = 413,
		//
		// 摘要:
		//     相當於 HTTP 狀態 414。 System.Net.HttpStatusCode.RequestUriTooLong 表示 URI 太長。
		RequestUriTooLong = 414,
		//
		// 摘要:
		//     相當於 HTTP 狀態 415。 System.Net.HttpStatusCode.UnsupportedMediaType 表示要求不支援的類型。
		UnsupportedMediaType = 415,
		//
		// 摘要:
		//     相當於 HTTP 狀態 416。 System.Net.HttpStatusCode.RequestedRangeNotSatisfiable 表示資料自資源要求的範圍無法傳回，或是因為範圍的開頭之前的資源，開頭或結尾的範圍會在後的資源。
		RequestedRangeNotSatisfiable = 416,
		//
		// 摘要:
		//     相當於 HTTP 狀態 417。 System.Net.HttpStatusCode.ExpectationFailed 表示指定 Expect 標頭中可能不符合伺服器。
		ExpectationFailed = 417,
		//
		// 摘要:
		//     相當於 HTTP 狀態 426。 System.Net.HttpStatusCode.UpgradeRequired 表示用戶端應該切換到不同的通訊協定，例如
		//     TLS/1.0。
		UpgradeRequired = 426,
		//
		// 摘要:
		//     相當於 HTTP 狀態 500。 System.Net.HttpStatusCode.InternalServerError 表示在伺服器上已發生一般錯誤。
		InternalServerError = 500,
		//
		// 摘要:
		//     相當於 HTTP 狀態 501。 System.Net.HttpStatusCode.NotImplemented 表示伺服器不支援要求的函式。
		NotImplemented = 501,
		//
		// 摘要:
		//     與 HTTP 狀態 502 相同。 System.Net.HttpStatusCode.BadGateway 表示中繼 proxy 伺服器從另一個 proxy
		//     或原始伺服器收到不正確的回應。
		BadGateway = 502,
		//
		// 摘要:
		//     相當於 HTTP 狀態 503。 System.Net.HttpStatusCode.ServiceUnavailable 表示伺服器暫時無法使用，通常是因為高負載或維護。
		ServiceUnavailable = 503,
		//
		// 摘要:
		//     相當於 HTTP 狀態 504。 System.Net.HttpStatusCode.GatewayTimeout 表示中繼 proxy 伺服器逾時等待來自另一個
		//     proxy 或原始伺服器的回應。
		GatewayTimeout = 504,
		//
		// 摘要:
		//     與 HTTP 狀態 505 相同。 System.Net.HttpStatusCode.HttpVersionNotSupported 表示伺服器不支援要求的
		//     HTTP 版本。
		HttpVersionNotSupported = 505,

		/// <summary>
		/// 字定義的，與手機中斷連線
		/// </summary>
		DisconnectedFromBlePhone,
		/// <summary>
		/// 字定義的，mtu太小
		/// </summary>
		MtuTooSmall,
		/// <summary>
		/// 字定義的，被要求終止
		/// </summary>
		ExitRequested,
		/// <summary>
		/// 字定義的，沒有把檔案轉換出file segment
		/// </summary>
		TransferToFileSegmentsFailed,



	};

	class BleRequestException : public CommunicationRequestException {

	public:

		BleRequestException(BleResponseCode bResponseCode);

		BleResponseCode GetBleResponseCode();

	protected:

		BleResponseCode bleResponseCode = BleResponseCode::None;

	};

}}}

#endif