/***************************************************************************** 
 *  @COPYRIGHT NOTICE 
 *  @Copyright (c) 2017, Microbeam software technology (Shanghai) co. LTD 
 *  @All rights reserved 
 
 *  @file     : Config.h 
 *  @version  : ver 1.0 
 
 *  @author   : yiwenqiang 
 *  @date     : 2017/7/6 10:20 
 *  @brief    : brief 
*****************************************************************************/ 
#ifndef _CONFIG_H_
#define _CONFIG_H_

struct cfg_info_t
{
	int gamesid;
	int clubid;
};

class CConfig
{
public:
	CConfig();
	virtual ~CConfig();

	void cleanup();

public:/*initialize*/
	bool init(const char *xml_conf);

public:/*set or get*/

	/*server ip and port*/
	const char* ds_listen_ip() const { return ds_listen_addr_; }
	int ds_listen_port() const { return ds_listen_port_; }

	const char* as_listen_ip() { return as_listen_addr_; }
	int as_listen_port() { return as_listen_port_; }

	const cfg_info_t* get_cfg_info() const { return &cfg_info_; }

protected:/** parse*/
	// parse config
	bool parse_cfg(TiXmlElement* element);
	bool parse_server(TiXmlElement* element);
	bool parse_connect(TiXmlElement* element);

protected:/** common*/
	static void ReadChar(TiXmlElement* pElement, const char* szNode, char& cValue, const char cDefault = 0);
	static void ReadBool(TiXmlElement* pElement, const char* szNode, bool& bValue, const bool bDefault = true);
	static void ReadShort(TiXmlElement* pElement, const char* szNode, short& sValue, const short sDefault = 0);
	static void ReadInteger(TiXmlElement* pElement, const char* szNode, int& nValue, const int nDefault = 0);
	static void ReadIntegerVector(TiXmlElement* pElement, const char* szNode, std::vector<int> &vctValue, const char *szToken = ", ");
	static void ReadString(TiXmlElement* pElement, const char* szNode, char* szValue, int nLen = 0, const char* szDefault = "");
	static void ReadStringEx(TiXmlElement* pElement, const char* szNode, string &stValue, const char* szDefault = "");
	static void ReadStringVector(TiXmlElement* pElement, const char* szNode, std::vector<string> &vctValue, const char *szToken = ", ");
	static void ReadFloat(TiXmlElement* pElement, const char* szNode, float& fValue, const float fDefault = 0.0f);
	static void ReadFloatVector(TiXmlElement* pElement, const char* szNode, std::vector<float> &vctValue, const char *szToken = ", ");

	void SplitString(std::string str, std::string pattern, std::vector<std::string>& items);

private:/** common data*/
	char ds_listen_addr_[64]; int ds_listen_port_;
	char as_listen_addr_[64]; int as_listen_port_;

	cfg_info_t cfg_info_;

private:
	std::string xml_name_;
};

#endif //_CONFIG_H_