#include "prefixhead.h"
#include "reactor.h"
#include "net_module.h"
#include "net_event.h"

CReactor::CReactor()
: io_model_(NULL)
{

}

CReactor::~CReactor()
{
    misfire();
}

bool CReactor::fire()
{
	io_model_ = CIOModel::create_model(0);
	if (!io_model_)
    {
        LOG_ERR("failed to create model!");
        return false;
    }

    if(-1 == io_model_->init())
    {
        LOG_ERR("failed to init io model!");
        return false;
    }
    
	CThread::start();
    
	return true;
}

void CReactor::misfire()
{
	CIOModel::destry_model(io_model_);
	io_model_ = NULL;
	is_started_ = false;
	CThread::stop();
}

void CReactor::run()
{
	LOG_DBG("CReactor::thread[%p] run ...", this);
    
	int idxs = -1;
	while(is_started_)
    {
		idxs = io_model_->wait();
		if(idxs < 0)
		{
			if(errno == EINTR)
				continue;

			LOG_ERR("failed to watch i/o! err: %s", p_socket_last_error());
			break;
		}

        do_event_queue();

		for(int i = 0; i < idxs; ++i)
		{
			INetSocket* net_socket = (INetSocket *)io_model_->get_key(i);
			if(net_socket == NULL)
            {
                LOG_DBG("invalid net_socket!");
				continue;
			}
            
            LOG_DBG("net_socket[%p]: idx=%d/%d, type=%d", net_socket, i, idxs, net_socket->get_socket_type());
            
			if(net_socket->get_socket_type() == SO_LISTENER)
			{//listen
				net_socket->on_accept();
			}
			else
			{//socket
				int events = io_model_->get_event(i);
				if (events & EPOLLERR || events & EPOLLHUP)
                {
                    LOG_ERR("socket events error, events=%d, err: %s", events, p_socket_last_error());
					//if(errno == 0)
					//	net_socket->OnDisConnect();
					// net_socket->OnSend();
                    net_socket->on_error();
                    continue;
				}

				if(events & EPOLLIN)
				{
					net_socket->on_recv();
				}

				if(events & EPOLLOUT)
				{
					net_socket->on_send();
				}
			}
		}
	}

	LOG_DBG("leave CReactor::Run()");
}

void CReactor::do_event_queue()
{
    // queue of event
    CTCPEvent* ev = (CTCPEvent *)que_of_event_.pop();
    while(ev)
    {
        LOG_DBG("CReactor::ev->process()");
        ev->process();
        
        // get_module()->main_event_queue()->push(ev);
        ev = (CTCPEvent *)que_of_event_.pop();
    }
    
    // queue of delay event
    ev = (CTCPEvent*)que_of_delay_event_.front();
    while(ev)
    {
        const int elapse = ev->get_delay() - helper::CUtility::GetCurTime();
        if(elapse > 0 && elapse <= 1000 * 10)
            break;
        
        LOG_DBG("CReactor::delay::ev->process()");
        ev->process();
        
        // get_module()->main_event_queue()->push(ev);
        que_of_delay_event_.pop();
        ev = (CTCPEvent*)que_of_delay_event_.front();
    }
}

bool CReactor::add_socket(YI_SOCKET sockfd, int events, void* key)
{
    return io_model_->add_fd(sockfd, events, key);
}

bool CReactor::del_socket(YI_SOCKET sockfd)
{
    return io_model_->del_fd(sockfd);
}

bool CReactor::modify_socket(YI_SOCKET sockfd, int events, void* key)
{
    return io_model_->modify_fd(sockfd, events, key); 
}