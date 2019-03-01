#include "prefixhead.h"
#include "session.h"
#include "parser.h"

char file_data_test[] = {"6c7166407562756e74753a2f6d6e742f686766732f776f726b24206364206c69626576656e742f6578616d706c652fa6c7\n	\
	166407562756e74753a2f6d6e742f686766732f776f726b2f6c69626576656e742f6578616d706c6524206c73a312d636c69656e7420202020\n	\
	322d636c69656e7420202020332d636c69656e74202020206331306b5f7365727665722e632020202020202020202020202020202020202020\n	\
	20207468726561645f706f6c6c5f636c69656e742e63202074696d65722e63a312d636c69656e742e632020322d636c69656e742e632020332\n	\
	d636c69656e742e6320206c69626576656e742d74687265616420202020202020202020202020202020202020207468726561645f706f6c6c5\n	\
	f736572766572a312d73657276657220202020322d73657276657220202020332d736572766572202020206c69626576656e742d7468726561\n	\
	642d32303134303232342d312e7461722e677a20207468726561645f706f6c6c5f7365727665722e63a312d7365727665722e632020322d736\n	\
	5727665722e632020332d7365727665722e6320207468726561645f706f6c6c5f636c69656e742020202020202020202020202020202020746\n	\
	96d6572a6c7166407562756e74753a2f6d6e742f686766732f776f726b2f6c69626576656e742f6578616d706c652420676363202d6f206331\n	\
	306b5f736572766572206331306b5f7365727665722e6320a6c7166407562756e74753a2f6d6e742f686766732f776f726b2f6c69626576656\n	\
	e742f6578616d706c6524206c73a312d636c69656e7420202020322d636c69656e7420202020332d636c69656e74202020206331306b5f7365\n	\
	727665722020202020202020202020202020202020202020202020207468726561645f706f6c6c5f636c69656e742020202074696d6572a312\n	\
	d636c69656e742e632020322d636c69656e742e632020332d636c69656e742e6320206331306b5f7365727665722e632020202020202020202\n	\
	02020202020202020202020207468726561645f706f6c6c5f636c69656e742e63202074696d65722e63a312d73657276657220202020322d73\n	\
	657276657220202020332d736572766572202020206c69626576656e742d746872656164202020202020202020202020202020202020202074\n	\
	68726561645f706f6c6c5f736572766572a312d7365727665722e632020322d7365727665722e632020332d7365727665722e6320206c69626\n	\
	576656e742d7468726561642d32303134303232342d312e7461722e677a20207468726561645f706f6c6c5f7365727665722e63a6c71664075\n	\
	62756e74753a2f6d6e742f686766732f776f726b2f6c69626576656e742f6578616d706c6524202e2f6331306b5f736572766572a65706f6c6\n	\
	c73657276657220737461727475702c706f"};

int _send_data(jvt_session_t *S, void *buf, int size)
{
	int len;
	void *data = jvt_parser_encode(S, buf, size, &len);

	return udp_socket_send(&S->udp_socket_, data, len);
}

void jvt_session_on_recv_data(void* user_data, udp_socket_result_t* result)
{
	assert(result);
	jvt_session_t* session = (jvt_session_t *)user_data;

	if (result->type == RECV_TYPE_SUCCESS)
	{
		if (0 != jvt_parser_decode(session, (void *)result->data, result->len))
		{
			LOG_ERR("failed to decode the message: data=%p, len=%d", result->data, result->len);
		}
	}
	else if (result->type == RECV_TYPE_INCOMPLETE)
	{
        // LOG_INF("recv: type=%d", result->type, result->data, result->len);
	}
	else
	{
        LOG_ERR("recv: error=%d, data=%p, len=%d", result->type, result->data, result->len);
	}
}

int jvt_session_init(jvt_session_t *session)
{
	assert(session);

	return 0;
}

void jvt_session_uninit(jvt_session_t *S)
{
	LOG_ERR("TODO: jvt_session_uninit...");
}

void jvt_session_recv_downloadfile_req(jvt_session_t *S, pt_downloadfile_req *req)
{
    LOG_INF("recv[%s:%d][downloadfile_req]:: filename='%s'", S->udp_socket_.ip, S->udp_socket_.port, req->filename);

	pt_downloadfile_ack ack;
	ack.opcode = downloadfile_ack;
	ack.ret = 0;
	ack.fileid = 100;
	ack.filesize = 1000;
	strcpy(ack.filename, req->filename);
	_send_data(S, (void *)&ack, sizeof(ack));

    LOG_INF("send[%s:%d][downloadfile_ack]:: ret=%d, fileid=%d, filesize=%d, filename='%s'"
		, S->udp_socket_.ip, S->udp_socket_.port
		, ack.ret, ack.fileid, ack.filesize, ack.filename);

	pt_transferfile_noti noti;
	noti.opcode = transferfile_noti;
	noti.fileid = ack.fileid;
	noti.index = 0;
	noti.size = sizeof(file_data_test);
	strcpy(noti.data, file_data_test);
	_send_data(S, (void *)&noti, sizeof(noti));

    LOG_INF("send[%s:%d][transferfile_noti]:: fileid=%d, index=%d, size=%d, data='%s'"
		, S->udp_socket_.ip, S->udp_socket_.port
		, noti.fileid, noti.index, noti.size, noti.data);
}

void jvt_session_recv_transferfile_ack(jvt_session_t *S, pt_transferfile_ack *ack)
{
    LOG_INF("recv[%s:%d][transferfile_ack]:: ret=%d, fileid=%d, index=%d"
		, S->udp_socket_.ip, S->udp_socket_.port
		, ack->ret, ack->fileid, ack->index);
}