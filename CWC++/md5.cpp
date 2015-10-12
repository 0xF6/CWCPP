#include "md5.h"
#include <Windows.h>
#include <wincrypt.h>


CString MD5(CString &in)
{
	CString ret;
    HCRYPTPROV hProv = 0;
    HCRYPTHASH hHash = 0;
	BYTE *rgbFile = (BYTE * ) (char *) in.data();
	DWORD cbRead = in.len();
    BYTE rgbHash[32];
    DWORD cbHash = 0;
    CHAR rgbDigits[] = "0123456789abcdef";
	if (!CryptAcquireContext(&hProv,
        NULL,
        NULL,
        PROV_RSA_FULL,
        CRYPT_VERIFYCONTEXT))
    {
		return "";
	}
	if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash))
    {
        CryptReleaseContext(hProv, 0);
		return "";
    }
	if (!CryptHashData(hHash, rgbFile, cbRead, 0))
	{
		CryptReleaseContext(hProv, 0);
		CryptDestroyHash(hHash);
		return "";
	}
	cbHash = 32;
    if (CryptGetHashParam(hHash, HP_HASHVAL, rgbHash, &cbHash, 0))
	{
		for (DWORD i = 0; i < cbHash; i++)
        {
            ret += rgbDigits[rgbHash[i] >> 4];
			ret +=  rgbDigits[rgbHash[i] & 0xf];
        }
	}
	CryptDestroyHash(hHash);
    CryptReleaseContext(hProv, 0);
	return ret;
}