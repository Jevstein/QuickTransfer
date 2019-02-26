#ifndef _UNPRTT_H_
#define _UNPRTT_H_

struct rtt_info
{
	float rtt_rtt;		// most recent measured RTT, in seconds
	float rtt_srtt;		// smoothed RTT estimator, in seconds
	float rtt_rttvar;	// smoothed mean deviation, in seconds
	float rtt_rto;		// current RTO to use, in seconds
	int rtt_nrexmt;		// time restransmitted: 0, 1, 2, ....
	uint rtt_base;		// sec since 1/1/1970 at start
};

#define RTT_RXTMIN		2	// min restransmit timeout value, in seconds
#define RTT_RXTMAX		60	// max restransmit timeout value, in seconds
#define RTT_MAXNREXMT	3	// max times to restransmit

void rtt_debug(struct rtt_info*);
void rtt_init(struct rtt_info*);
void rtt_newpack(struct rtt_info*);
int rtt_start(struct rtt_info*);
void rtt_stop(struct rtt_info*, uint);
int rtt_timeout(struct rtt_info*);
uint rtt_ts(struct rtt_info*);

extern int rtt_d_flag;

struct sock4req1 
{ 
	char VN; 
	char CD; 
	unsigned short Port; 
	unsigned long IPAddr; 
	char other[1]; 
}; 

struct sock4ans1 
{ 
	char VN; 
	char CD; 
}; 

struct sock5req1 
{ 
	char Ver; 
	char nMethods; 
	char Methods[255]; 
}; 

struct sock5ans1 
{ 
	char Ver; 
	char Method; 
}; 

struct sock5req2 
{ 
	char Ver; 
	char Cmd; 
	char Rsv; 
	char Atyp; 
	unsigned long IPAddr;
	unsigned short Port;
}; 

struct sock5ans2 
{ 
	char Ver; 
	char Rep; 
	char Rsv; 
	char Atyp; 
	char other[1]; 
}; 

struct authreq 
{ 
	char Ver; 
	char Ulen; 
	char Name[255]; 
	char PLen; 
	char Pass[255]; 
}; 

struct authans 
{ 
	char Ver; 
	char Status; 
};

#endif//_UNPRTT_H_
