#include "prefixhead.h"
#include "session.h"
#include "delegate.h"

CSession::CSession()
{
}

CSession::~CSession()
{
}

void CSession::release()
{
}

void CSession::on_connection(IConnection* connection)
{
}

void CSession::on_disconnect()
{
}

void CSession::on_disconnection()
{
}

void CSession::on_recv(const char* data, int size)
{
}
