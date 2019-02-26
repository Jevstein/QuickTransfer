#include "prefixhead.h"
#include "utility.h"
// #include "aes/aes_encryptor.h"

using namespace helper;

CUtility::CUtility()
{

}

CUtility::~CUtility(void)
{

}

unsigned int CUtility::GetCurTime()
{
#ifdef WIN32
    return GetTickCount();
#else
    struct timeval tv;
    gettimeofday(&tv, 0);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
#endif
}

//const char* CUtility::GetNowTimeStr()
//{
//	static char szNow[24] = { 0 };
//	time_t tiNow = time(0);
//	tm * tmNow = localtime(&tiNow);
//	//sprintf(szNow, "%d-%d-%d %d:%d:%d"
//	//	, 1900 + tmNow->tm_year, tmNow->tm_mon + 1, tmNow->tm_mday, tmNow->tm_hour, tmNow->tm_min, tmNow->tm_sec);
//
//	strftime(szNow, 26, "%b%d %H:%M:%S", tmNow);
//
//	return szNow;
//}

// 从双字中取单字节
#define B0(a) (a & 0xFF)
#define B1(a) (a >> 8 & 0xFF)
#define B2(a) (a >> 16 & 0xFF)
#define B3(a) (a >> 24 & 0xFF)

std::string g_strEn="dXBkYXRlIHRiX2FjY291bnQgc2V0IGFfcGFzc3dvcmQ9ZW5jb2RlKGNvbmNhdChhX3Bhc3N3b3JkLCAnMDAwJyksICclcycpOw==";
std::string g_strDe="dXBkYXRlIHRiX2FjY291bnQgc2V0IGFfcGFzc3dvcmQ9bGVmdChkZWNvZGUoYV9wYXNzd29yZCwgJyVzJyksIDMyKSB3aGVyZSBsZW5ndGgoYV9wYXNzd29yZCkgPiAzMjs=";

char GetB64Char(int index)
{
    const char szBase64Table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
		  //const char szBase64Table[] = "HaveItoldyoulatelythatIloveyouABCDEFGHIJKLMnopqrstuvwxyz012345+/";
    
    if (index >= 0 && index < 64)
        return szBase64Table[index];
    
    return '=';
}

int GetB64Index(char ch)
{
    int index = -1;
    if (ch >= 'A' && ch <= 'Z')
    {
        index = ch - 'A';
    }
    else if (ch >= 'a' && ch <= 'z')
    {
        index = ch - 'a' + 26;
    }
    else if (ch >= '0' && ch <= '9')
    {
        index = ch - '0' + 52;
    }
    else if (ch == '+')
    {
        index = 62;
    }
    else if (ch == '/')
    {
        index = 63;
    }
    
    return index;
}

// 编码后的长度一般比原文多占1/3的存储空间，请保证base64code有足够的空间
int CUtility::Base64Encode(char * base64code, const char * src, int src_len)
{
    if (src_len == 0)
        src_len = strlen(src);
    
    int len = 0;
    unsigned char* psrc = (unsigned char*)src;
    char * p64 = base64code;
    int i;
    for (i = 0; i < src_len - 3; i += 3)
    {
        unsigned long ulTmp = *(unsigned long*)psrc;
        register int b0 = GetB64Char((B0(ulTmp) >> 2) & 0x3F);
        register int b1 = GetB64Char((B0(ulTmp) << 6 >> 2 | B1(ulTmp) >> 4) & 0x3F);
        register int b2 = GetB64Char((B1(ulTmp) << 4 >> 2 | B2(ulTmp) >> 6) & 0x3F);
        register int b3 = GetB64Char((B2(ulTmp) << 2 >> 2) & 0x3F);
        *((unsigned long*)p64) = b0 | b1 << 8 | b2 << 16 | b3 << 24;
        len += 4;
        p64  += 4;
        psrc += 3;
    }
    
    // 处理最后余下的不足3字节的饿数据
    if (i < src_len)
    {
        int rest = src_len - i;
        unsigned long ulTmp = 0;
        for (int j = 0; j < rest; ++j)
        {
            *(((unsigned char*)&ulTmp) + j) = *psrc++;
        }
        
        p64[0] = GetB64Char((B0(ulTmp) >> 2) & 0x3F);
        p64[1] = GetB64Char((B0(ulTmp) << 6 >> 2 | B1(ulTmp) >> 4) & 0x3F);
        p64[2] = rest > 1 ? GetB64Char((B1(ulTmp) << 4 >> 2 | B2(ulTmp) >> 6) & 0x3F) : '=';
        p64[3] = rest > 2 ? GetB64Char((B2(ulTmp) << 2 >> 2) & 0x3F) : '=';
        p64 += 4;
        len += 4;
    }
    
    *p64 = '\0';
    
    return len;
}

// 解码后的长度一般比原文少用占1/4的存储空间，请保证buf有足够的空间
int CUtility::Base64Decode(char * buf, const char * base64code, int src_len)
{
    if (src_len == 0)
        src_len = strlen(base64code);
    
    int len = 0;
    unsigned char* psrc = (unsigned char*)base64code;
    char * pbuf = buf;
    int i;
    for (i = 0; i < src_len - 4; i += 4)
    {
        unsigned long ulTmp = *(unsigned long*)psrc;
        
        register int b0 = (GetB64Index((char)B0(ulTmp)) << 2 | GetB64Index((char)B1(ulTmp)) << 2 >> 6) & 0xFF;
        register int b1 = (GetB64Index((char)B1(ulTmp)) << 4 | GetB64Index((char)B2(ulTmp)) << 2 >> 4) & 0xFF;
        register int b2 = (GetB64Index((char)B2(ulTmp)) << 6 | GetB64Index((char)B3(ulTmp)) << 2 >> 2) & 0xFF;
        
        *((unsigned long*)pbuf) = b0 | b1 << 8 | b2 << 16;
        psrc  += 4;
        pbuf += 3;
        len += 3;
    }
    
    // 处理最后余下的不足4字节的饿数据
    if (i < src_len)
    {
        int rest = src_len - i;
        unsigned long ulTmp = 0;
        for (int j = 0; j < rest; ++j)
        {
            *(((unsigned char*)&ulTmp) + j) = *psrc++;
        }
        
        register int b0 = (GetB64Index((char)B0(ulTmp)) << 2 | GetB64Index((char)B1(ulTmp)) << 2 >> 6) & 0xFF;
        *pbuf++ = b0;
        len  ++;
        
        if ('=' != B1(ulTmp) && '=' != B2(ulTmp))
        {
            register int b1 = (GetB64Index((char)B1(ulTmp)) << 4 | GetB64Index((char)B2(ulTmp)) << 2 >> 4) & 0xFF;
            *pbuf++ = b1;
            len  ++;
        }
        
        if ('=' != B2(ulTmp) && '=' != B3(ulTmp))
        {
            register int b2 = (GetB64Index((char)B2(ulTmp)) << 6 | GetB64Index((char)B3(ulTmp)) << 2 >> 2) & 0xFF;
            *pbuf++ = b2;
            len  ++;
        }
        
    }
    
    *pbuf = '\0'; 
    
    return len;
}

std::string CUtility::ip_from_addr(const std::string &address)
{
	std::size_t pos = address.find("|");
	if (pos == -1)
		return "";

	return address.substr(0, pos);
}

int CUtility::port_from_addr(const std::string &address)
{
	std::size_t pos = address.find("|");
	if (pos == -1)
		return 0;

	return atoi(address.substr(pos+1).c_str());
}

bool CUtility::from_addr(const std::string &address, std::string &ip, int &port)
{
	std::size_t pos = address.find("|");
	if (pos == -1)
		return false;

	ip = address.substr(0, pos);
	port = atoi(address.substr(pos+1).c_str());

	return true;
}

int64 CUtility::str_to_int64(const char* str)
{
	if (str == NULL)
		return 0;

	return atoll(str);
}

// std::string CUtility::aes_encrypt_string(const std::string &key, const std::string &src_str)
// {
// 	AesEncryptor aes((unsigned char *)key.c_str());
// 	return aes.EncryptString(src_str);
// }

// std::string CUtility::aes_decrypt_string(const std::string &key, const std::string &src_str)
// {
// 	AesEncryptor aes((unsigned char *)key.c_str());
// 	return aes.DecryptString(src_str);
// }

// bool CUtility::aes_encrypt_file(const std::string &key, const std::string &input_filename, const std::string &output_filename)
// {
// 	AesEncryptor aes((unsigned char *)key.c_str());
// 	return aes.EncryptTxtFile(input_filename.c_str(), output_filename.c_str());
// }

// bool CUtility::aes_decrypt_file(const std::string &key, const std::string &input_filename, const std::string &output_filename)
// {
// 	AesEncryptor aes((unsigned char *)key.c_str());
// 	return aes.DecryptTxtFile(input_filename.c_str(), output_filename.c_str());
// }

uint32_t CUtility::murmur3_32(const char *key, uint32_t len, uint32_t seed /*= 17*/)
{
	static const uint32_t c1 = 0xcc9e2d51;
	static const uint32_t c2 = 0x1b873593;
	static const uint32_t r1 = 15;
	static const uint32_t r2 = 13;
	static const uint32_t m = 5;
	static const uint32_t n = 0xe6546b64;

	uint32_t hash = seed;

	const int nblocks = len / 4;
	const uint32_t *blocks = (const uint32_t *)key;
	int i;
	for (i = 0; i < nblocks; i++)
	{
		uint32_t k = blocks[i];
		k *= c1;
		k = (k << r1) | (k >> (32 - r1));
		k *= c2;

		hash ^= k;
		hash = ((hash << r2) | (hash >> (32 - r2))) * m + n;
	}

	const uint8_t *tail = (const uint8_t *)(key + nblocks * 4);
	uint32_t k1 = 0;

	switch (len & 3)
	{
	case 3:
		k1 ^= tail[2] << 16;
	case 2:
		k1 ^= tail[1] << 8;
	case 1:
		k1 ^= tail[0];

		k1 *= c1;
		k1 = (k1 << r1) | (k1 >> (32 - r1));
		k1 *= c2;
		hash ^= k1;
	}

	hash ^= len;
	hash ^= (hash >> 16);
	hash *= 0x85ebca6b;
	hash ^= (hash >> 13);
	hash *= 0xc2b2ae35;
	hash ^= (hash >> 16);

	return hash;
}

std::string CUtility::format(const char* fmt, ...)
{
	std::string var_str;

	va_list	args;
	va_start(args, fmt);
	int len = p_vscprintf(fmt, args);
	if (len > 0)
	{
		std::vector<char> buf(len + 1);
		p_vsnprintf(&buf.front(), buf.capacity(), fmt, args);//vsprintf(&buf.front(), fmt, args);
		var_str.assign(buf.begin(), buf.end() - 1);
	}
	va_end(args);

	return var_str;
}
