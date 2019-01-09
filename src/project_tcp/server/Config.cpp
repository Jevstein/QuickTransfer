#include "PrefixHead.h"
#include "Config.h"
#include "Delegate.h"

CConfig::CConfig()
{
}

CConfig::~CConfig()
{
}

void CConfig::cleanup()
{
}

void CConfig::ReadChar(TiXmlElement* pElement, const char* szNode, char& cValue, const char cDefault /*= 0*/)
{
	int nTemp;
	ReadInteger(pElement, szNode, nTemp, cDefault);
	cValue = nTemp;
}

void CConfig::ReadBool(TiXmlElement* pElement, const char* szNode, bool& bValue, const bool bDefault /*= true*/)
{
	int nTemp;
	ReadInteger(pElement, szNode, nTemp, bDefault ? 1 : 0);
	bValue = (nTemp == 1);
}

void CConfig::ReadShort(TiXmlElement* pElement, const char* szNode, short& sValue, const short sDefault)
{
	int nTemp;
	ReadInteger(pElement, szNode, nTemp, sDefault);
	sValue = nTemp;
}

void CConfig::ReadInteger(TiXmlElement* pElement, const char* szNode, int& nValue, const int nDefault)
{
	if (!pElement->Attribute(szNode, &nValue))
	{
		nValue = nDefault;
	}
}

void CConfig::ReadIntegerVector(TiXmlElement* pElement, const char* szNode, std::vector<int> &vctValue, const char *szToken /*= ", "*/)
{
	const char* pstr = pElement->Attribute(szNode);
	if (pstr)
	{
		string strValueArray(pstr);
		char *pArray = (char*)strValueArray.c_str();
		char *tokenPtr = strtok(pArray, szToken);
		while (tokenPtr != NULL)
		{
			vctValue.push_back(atoi(tokenPtr));
			tokenPtr = strtok(NULL, szToken);
		}
	}
}

void CConfig::ReadString(TiXmlElement* pElement, const char* szNode, char* szValue, int nLen/* = 0*/, const char* szDefault/* = ""*/)
{
	const char* pstr = pElement->Attribute(szNode);
	if (nLen == 0)
	{
		nLen = strlen(pstr);
	}
	if (pstr)
	{
		strncpy(szValue, pstr, nLen);
	}
	else
	{
		strncpy(szValue, szDefault, nLen);
	}
}

void CConfig::ReadStringEx(TiXmlElement* pElement, const char* szNode, string &stValue, const char* szDefault /*= ""*/)
{
	const char* pstr = pElement->Attribute(szNode);
	if (pstr)
	{
		stValue = pstr;
	}
	else
	{
		stValue = szDefault;
	}
}

void CConfig::ReadStringVector(TiXmlElement* pElement, const char* szNode, std::vector<string> &vctValue, const char *szToken /*= ", "*/)
{
	const char* pstr = pElement->Attribute(szNode);
	if (pstr)
	{
		string strValueArray(pstr);
		char *pArray = (char*)strValueArray.c_str();
		char *tokenPtr = strtok(pArray, szToken);
		while (tokenPtr != NULL)
		{
			vctValue.push_back(tokenPtr);
			tokenPtr = strtok(NULL, szToken);
		}
	}
}

void CConfig::ReadFloat(TiXmlElement* pElement, const char* szNode, float& fValue, const float fDefault)
{
	const char* pstr = pElement->Attribute(szNode);
	if (pstr)
	{
		fValue = atof(pstr);
	}
	else
	{
		fValue = fDefault;
	}
}

void CConfig::ReadFloatVector(TiXmlElement* pElement, const char* szNode, std::vector<float> &vctValue, const char *szToken /*= ", "*/)
{
	const char* pstr = pElement->Attribute(szNode);
	if (pstr)
	{
		string strValueArray(pstr);
		char *pArray = (char*)strValueArray.c_str();
		char *tokenPtr = strtok(pArray, szToken);
		while (tokenPtr != NULL)
		{
			vctValue.push_back(atof(tokenPtr));
			tokenPtr = strtok(NULL, szToken);
		}
	}
}

void CConfig::SplitString(std::string str, std::string pattern, std::vector<std::string>& items)
{
	size_t pos = 0;
	str += pattern;//扩展字符串以方便操作  
	size_t size = str.size();
	for (size_t i = 0; i<size; i++)
	{
		pos = str.find(pattern, i);
		if (pos < size)
		{
			std::string s = str.substr(i, pos - i);
			//int times = atoi(s.c_str());
			items.push_back(s);
			i = pos + pattern.size() - 1;
		}
	}
}

bool CConfig::init(const char* xml_name)
{
	xml_name_ = xml_name;

	TiXmlDocument doc(xml_name);
	if (!doc.LoadFile())
	{
		LOG_ERR("the file='%s' is not existed!", xml_name);
		return false;
	}

	// parser common config
	TiXmlElement* element = doc.FirstChildElement("config");
	if (!parse_cfg(element))
	{
		LOG_ERR("failed to parse config!");
		return false;
	}

	return true;
}

bool CConfig::parse_cfg(TiXmlElement* element)
{
	if (!element)
		return false;

	////日志输出模式
	//ReadInteger(pNext, "log", _log_mode);
	//ReadInteger(pNext, "loglevel", _log_level, 15);

	TiXmlElement *srv = element->FirstChildElement("server");
	if (!parse_server(srv))
	{
		LOG_ERR("failed to parse server!");
		return false;
	}

	TiXmlElement *conn = element->FirstChildElement("connect");
	if (!parse_connect(conn))
	{
		LOG_ERR("failed to parse connect!");
		return false;
	}

	return true;
}

bool CConfig::parse_server(TiXmlElement* element)
{
	if (!element)
		return false;

	ReadInteger(element, "gamesid", cfg_info_.gamesid, 1);
	ReadInteger(element, "clubid", cfg_info_.clubid, 0);

	return true;
}

bool CConfig::parse_connect(TiXmlElement* element)
{
	if (!element)
		return false;

	//AuthenServer
	TiXmlElement *srv = element->FirstChildElement("authenserver");
	if (!srv)
	{
		LOG_ERR("failed to parse authenserver!");
		return false;
	}
	ReadString(srv, "listenip", as_listen_addr_, sizeof(as_listen_addr_), SRV_AUTHENSERVER_IP);
	ReadInteger(srv, "listenport", as_listen_port_, SRV_AUTHENSERVER_PORT);

	//DirServer
	srv = element->FirstChildElement("dirserver");
	if (!srv)
	{
		LOG_ERR("failed to parse dirserver!");
		return false;
	}
	ReadString(srv, "listenip", ds_listen_addr_, sizeof(ds_listen_addr_), SRV_DIRSERVER_IP);
	ReadInteger(srv, "listenport", ds_listen_port_, SRV_DIRSERVER_PORT);

	return true;
}
