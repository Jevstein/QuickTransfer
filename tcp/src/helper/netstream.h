/*
 *  netstream.h 
 *  netstream
 *
 *  Created by Jevstein on 2018/4/10 18:55.
 *  Copyright @ 2018year Jevstein. All rights reserved.
 *
 */
#ifndef _NET_STREAM_H_
#define _NET_STREAM_H_
#include <assert.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <sys/types.h>
#include "utility.h"

#ifdef WIN32
#include <Winsock2.h>
#pragma comment(lib, "Ws2_32.lib")
//#define snprintf _snprintf
//#define snprintf _snprintf_s
#else
#include <netinet/in.h>
#endif
#include "globaldefine.h"
#include <string>
#include <vector>
#include <map>

/************************************************************************/
/* macro: write and read data                                           */
/************************************************************************/
#define DEF_BASETYPE_SERIA_WRITE_1(T, type) \
	inline T& operator<<(const type& t) \
	{ \
		WriteData(&t, sizeof(t)); \
		return *this; \
	}
#define DEF_BASETYPE_SERIA_WRITE_2(T, type) \
	inline T& operator<<(const type& t) \
	{ \
		type t2 = htons(t); \
		WriteData(&t2, sizeof(type)); \
		return *this; \
	}
#define DEF_BASETYPE_SERIA_WRITE_4(T, type) \
	inline T& operator<<(const type& t) \
	{ \
		type t2 = htonl(t); \
		WriteData(&t2, sizeof(type)); \
		return *this; \
	}
#define DEF_BASETYPE_SERIA_WRITE_8(T, type) \
	inline T& operator<<(const type& t) \
	{ \
		type t2 = htonl64(t); \
		WriteData(&t2, sizeof(type)); \
		return *this; \
	}

/*#define DEF_BASETYPE_SERIA_WRITE_FLOAT(type) \
	inline COutputStream& operator<<(const type& t) \
	{ \
		type t2 = hton_float(t); \
		WriteData(&t2, sizeof(type)); \
		return *this; \
	}

#define DEF_BASETYPE_SERIA_WRITE_FLOAT(type) \
	inline COutputStream& operator<<(const type& t) \
	{ \
		char buffer[64];\
		p_snprintf(buffer, 64, "%f", t);\
		int nLen = strlen(buffer);\
		WriteData(&nLen, sizeof(nLen));\
		WriteData(buffer, nLen);\
		return *this; \
	}
#define DEF_BASETYPE_SERIA_READ_FLOAT(type) \
	inline CInputStream& operator>>(type& t)  \
	{ \
		char buffer[64];\
		memset(buffer, 0, sizeof(buffer));\
		int nLen;\
		ReadData(&nLen, sizeof(int));\
		ReadData(buffer, nLen); \
		t = atof(buffer);\
		return *this; \
	}*/

#define DEF_BASETYPE_SERIA_READ_1(T, type) \
	inline T& operator>>(type& t)  \
	{ \
		ReadData(&t, sizeof(type)); \
		return *this; \
	}

#define DEF_BASETYPE_SERIA_READ_2(T, type) \
	inline T& operator>>(type& t)  \
	{ \
		ReadData(&t, sizeof(type)); \
		t = ntohs(t); \
		return *this; \
	}
#define DEF_BASETYPE_SERIA_READ_4(T, type) \
	inline T& operator>>(type& t)  \
	{ \
		ReadData(&t, sizeof(type)); \
		t = ntohl(t); \
		return *this; \
	}
#define DEF_BASETYPE_SERIA_READ_8(T, type) \
	inline T& operator>>(type& t)  \
	{ \
		ReadData(&t, sizeof(type)); \
		t = ntohl64(t); \
		return *this; \
	}

#define DEF_BASETYPE_SERIA_WRITE_FLOAT(T, type) \
	inline T& operator<<(const type& t) \
	{ \
		char buffer[64]; \
		p_snprintf(buffer, 64, "%f", t); \
		std::string strFloat = buffer; \
		*this << strFloat; \
		return *this; \
	}
#define DEF_BASETYPE_SERIA_READ_FLOAT(T, type) \
	inline T& operator>>(type& t)  \
	{ \
		std::string strFloat; \
		*this >> strFloat; \
		t = atof(strFloat.c_str()); \
		return *this; \
	}

/************************************************************************/
/* macro: write and read data                                           */
/************************************************************************/
struct pt_header
{
	short opcode;
};

#ifdef NO_INLINE_SERIAL
    /** NET_PACKET*/
    #define NET_PACKET(packet) \
    struct packet; \
        CInputStream& operator>>(CInputStream& is, packet& pt); \
        COutputStream& operator<<(COutputStream& os, const packet& pt); \
    struct packet : public pt_header

    #define NET_GET(packet) \
        CInputStream& operator>>(CInputStream& is, packet& pt)
    #define NET_PUT(packet) \
        COutputStream& operator<<(COutputStream& os, const packet& pt)

    /** NET_PACKET_EX*/
    #define NET_PACKET_EX(packet, parent) \
    struct packet; \
        CInputStream& operator>>(CInputStream& is, packet& pt); \
        COutputStream& operator<<(COutputStream& os, const packet& pt); \
    struct packet : public parent

#else
    /** NET_PACKET*/
    #define NET_PACKET(packet) \
    struct packet; \
        inline CInputStream& operator>>(CInputStream& is, packet& pt); \
        inline COutputStream& operator<<(COutputStream& os, const packet& pt); \
    struct packet : public pt_header

    #define NET_GET(packet) \
        CInputStream& operator>>(CInputStream& is, packet& pt)
    #define NET_PUT(packet) \
        COutputStream& operator<<(COutputStream& os, const packet& pt)

    /** NET_PACKET_EX*/
    #define NET_PACKET_EX(packet, parent) \
    struct packet; \
        inline CInputStream& operator>>(CInputStream& is, packet& pt); \
        inline COutputStream& operator<<(COutputStream& os, const packet& pt); \
    struct packet : public parent
#endif//NO_INLINE_SERIAL

/************************************************************************/
/* macro: dispatch message                                              */
/************************************************************************/
#define DISPATCH_MESSAGE(packet) { \
            packet obj; \
            is >> obj; \
            OnPacket(obj); \
            break;	\
        }

#define DISPATCH_MESSAGE_REF(packet, parm) { \
            packet pt(parm); \
            is >> pt; \
            OnPacket(pt); \
            break;	\
        }

// network message
#define DECLARE_NETMESSAGE_MAP()		int OnNetMessage(const char* pData, int nLen);
#define BEGIN_NETMESSAGE_MAP(thisClass)	int thisClass::OnNetMessage(const char* pData, int nLen) {\
                                            CInputStream is(pData, nLen); \
                                            short opcode = 0; \
                                            is >> opcode; \
                                            is.Reset(); \
                                            switch (opcode)\
                                            {
#define HANDLE_NONE_NETMESSAGE(id)				case id:                           break;
#define HANDLE_NETMESSAGE(id, packet)			case id: DISPATCH_MESSAGE(packet); break;
#define END_NETMESSAGE_MAP()					default: return opcode;\
                                            }\
                                            return 0; \
                                        }

// database message
#define DECLARE_DBMESSAGE_MAP()			int OnDBMessage(int opcode, IDBResultSet* rst);
#define BEGIN_DBMESSAGE_MAP(thisClass)	int thisClass::OnDBMessage(int opcode, IDBResultSet* rst) {\
                                            switch (opcode)\
                                            {
#define HANDLE_NONE_DBMESSAGE(id)			case id:			break;
#define HANDLE_DBMESSAGE(id, func)			case id: func(rst); break;
#define END_DBMESSAGE_MAP()					default: return opcode;\
                                            }\
                                            return 0; \
                                        }

/************************************************************************/
/* class: Input and Output stream                                       */
/************************************************************************/
namespace ystd
{
	// 输入流类
	class CInputStream
	{
	public:
		CInputStream(const char* pData, int nLen);
		void Init(const char* pData, int nLen);
		void Reset() { m_nOffset = 0; }
		bool ReadData(void* pData, int nLen);

		//CInputStream& operator>>(std::basic_string<char>& str);
		//template<class T>
		//CInputStream& operator>>(std::vector<T>& obj);
		//template<class T>
		//CInputStream& operator>>(std::vector<T*>& obj);
		//template<class KEY, class T>
		//CInputStream& operator>>(std::map<KEY, T>& obj);
		//template<class KEY, class T>
		//CInputStream& operator>>(std::map<KEY, T*>& obj);
		CInputStream& operator>>(std::basic_string<char>& str)
		{
			short len = -1;
			*this >> len;
			if (len < 0 || len > 1024 * 10 || len > m_nLen - m_nOffset)
			{
				//LOG_ERR("operator>>(std::basic_string<char>& str) size= %d", len);
				//assert(false);
				str.clear();
			}
			else
			{
				char szStr[1024 * 10];
				ReadData(szStr, len);
				str.reserve(len);
				str.assign(szStr, len);
			}
			return *this;
		}
		template<class T>
		CInputStream& operator>>(std::vector<T>& obj)
		{
			short len = -1;
			*this >> len;

			if (len < 0 || len > 512 || len > m_nLen - m_nOffset)
			{
				//LOG_ERR("operator<<(const std::vector<T>& obj), size=%d", len);
				//assert(false);
				obj.clear();
			}
			else
			{
				obj.reserve(len);
				for (int i = 0; i < len; ++i)
				{
					T element;
					*this >> element;
					obj.push_back(element);
				}
			}
			return *this;
		}
		template<class T>
		CInputStream& operator>>(std::vector<T*>& obj)
		{
			short len = -1;
			*this >> len;

			if (len < 0 || len > 512 || len > m_nLen - m_nOffset)
			{
				//LOG_ERR("operator<<(const std::vector<T*>& obj), size=%d", len);
				//assert(false);
				obj.clear();
			}
			else
			{
				obj.reserve(len);
				for (int i = 0; i < len; ++i)
				{
					T* element = new T;
					*this >> *element;
					obj.push_back(element);
				}
			}
			return *this;
		}
		template<class KEY, class T>
		CInputStream& operator>>(std::map<KEY, T>& obj)
		{
			short len = -1;
			*this >> len;
			if (len < 0 || len > 512 || len > m_nLen - m_nOffset)
			{
				//LOG_ERR("operator>>(std::map<KEY, T>& obj), size=%d", len);
				//assert(false);
				obj.clear();
			}
			else
			{
				for (int i = 0; i < len; ++i)
				{
					KEY key;
					T second;
					*this >> key >> second;
					obj[key] = second;
				}
			}
			return *this;
		}
		template<class KEY, class T>
		CInputStream& operator>>(std::map<KEY, T*>& obj)
		{
			short len = -1;
			*this >> len;
			if (len < 0 || len > 512 || len > m_nLen - m_nOffset)
			{
				//LOG_ERR("operator>>(std::map<KEY, T*>& obj), size=%d", len);
				//assert(false);
				obj.clear();
			}
			else
			{
				for (int i = 0; i < len; ++i)
				{
					KEY key;
					T* second = new T;
					*this >> key >> *second;
					obj[key] = second;
				}
			}
			return *this;
		}
		DEF_BASETYPE_SERIA_READ_1(CInputStream, char);
		DEF_BASETYPE_SERIA_READ_1(CInputStream, unsigned char);
		DEF_BASETYPE_SERIA_READ_2(CInputStream, short int);
		DEF_BASETYPE_SERIA_READ_2(CInputStream, unsigned short int);
		DEF_BASETYPE_SERIA_READ_4(CInputStream, int);
		DEF_BASETYPE_SERIA_READ_4(CInputStream, unsigned int);
		DEF_BASETYPE_SERIA_READ_FLOAT(CInputStream, float);
		DEF_BASETYPE_SERIA_READ_8(CInputStream, int64);

	private:
		char* m_pData;
		int m_nLen;
		int m_nOffset;
	};

	// 输出流类
	class COutputStream
	{
	public:
		COutputStream();
		COutputStream(char* pData, int nMaxLen);
		bool WriteData(const void* pData, int nLen);
		char* GetData();
		int GetLength();

		//COutputStream& operator<<(const std::basic_string<char>& str);
		//template<class T>
		//COutputStream& operator<<(const std::vector<T>& obj);
		//template<class T>
		//COutputStream& operator<<(const std::vector<T*>& obj);
		//template<typename KEY, typename T>
		//COutputStream& operator<<(const std::map<KEY, T>& obj);
		//template<typename KEY, typename T>
		//COutputStream& operator<<(const std::map<KEY, T*>& obj);
		COutputStream& operator<<(const std::basic_string<char>& str)
		{
			short len = (short)str.size();
			if (len > m_nMaxLen - m_nOffset || len > 1024 * 10)
			{
				//LOG_ERR("operator<<(const std::basic_string<char>& str) size= %d leave=%d", len, m_nMaxLen - m_nOffset);
				//assert(false);
				len = 0;
				*this << len;
			}
			else
			{
				*this << len;
				WriteData(str.data(), (int)str.length());
			}
			return *this;
		}
		template<class T>
		COutputStream& operator<<(const std::vector<T>& obj)
		{
			short len = (short)obj.size();
			if (len > m_nMaxLen - m_nOffset || len > 512)
			{
				//LOG_ERR("operator<<(const std::vector<T>& obj), max=%d, offset=%d, size=%d", m_nMaxLen, m_nOffset, len);
				//assert(false);
				len = 0;
				*this << len;
			}
			else
			{
				*this << len;
				for (int i = 0; i < len; ++i)
				{
					*this << obj[i];
				}
			}
			return *this;
		}
		template<class T>
		COutputStream& operator<<(const std::vector<T*>& obj)
		{
			short len = (short)obj.size();
			if (len > m_nMaxLen - m_nOffset || len > 512)
			{
				//LOG_ERR("operator<<(const std::vector<T*>& obj), size=%d", len);
				//assert(false);
				len = 0;
				*this << len;
			}
			else
			{
				*this << len;
				for (int i = 0; i < len; ++i)
				{
					*this << *obj[i];
				}
			}
			return *this;
		}
		template<typename KEY, typename T>
		COutputStream& operator<<(const std::map<KEY, T>& obj)
		{
			short len = (short)obj.size();
			if (len > m_nMaxLen - m_nOffset || len > 512)
			{
				//LOG_ERR("operator<<(const std::map<KEY, T>& obj), %d", len);
				//assert(false);
				len = 0;
				*this << len;
			}
			else
			{
				*this << len;
				typename std::map<KEY, T>::const_iterator iter = obj.begin();
				for (; iter != obj.end(); ++iter)
				{
					*this << iter->first << iter->second;
				}
			}
			return *this;
		}
		template<typename KEY, typename T>
		COutputStream& operator<<(const std::map<KEY, T*>& obj)
		{
			short len = (short)obj.size();
			if (len > m_nMaxLen - m_nOffset || len > 512)
			{
				//LOG_ERR("operator<<(const std::map<KEY, T*>& obj), %d", len);
				//assert(false);
				len = 0;
				*this << len;
			}
			else
			{
				*this << len;
				typename std::map<KEY, T*>::const_iterator iter = obj.begin();
				for (; iter != obj.end(); ++iter)
				{
					*this << iter->first << *(iter->second);
				}
			}
			return *this;
		}
		DEF_BASETYPE_SERIA_WRITE_1(COutputStream, char);
		DEF_BASETYPE_SERIA_WRITE_1(COutputStream, unsigned char);
		DEF_BASETYPE_SERIA_WRITE_2(COutputStream, short int);
		DEF_BASETYPE_SERIA_WRITE_2(COutputStream, unsigned short int);
		DEF_BASETYPE_SERIA_WRITE_4(COutputStream, int);
		DEF_BASETYPE_SERIA_WRITE_4(COutputStream, unsigned int);
		DEF_BASETYPE_SERIA_WRITE_FLOAT(COutputStream, float);
		DEF_BASETYPE_SERIA_WRITE_8(COutputStream, int64);

	private:
		char* m_pData;
		char m_pOwnerBuffer[MAX_PACKET_LEN];
		int m_nMaxLen;
		int m_nOffset;
	};

	// 输入输出流类
	class CIONetStream
	{
	public:
		CIONetStream();
		CIONetStream(const char* data, int size);
		~CIONetStream();

		void Reset() { _offset_is = 0; }

		char* GetData() { return _data; }
		int GetLength() { return _capacity; }

	public:
		// CInputStream
		CIONetStream& operator>>(std::basic_string<char>& str);
		template<class T>
		CIONetStream& operator>>(std::vector<T>& obj);
		template<class T>
		CIONetStream& operator>>(std::vector<T*>& obj);
		template<class KEY, class T>
		CIONetStream& operator>>(std::map<KEY, T>& obj);
		template<class KEY, class T>
		CIONetStream& operator>>(std::map<KEY, T*>& obj);
		DEF_BASETYPE_SERIA_READ_1(CIONetStream, char);
		DEF_BASETYPE_SERIA_READ_1(CIONetStream, unsigned char);
		DEF_BASETYPE_SERIA_READ_2(CIONetStream, short int);
		DEF_BASETYPE_SERIA_READ_2(CIONetStream, unsigned short int);
		DEF_BASETYPE_SERIA_READ_4(CIONetStream, int);
		DEF_BASETYPE_SERIA_READ_4(CIONetStream, unsigned int);
		DEF_BASETYPE_SERIA_READ_FLOAT(CIONetStream, float);
		DEF_BASETYPE_SERIA_READ_8(CIONetStream, int64);

		// COutputStream
		CIONetStream& operator<<(const std::basic_string<char>& str);
		template<class T>
		CIONetStream& operator<<(const std::vector<T>& obj);
		template<class T>
		CIONetStream& operator<<(const std::vector<T*>& obj);
		template<typename KEY, typename T>
		CIONetStream& operator<<(const std::map<KEY, T>& obj);
		template<typename KEY, typename T>
		CIONetStream& operator<<(const std::map<KEY, T*>& obj);
		DEF_BASETYPE_SERIA_WRITE_1(CIONetStream, char);
		DEF_BASETYPE_SERIA_WRITE_1(CIONetStream, unsigned char);
		DEF_BASETYPE_SERIA_WRITE_2(CIONetStream, short int);
		DEF_BASETYPE_SERIA_WRITE_2(CIONetStream, unsigned short int);
		DEF_BASETYPE_SERIA_WRITE_4(CIONetStream, int);
		DEF_BASETYPE_SERIA_WRITE_4(CIONetStream, unsigned int);
		DEF_BASETYPE_SERIA_WRITE_FLOAT(CIONetStream, float);
		DEF_BASETYPE_SERIA_WRITE_8(CIONetStream, int64);

	protected:
		bool ReadData(void* data, int size);
		bool WriteData(const void* data, int size);
		bool WriteSize(short int size);

	private:
		void init();
		void release();

	private:
		char* _data;
		int _capacity;
		int _offset_is;
		int _offset_os;
	};
}

#endif //_NET_STREAM_H_
