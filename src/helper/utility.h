/*
 *  utility.h 
 *  utility
 *
 *  Created by Jevstein on 2018/4/9 17:40.
 *  Copyright @ 2018year Jevstein. All rights reserved.
 *
 */
#ifndef _UTILITY_H_
#define _UTILITY_H_
#include "globaldefine.h"
#include "platform.h"
#include <vector>

#ifdef YI_OS_WIN32
#	include <Winsock2.h>
#	pragma comment(lib, "Ws2_32.lib")
#else
#	include <netinet/in.h>
#endif//YI_OS_WIN32

namespace helper
{
	//内存池的内存地址对齐和长度按照2的幂取整：a必须为2的幂的时候，如：4、8、16
	#define ngx_align_ptr(p, a) (unsigned char *)(((unsigned long int)(p) + ((unsigned long int)a - 1)) & ~((unsigned long int)a - 1))

	/************************************************************************/
	/* macro: singleton pattern                                             */
	/************************************************************************/
	#define SINGLETON(type)\
		static inline type* GetInstancePtr()\
		{\
			static type _instance; \
			return &_instance; \
		}
    
	/************************************************************************/
	/* macro: safe delete                                                   */
	/************************************************************************/
	#ifndef SAFE_DELETE
		#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
		#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
		#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
		#define ARRAY(a)	sizeof(a)/sizeof(a[0])
	#endif//SAFE_DELETE

	/************************************************************************/
	/* function: the byte order conversion between Host and Network         */
	/************************************************************************/
    inline int64 htonl64(int64 n) {
        return (((int64)htonl(n)) << 32) | htonl(n >> 32);
    }
    inline int64 ntohl64(int64 n) {
        return (((int64)ntohl(n)) << 32) | ntohl(n >> 32);
    }
    //inline float ntoh_float(float net_float) {
    //	int host_int;
    //	host_int = ntohl(*((int *) &net_float));
    //	return *((float *) &host_int);
    //}
    //
    //inline float hton_float(float host_float) {
    //	int net_int32;
    //	net_int32 = htonl(*((int *) &host_float));
    //	return *((float *) &net_int32);
    //}
    inline double ntoh_double(double net_double) {
        int64 host_int64;
        host_int64 = ntohl64(*((int64 *)&net_double));
        return *((double *)&host_int64);
    }
    inline double hton_double(double host_double) {
        int64 net_int64;
		net_int64 = htonl64(*((int64 *)&host_double));
        return *((double *)&net_int64);
    }

	/************************************************************************/
	/* Utility class                                                        */
	/************************************************************************/
	/*CUtility*/
	class CUtility
	{
	public:
		CUtility();
		~CUtility(void);

	public:
		/** 获取当前时间
		@param [in]
		@return 返回当前时间，单位: 毫秒
		*/
        static unsigned int GetCurTime();

		///** 获取当前时间
		//@param [in]
		//@return 返回当前时间字符串
		//*/
		//static const char* GetNowTimeStr();

		/** base64编解码
		@param [inout] buf
		@param [in] src
		@param [in] src_len
		@return 正确返回实际长度,失败返回0
		*/
		static int Base64Encode(char* buf, const char* src, int src_len = 0);
		static int Base64Decode(char* buf, const char* src, int src_len = 0);

		// /** aes编解码
		// @param [in] key
		// @param [in] src_str
		// @param [in] input_filename
		// @param [in] output_filename
		// @return 正确返回实际长度,失败返回0
		// */
		// static std::string aes_encrypt_string(const std::string &key, const std::string &src_str);
		// static std::string aes_decrypt_string(const std::string &key, const std::string &src_str);
		// static bool aes_encrypt_file(const std::string &key, const std::string &input_filename, const std::string &output_filename);
		// static bool aes_decrypt_file(const std::string &key, const std::string &input_filename, const std::string &output_filename);

		/** MurMurHash算法：这是一种非加密HASH算法，性能很高，比传统的CRC32, MD5，SHA-1
		*（这两个算法都是加密hash算法，复杂度本身就很高，带来的性能上的损害也不可避免）
		* 等HASH算法要快很多，而且这个算法的碰撞率很低.http://murmurhash.googlepages.com/
		@param [in] key
		@param [in] len
		@param [in] seed 种子，最好用一个质数
		@return 正确返回实际长度,失败返回0
		*/
		static uint32_t murmur3_32(const char *key, uint32_t len, uint32_t seed = 17);

		/** IP地址及端口转换
		@param [in] address	原地址, eg: "127.0.0.1:51912"
		@param [out] ip		IP地址
		@param [out] port	端口号
		@return 返回：IP地址 & 端口 & 正确返回ture，失败返回false
		*/
		static std::string ip_from_addr(const std::string &address);
		static int port_from_addr(const std::string &address);
		static bool from_addr(const std::string &address, std::string &ip, int &port);

		/** 字符串转换为int64
		@param [in] str	数字字符串
		@return 返回：int64字符串
		*/
		static int64 str_to_int64(const char* str);

		/** 利用变长参数函数格式化字符串
		@param[in]	fmt 类似printf中第一个参数
		@remark		
		@return		格式化后的字符串
		*/
		static std::string format(const char* fmt, ...);


		/************************************************************************/
		/* Template function                                                    */
		/************************************************************************/

		/** 释放vector内存
		@param [in] vct	待释放内存的容器
		@return 无
		*/
		template <class T>
		static void delete_vector(std::vector<T>& vct)
		{
			std::vector<T>().swap(vct);
		}
    };
}
#endif//_UTILITY_H_
