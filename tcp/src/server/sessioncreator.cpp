#include "prefixhead.h"
#include "sessioncreator.h"
#include "delegate.h"
#include "session.h"

CSessionCreator::CSessionCreator()
{
}

CSessionCreator::~CSessionCreator()
{
}

bool CSessionCreator::on_preaccept(struct sockaddr* remoteaddr)
{
    LOG_DBG("prepare to accept TCP[%s.%d]", inet_ntoa(((sockaddr_in*)remoteaddr)->sin_addr), ntohs(((sockaddr_in*)remoteaddr)->sin_port));

	return true;
}

ISession* CSessionCreator::on_create()
{
    return new CSession();
}

void CSessionCreator::destroy(ISession* session)
{
    SAFE_DELETE(session);
}
