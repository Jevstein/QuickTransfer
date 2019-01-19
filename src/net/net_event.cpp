#include "prefixhead.h"
#include "net_event.h"
#include "connection.h"
#include "net_module.h"

bool CNetPacket::set_data(const char* data, int len)
{
	length_ = len;
	if (len < 0 || !data)
		return true;

	length_ = len > CConnection::MAX_OVERLAP_BUFFER ? CConnection::MAX_OVERLAP_BUFFER : len;
	data_ = static_cast<char*>(get_module()->get_memory()->Malloc(length_)); 
	if (!data_)
	{
		LOG_EXP("failed to malloc! len=%d", len);
		return false;
	}
	memcpy(data_, data, length_);

	return true;
}

void CNetPacket::clear()
{
	if (data_)
		get_module()->get_memory()->Free(data_);

	data_ = NULL;
	length_ = 0;
	offset_ = 0;
}

CTCPEvent::CTCPEvent()
: delay_(0)
, connection_(NULL)
{
}

CTCPEvent::~CTCPEvent(void)
{
	delay_ = 0;
	connection_ = NULL;
	packet_.clear();
}

void CTCPEvent::set(CConnection* connection, const char* data, int len)
{
	CLockGuard<CLocker> lock(locker_);
	
	connection_ = connection;
	delay_ = 0;

	if (!packet_.is_empty())
		packet_.clear();

	packet_.set_data(data, len);
}

void CTCPEvent::process()
{
	if (NULL == connection_)
	{
    	LOG_WAR("invalid connection!");
		return;
	}
    
	const int process_id = packet_.length();
    LOG_DBG("CTCPEvent::process=%d", process_id);

	if(process_id > 0)
	{
		CLockGuard<CLocker> lock(locker_);
		ISession* session = connection_->get_session();
		if (session)
			session->on_recv(&packet_);
	}
	else
	{
		switch (process_id)
		{
		case CConnection::ON_CONNECT: 			connection_->on_connect(); 			break;
		case CConnection::ON_CONNECTION: 		connection_->on_connection(); 		break;
		case CConnection::ON_DISCONNECT: 		connection_->on_disconnect(); 		break;
		case CConnection::ON_CLOSECONNECTION: 	connection_->on_closeconnection(); 	break;
		case CConnection::ON_DISCONNECTION: {
				connection_->on_disconnection(); 
				connection_ = NULL;
			}break;
		default:
			LOG_ERR("invalid process id: %d", process_id);
			break;
		}
	}
}

void CTCPEvent::release()
{
	CLockGuard<CLocker> lock(locker_);
	
	connection_ = NULL;
	delay_ = 0;
	packet_.clear();
    
	get_module()->get_pool()->push_tcpevent(this);
}