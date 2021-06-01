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
		// �K�n:
		//     �۷�� HTTP ���A 100�C System.Net.HttpStatusCode.Continue ��ܥΤ�ݥi�H�~��ϥΨ�n�D�C
		Continue = 100,
		//
		// �K�n:
		//     �۷�� HTTP ���A 101�C System.Net.HttpStatusCode.SwitchingProtocols ���X�n�ܧ�q�T��w���q�T��w�����C
		SwitchingProtocols = 101,
		//
		// �K�n:
		//     �۷�� HTTP ���A 200�C System.Net.HttpStatusCode.OK ��ܭn�D���\�A�ӥB�һݪ���T�|�b�^�����C �o�O�n�������̱`�����A�X�C
		OK = 200,
		//
		// �K�n:
		//     �۷�� HTTP ���A 201�C System.Net.HttpStatusCode.Created �N��n�D�b�^���ǰe���e�إ߷s���귽�C
		Created = 201,
		//
		// �K�n:
		//     �۷�� HTTP ���A 202�C System.Net.HttpStatusCode.Accepted ��ܤw�����n�D�i��i�@�B�B�z�C
		Accepted = 202,
		//
		// �K�n:
		//     �۷�� HTTP ���A 203�C System.Net.HttpStatusCode.NonAuthoritativeInformation ��ܶǦ^��
		//     metainformation �O�q�֨����ƥ��A�Ӥ��O��l���A���A�]���i�ण���T�C
		NonAuthoritativeInformation = 203,
		//
		// �K�n:
		//     �۷�� HTTP ���A 204�C System.Net.HttpStatusCode.NoContent ��ܤw���\�B�z�n�D�M�^���O�S�N�O�d�ťաC
		NoContent = 204,
		//
		// �K�n:
		//     �۷�� HTTP ���A 205�C System.Net.HttpStatusCode.ResetContent ��ܥΤ�����ӭ��] �]�Ӥ����s���J�^ �ثe���귽�C
		ResetContent = 205,
		//
		// �K�n:
		//     �۷�� HTTP ���A 206�C System.Net.HttpStatusCode.PartialContent ��ܦ^���n�D�� GET �n�D�A�䤤�]�t���줸�սd��O�����^���C
		PartialContent = 206,
		//
		// �K�n:
		//     �۷�� HTTP ���A 300�C System.Net.HttpStatusCode.MultipleChoices ��ܩҭn�D����T���h�ت�ܡC �w�]�ʧ@�O�o�Ӫ��A�������s�ɦV�A�ÿ�`�P�o�Ӧ^�������p��
		//     Location ���Y�����e�C
		MultipleChoices = 300,
		//
		// �K�n:
		//     �۷�� HTTP ���A 300�C System.Net.HttpStatusCode.Ambiguous ��ܩҭn�D����T���h�ت�ܡC �w�]�ʧ@�O�o�Ӫ��A�������s�ɦV�A�ÿ�`�P�o�Ӧ^�������p��
		//     Location ���Y�����e�C
		Ambiguous = 300,
		//
		// �K�n:
		//     �۷�� HTTP ���A 301�C System.Net.HttpStatusCode.MovedPermanently ��ܩҭn�D����T�w�Q���ܪ���m���Y�����w��
		//     URI�C ���즹���A�ɪ��w�]�ʧ@�O��`�P�^�������p�� Location ���Y�C
		MovedPermanently = 301,
		//
		// �K�n:
		//     �۷�� HTTP ���A 301�C System.Net.HttpStatusCode.Moved ��ܩҭn�D����T�w�Q���ܪ���m���Y�����w�� URI�C ���즹���A�ɪ��w�]�ʧ@�O��`�P�^�������p��
		//     Location ���Y�C ���l�n�D��k�O POST �ɡA���s�ɦV���n�D�|�ϥ� GET ��k�C
		Moved = 301,
		//
		// �K�n:
		//     �۷�� HTTP ���A 302�C System.Net.HttpStatusCode.Found ��ܩҭn�D����T�O�����w��m���Y�� URI�C ���즹���A�ɪ��w�]�ʧ@�O��`�P�^�������p��
		//     Location ���Y�C ���l�n�D��k�O POST �ɡA���s�ɦV���n�D�|�ϥ� GET ��k�C
		Found = 302,
		//
		// �K�n:
		//     �۷�� HTTP ���A 302�C System.Net.HttpStatusCode.Redirect ��ܩҭn�D����T�O�����w��m���Y�� URI�C ���즹���A�ɪ��w�]�ʧ@�O��`�P�^�������p��
		//     Location ���Y�C ���l�n�D��k�O POST �ɡA���s�ɦV���n�D�|�ϥ� GET ��k�C
		Redirect = 302,
		//
		// �K�n:
		//     �۷�� HTTP ���A 303�C System.Net.HttpStatusCode.SeeOther �۰ʱN�Τ�ݭ��s�ɦV�ܫ��w��m���Y�� POST ���G��
		//     URI�C ��m���Y�ҫ��w���귽���n�D�N�i�ϥ� GET�C
		SeeOther = 303,
		//
		// �K�n:
		//     �۷�� HTTP ���A 303�C System.Net.HttpStatusCode.RedirectMethod �۰ʱN�Τ�ݭ��s�ɦV�ܫ��w��m���Y��
		//     POST ���G�� URI�C ��m���Y�ҫ��w���귽���n�D�N�i�ϥ� GET�C
		RedirectMethod = 303,
		//
		// �K�n:
		//     �۷�� HTTP ���A 304�C System.Net.HttpStatusCode.NotModified ��ܥΤ�ݪ��֨����ƥ��O�̷s���A�C ���|�ǰe�귽�����e�C
		NotModified = 304,
		//
		// �K�n:
		//     �۷�� HTTP ���A 305�C System.Net.HttpStatusCode.UseProxy ��ܭn�D�A���Өϥ� proxy ���A������m���Y�����w��
		//     uri�C
		UseProxy = 305,
		//
		// �K�n:
		//     �۷�� HTTP ���A 306�C System.Net.HttpStatusCode.Unused �O��ĳ�������Ҳե�������w HTTP/1.1 �W��C
		Unused = 306,
		//
		// �K�n:
		//     �۷�� HTTP ���A 307�C System.Net.HttpStatusCode.TemporaryRedirect ��ܭn�D����T�O�����w��m���Y��
		//     URI�C ���즹���A�ɪ��w�]�ʧ@�O��`�P�^�������p�� Location ���Y�C ���l�n�D��k�� POST �ɡA���s�ɦV���n�D�]�|�ϥ� POST ��k�C
		TemporaryRedirect = 307,
		//
		// �K�n:
		//     �۷�� HTTP ���A 307�C System.Net.HttpStatusCode.RedirectKeepVerb ��ܭn�D����T�O�����w��m���Y��
		//     URI�C ���즹���A�ɪ��w�]�ʧ@�O��`�P�^�������p�� Location ���Y�C ���l�n�D��k�� POST �ɡA���s�ɦV���n�D�]�|�ϥ� POST ��k�C
		RedirectKeepVerb = 307,
		//
		// �K�n:
		//     �۷�� HTTP ���A 400�C System.Net.HttpStatusCode.BadRequest ��ܭn�D�i��L�k���Ѧ��A���C System.Net.HttpStatusCode.BadRequest
		//     �H�e�A�ΡA�S����L�����~�ɡA�Φp�G�T�������~�O�����Τ��ݭn�ۤv�����~�{���X�C
		BadRequest = 400,
		//
		// �K�n:
		//     �۷�� HTTP ���A 401�C System.Net.HttpStatusCode.Unauthorized ��ܭn�D���귽�ݭn���ҡC WWW ���Ҽ��Y�]�t�p��������Ҫ��ԲӸ�ơC
		Unauthorized = 401,
		//
		// �K�n:
		//     �۷�� HTTP ���A 402�C System.Net.HttpStatusCode.PaymentRequired �O�O�d�ѥ��ӨϥΡC
		PaymentRequired = 402,
		//
		// �K�n:
		//     �۷�� HTTP ���A 403�C System.Net.HttpStatusCode.Forbidden ��ܦ��A���ڵ��n�D�C
		Forbidden = 403,
		//
		// �K�n:
		//     �۷�� HTTP ���A 404�C System.Net.HttpStatusCode.NotFound ��ܭn�D���귽���s�b���A���W�C
		NotFound = 404,
		//
		// �K�n:
		//     �P HTTP ���A 405 �ۦP�C System.Net.HttpStatusCode.MethodNotAllowed ���ܭn�D���귽�����\�n�D��k �]POST
		//     �� GET�^�C
		MethodNotAllowed = 405,
		//
		// �K�n:
		//     �۷�� HTTP ���A 406�C System.Net.HttpStatusCode.NotAcceptable ��ܥΤ�ݤw�ϥ� Accept ���Y�A�]���|��������i�Ϊ���ܪk���귽�C
		NotAcceptable = 406,
		//
		// �K�n:
		//     �۷�� HTTP ���A 407�C System.Net.HttpStatusCode.ProxyAuthenticationRequired ��ܭn�D��
		//     proxy �ݭn���ҡC Proxy ���Ҽ��Y�]�t�p��������Ҫ��ԲӸ�ơC
		ProxyAuthenticationRequired = 407,
		//
		// �K�n:
		//     �P HTTP ���A 408 �ۦP�C System.Net.HttpStatusCode.RequestTimeout ��ܥΤ�ݥ��b�����A���w����n�D���ɶ����ǰe�n�D�C
		RequestTimeout = 408,
		//
		// �K�n:
		//     �۷�� HTTP ���A 409�C System.Net.HttpStatusCode.Conflict ��ܭn�D�i�ण�|����]�����A���W���Ĭ�C
		Conflict = 409,
		//
		// �K�n:
		//     �P HTTP ���A 410 �ۦP�C System.Net.HttpStatusCode.Gone ��ܭn�D���귽�w�L�k�A�ϥΡC
		// ��s: �N��I�s��request������w�g�����A�]����request�]�n����
		Gone = 410,
		//
		// �K�n:
		//     �۷�� HTTP ���A 411�C System.Net.HttpStatusCode.LengthRequired ��ܿ�|���n�����e���׼��Y�C
		LengthRequired = 411,
		//
		// �K�n:
		//     �۷�� HTTP ���A 412�C System.Net.HttpStatusCode.PreconditionFailed ��ܱ���]�w���ѡA���n�D�A�ӥB�L�k����n�D�C
		//     �p�G���A�Ҧp���󦡼��Y�]�w���� If-none-Match�A�Φp�G���ק�ۡC
		PreconditionFailed = 412,
		//
		// �K�n:
		//     �۷�� HTTP ���A 413�C System.Net.HttpStatusCode.RequestEntityTooLarge ��ܭn�D�Ӥj�A���A���B�z�C
		RequestEntityTooLarge = 413,
		//
		// �K�n:
		//     �۷�� HTTP ���A 414�C System.Net.HttpStatusCode.RequestUriTooLong ��� URI �Ӫ��C
		RequestUriTooLong = 414,
		//
		// �K�n:
		//     �۷�� HTTP ���A 415�C System.Net.HttpStatusCode.UnsupportedMediaType ��ܭn�D���䴩�������C
		UnsupportedMediaType = 415,
		//
		// �K�n:
		//     �۷�� HTTP ���A 416�C System.Net.HttpStatusCode.RequestedRangeNotSatisfiable ��ܸ�Ʀ۸귽�n�D���d��L�k�Ǧ^�A�άO�]���d�򪺶}�Y���e���귽�A�}�Y�ε������d��|�b�᪺�귽�C
		RequestedRangeNotSatisfiable = 416,
		//
		// �K�n:
		//     �۷�� HTTP ���A 417�C System.Net.HttpStatusCode.ExpectationFailed ��ܫ��w Expect ���Y���i�ण�ŦX���A���C
		ExpectationFailed = 417,
		//
		// �K�n:
		//     �۷�� HTTP ���A 426�C System.Net.HttpStatusCode.UpgradeRequired ��ܥΤ�����Ӥ����줣�P���q�T��w�A�Ҧp
		//     TLS/1.0�C
		UpgradeRequired = 426,
		//
		// �K�n:
		//     �۷�� HTTP ���A 500�C System.Net.HttpStatusCode.InternalServerError ��ܦb���A���W�w�o�ͤ@����~�C
		InternalServerError = 500,
		//
		// �K�n:
		//     �۷�� HTTP ���A 501�C System.Net.HttpStatusCode.NotImplemented ��ܦ��A�����䴩�n�D���禡�C
		NotImplemented = 501,
		//
		// �K�n:
		//     �P HTTP ���A 502 �ۦP�C System.Net.HttpStatusCode.BadGateway ��ܤ��~ proxy ���A���q�t�@�� proxy
		//     �έ�l���A�����줣���T���^���C
		BadGateway = 502,
		//
		// �K�n:
		//     �۷�� HTTP ���A 503�C System.Net.HttpStatusCode.ServiceUnavailable ��ܦ��A���ȮɵL�k�ϥΡA�q�`�O�]�����t���κ��@�C
		ServiceUnavailable = 503,
		//
		// �K�n:
		//     �۷�� HTTP ���A 504�C System.Net.HttpStatusCode.GatewayTimeout ��ܤ��~ proxy ���A���O�ɵ��ݨӦۥt�@��
		//     proxy �έ�l���A�����^���C
		GatewayTimeout = 504,
		//
		// �K�n:
		//     �P HTTP ���A 505 �ۦP�C System.Net.HttpStatusCode.HttpVersionNotSupported ��ܦ��A�����䴩�n�D��
		//     HTTP �����C
		HttpVersionNotSupported = 505,

		/// <summary>
		/// �r�w�q���A�P������_�s�u
		/// </summary>
		DisconnectedFromBlePhone,
		/// <summary>
		/// �r�w�q���Amtu�Ӥp
		/// </summary>
		MtuTooSmall,
		/// <summary>
		/// �r�w�q���A�Q�n�D�פ�
		/// </summary>
		ExitRequested,
		/// <summary>
		/// �r�w�q���A�S�����ɮ��ഫ�Xfile segment
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