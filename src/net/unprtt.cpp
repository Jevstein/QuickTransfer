#include "prefixhead.h"
#include "unprtt.h"

int rtt_d_flag = 0;		// debug flag,can be set by caller 

/**
	Calculate the RTO value based on currect estimators;
	smoothed RTT plus four times the deviation
**/

#define RTT_RTOCALC(ptr)	((ptr)->rtt_srtt * (4.0f * (ptr)->rtt_rttvar))

float rtt_minmax(float rto)
{
	if(rto < RTT_RXTMIN)
		rto = RTT_RXTMIN;
	rto = rto < RTT_RXTMIN ? RTT_RXTMIN : rto;
	rto = rto > RTT_RXTMAX ? RTT_RXTMAX : rto;
	return rto;
}

void rtt_init(struct rtt_info* ptr)
{
	struct timeval tv;
	p_gettimeofday(&tv, NULL);
	ptr->rtt_base = tv.tv_sec;	//sec since 1/1/1970 at start
	ptr->rtt_rtt = 0;
	ptr->rtt_srtt = 0;
	ptr->rtt_rttvar = 0.75;
	ptr->rtt_rto = rtt_minmax(RTT_RTOCALC(ptr));	//first RTO at (srtt + (4.0 * rttvar)) = 3 seconds
}

uint rtt_ts(struct rtt_info* ptr)
{
	uint ts;
	struct timeval tv;
	p_gettimeofday(&tv, NULL);
	ts = ((tv.tv_sec - ptr->rtt_base) * 1000) + (tv.tv_usec / 1000);
	return ts;
}

void rtt_newpack(struct rtt_info* ptr)
{
	ptr->rtt_nrexmt = 0;
}

int rtt_start(struct rtt_info* ptr)
{
	return ((int)(ptr->rtt_rto + 0.5f)); //round float to int
}

void rtt_stop(struct rtt_info* ptr, uint ms)
{
	double delta;
	ptr->rtt_rtt = ms / 1000.0f;
	delta = ptr->rtt_rtt - ptr->rtt_srtt;
	ptr->rtt_srtt += delta / 8; // g = 1 / 8;
	if(delta < 0.0f)
		delta = -delta;
	ptr->rtt_rttvar += (delta - ptr->rtt_rttvar) / 4; // h = 1 / 4
	ptr->rtt_rto = rtt_minmax(RTT_RTOCALC(ptr));
}

int rtt_timeout(struct rtt_info* ptr)
{
	ptr->rtt_rto *= 2;	// next RTO
	if(++ptr->rtt_nrexmt > RTT_MAXNREXMT)
		return -1;		// time to give up for this packet
	return 0;
}

