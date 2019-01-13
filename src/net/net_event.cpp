#include "prefixhead.h"
#include "net_event.h"
#include "connection.h"
#include "net_module.h"

CTCPEvent::CTCPEvent()
: packet_buffer_(NULL)
, packet_len_(0)
, offset_(0)
, delay_(0)
, connection_(NULL)
{
}

CTCPEvent::~CTCPEvent(void)
{
	packet_len_ = 0;
	offset_ = 0;
	delay_ = 0;
	connection_ = NULL;
	if (packet_buffer_)
	{
		get_module()->get_memory()->Free(packet_buffer_);
		packet_buffer_ = NULL;
	}
}

void CTCPEvent::set(CConnection* connection, const char* packet, int packet_len)
{
	{
		CLockGuard<CLocker> lock(locker_);
		connection_ = connection;
		delay_ = 0;
		packet_len_ = packet_len;
		if(packet_len < 0 ||  NULL == packet)
			return;
	}

	char* buffer = NULL;
	{
		CLockGuard<CLocker> lock(locker_);
		buffer = packet_buffer_;
	}
	
	if (buffer)
	{
		get_module()->get_memory()->Free(buffer);
		CLockGuard<CLocker> lock(locker_);
		packet_buffer_ = NULL;
	}

	buffer = static_cast<char*>(get_module()->get_memory()->Malloc(packet_len_)); // new char[packet_len_];

	CLockGuard<CLocker> lock(locker_);
	packet_len_ = packet_len > CConnection::MAX_OVERLAP_BUFFER ? CConnection::MAX_OVERLAP_BUFFER : packet_len;
	packet_buffer_ = buffer;
	assert(packet_buffer_);
	memcpy(packet_buffer_, packet, packet_len_);
}

void CTCPEvent::process()
{
// 	CLockGuard<CLocker> lock(locker_);
	if (NULL == connection_)
		return;
    
    LOG_DBG("CTCPEvent::process=%d", packet_len_);

	if(packet_len_ > 0)
	{
		ISession* session = connection_->get_session();
		if(session)
			session->on_recv(packet_buffer_, packet_len_);
	}
	else
	{
		switch (packet_len_)
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
			LOG_ERR("invalid packet_len_: %d", packet_len_);
			break;
		}
	}
}

void CTCPEvent::release()
{
	char* packet_buffer = NULL;
	{
		CLockGuard<CLocker> lock(locker_);
		packet_buffer = packet_buffer_;
	}
    
	if (packet_buffer)
		get_module()->get_memory()->Free(packet_buffer);

	{
		CLockGuard<CLocker> lock(locker_);
		packet_len_ = 0;
		offset_ = 0;
		delay_ = 0;
		connection_ = NULL;
		packet_buffer_ = NULL;
	}
    
	get_module()->get_pool()->push_tcpevent(this);
}

char* CTCPEvent::get_data()
{
	if (NULL == packet_buffer_) 
		return NULL;
	
	return packet_buffer_ + offset_;
}

void CTCPEvent::transfer_data(int transfer_num)
{
	offset_ += transfer_num;
	assert(offset_ <= packet_len_);
}
