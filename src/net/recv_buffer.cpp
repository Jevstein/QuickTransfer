#include "prefixhead.h"
#include "recv_buffer.h"

CRecvBuffer::CRecvBuffer(uint capacity)
: capacity_(capacity)
, buffer_(new char[capacity])
{
    reset();
}

CRecvBuffer::~CRecvBuffer()
{
    if (buffer_)
    {
        delete []buffer_;
        capacity_ = 0;
        reset();
    }
}

void CRecvBuffer::reset()
{
	size_ = 0;
	begin_pos_ = 0;
	end_pos_ = 0;
}