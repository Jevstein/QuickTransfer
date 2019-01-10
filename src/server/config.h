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
	std::string ip;
	short port;
};

class CConfig
{
public:
	CConfig();
	virtual ~CConfig();

	void cleanup();

public:/*initialize*/

public:/*set or get*/
	const cfg_info_t* get_cfg_info() const { return &cfg_info_; }

private:/** common data*/
	cfg_info_t cfg_info_;
};

#endif //_CONFIG_H_