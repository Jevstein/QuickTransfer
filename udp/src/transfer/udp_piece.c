#include <stdlib.h>
#include <string.h>
#include "inner.h"
#include "udp_piece.h"

// #define UDP_ERR		printf
// #define UDP_DEBUG	printf

/**
 * @brief åå§åèµæº?
 * @param buf_size è®¾ç½®ç¼å²åºæ°æ®çæå¤§é¿åº?
 * @return æååè¿åä¸ä¸ªå¥æï¼å¤±è´¥åè¿åNULL
 */
udp_piece_t* udp_piece_init( int buf_size )
{
	udp_piece_t *udp_piece = (udp_piece_t *) calloc(1, sizeof(udp_piece_t) );
	assert(udp_piece);

	udp_piece->circular_buffer = circular_buffer_init( buf_size );
	if ( !udp_piece->circular_buffer )
		return(NULL);

	return(udp_piece);
}

/**
 * @brief éæ¾èµæº
 * @param udp_piece å¥æ
 */
void udp_piece_deinit( udp_piece_t *udp_piece )
{
	/* éæ¾èµæº */
	if ( udp_piece )
	{
		if ( udp_piece->recv_buf )
			free( udp_piece->recv_buf );
		udp_piece->recv_buf	= NULL;
		udp_piece->recv_pieces	= 0;
		udp_piece->total_size	= 0;    /* æ»æ°æ®å¤§å°?*/
		udp_piece->total_pieces = 0;    /* åçæ»æ°é?*/
		udp_piece->left			= 0;    /* æåä¸ççå¤§å° */
		udp_piece->piece_size	= 0;    /* åçå¤§å° */
		udp_piece->recv_len	= 0;
		circular_buffer_deinit( udp_piece->circular_buffer );
		udp_piece->circular_buffer = NULL;
	}
}

/**
 * @brief éç½®ï¼è¿éä¸ä¼éæ°åéèµæºï¼åªæ¯è®²é¨ååæ°éç½®å°åå§åç¶æ?
 * @param udp_piece å¥æ
 */
void udp_piece_reset( udp_piece_t *udp_piece )
{
	/* æ°æ®éç½® */
	if ( udp_piece )
	{
		if ( udp_piece->recv_buf )
			free( udp_piece->recv_buf );
		udp_piece->recv_buf	= NULL;
		udp_piece->send_ptr = NULL;
		udp_piece->recv_pieces = 0;
		udp_piece->total_size = 0;    	/* æ»æ°æ®å¤§å°?*/
		udp_piece->total_pieces = 0;    /* åçæ»æ°é?*/
		udp_piece->left = 0;    		/* æåä¸ççå¤§å° */
		udp_piece->piece_size = 0;    	/* åçå¤§å° */
		udp_piece->recv_len	= 0;
		circular_buffer_reset( udp_piece->circular_buffer );
	}
}

/**
 * @brief æ ¹æ®é¿åº¦è¿è¡åå²ï¼è¿ååå²åçåçæ°é?
 * @param udp_piece å¥æ
 * @param buf       è¦åçæ°æ®çæé
 * @param size      è¦åçæ°æ®çé¿åº¦
 * @return è¿ååççæ°é?
 */
int udp_piece_cut( udp_piece_t *udp_piece, const void *buf, int size )
{
	if (!udp_piece || size < 0)
		return 0;

	udp_piece->send_ptr = (uint8_t*)buf;
	udp_piece->total_size = size;
	udp_piece->left = size % PIECE_FIX_SIZE;	// æåä¸ä¸ªåçæ°æ®çå¤§å°
	udp_piece->total_pieces = 
		(udp_piece->left > 0) ? (size / PIECE_FIX_SIZE + 1) : (size / PIECE_FIX_SIZE);

	return udp_piece->total_pieces;
}

/**
 * @brief æ ¹æ®åçç¼å·è·ååçæéååçæ°æ®å¤§å°?
 * @param udp_piece å¥æ
 * @param index     åçç¼å·
 * @param got_piece_size è·åæå®ç¼å·åçæ°æ®çé¿åº?
 * @return è¿åæå®åçç¼å·çæ°æ®æé?
 */
uint8_t *udp_piece_get( udp_piece_t *udp_piece, int index, int *got_piece_size )
{
	int piece_size = 0;

	if (!udp_piece || !got_piece_size)
		return NULL;

	*got_piece_size = 0;

	// åçå¤§å°
	if(((udp_piece->total_pieces - 1) == index)		// æ¯ä¸æ¯æåä¸ä¸ªåç?
		&& (udp_piece->left > 0))
	{
		piece_size = udp_piece->left;
	}
	else
	{	
		piece_size = PIECE_FIX_SIZE;
	}

	// åå§åä¸º0
	memset( udp_piece->piece_buf, 0, sizeof(udp_piece->piece_buf) );
	// å¡«ååçå¸§å¤´
	// åæ­¥å­?
	udp_piece->piece_buf[HEAD_POS_SYNC_WORD] = 0xAF;
	udp_piece->piece_buf[HEAD_POS_SYNC_WORD + 1] = 0xAE;
	// ææåçæ°æ®çå¤§å°
	udp_piece->piece_buf[HEAD_POS_TOTAL_SIZE] = udp_piece->total_size >> 8;
	udp_piece->piece_buf[HEAD_POS_TOTAL_SIZE + 1] = (udp_piece->total_size & 0xff);
	// ææåççæ°é
	udp_piece->piece_buf[HEAD_POS_TOTAL_PIECES] = udp_piece->total_pieces >> 8;
	udp_piece->piece_buf[HEAD_POS_TOTAL_PIECES + 1] = (udp_piece->total_pieces & 0xff);
	// åçç¼å·ï¼ä»0å¼å§?
	udp_piece->piece_buf[HEAD_POS_P_INDEX] = index >> 8;
	udp_piece->piece_buf[HEAD_POS_P_INDEX + 1] = (index & 0xff);
	// åçæ°æ®çå¤§å°?
	udp_piece->piece_buf[HEAD_POS_P_LENGTH] = piece_size >> 8;
	udp_piece->piece_buf[HEAD_POS_P_LENGTH + 1] = (piece_size & 0xff);

	// æç¨æ·æ°æ®æ·è´å°åçæ°æ®å?
	memcpy(&udp_piece->piece_buf[HEAD_SIZE], &udp_piece->send_ptr[PIECE_FIX_SIZE * index], piece_size);
	*got_piece_size = piece_size + HEAD_SIZE;
	
	return udp_piece->piece_buf;
}

/**
 * @brief éç»åç
 * @param udp_piece å¥æ
 * @param buf   åçæ°æ®çæé?
 * @param size  åçæ°æ®çé¿åº?
 * @return  è¿å-1åéç»å¤±è´¥ï¼è¿å0åæ­£å¨éç»ä¸­ï¼è¿å?åéç»æå?
 */
int udp_piece_merge( udp_piece_t *udp_piece, void *buf, int size )
{
	uint8_t *piece_buf = NULL;
	int temp_size = 0;
	int temp_total_size = 0;
	int temp_total_pieces = 0;
	int p_index = 0;
	int get_all_pieces = 0;
	
	// æ£æµå¤´é¨æ¯å¦æåæ­¥å­?
	piece_buf = (uint8_t*)buf;
	temp_size = size;
	for(int i = 0; i < size; i++)
	{
		if((piece_buf[0] == 0xAF) && (piece_buf[1] == 0xAE))
			break;
		piece_buf++;
		temp_size--;
	}

	// å¦ææ£æµå°åæ­¥å­ï¼ä¸å©ä½æ°æ®é¿åº¦è¿è¶è¿ åçå¸§å¤´é¿åº¦
	while (temp_size > HEAD_SIZE)
	{
		// å½ååççæ°æ®é¿åº?
		int data_len = (piece_buf[HEAD_POS_P_LENGTH] << 8) + (piece_buf[HEAD_POS_P_LENGTH+1]);
		if(temp_size >= (HEAD_SIZE + data_len))
		{
			// è·ååçç¼å·
			p_index = (piece_buf[HEAD_POS_P_INDEX] << 8) + (piece_buf[HEAD_POS_P_INDEX+1]);
			if(udp_piece->total_size == 0)		// éç½®åç¬¬ä¸æ¬¡æ¶å°æ°æ?
			{
				// è·ååçæ°æ®çæ»å¤§å°?
				udp_piece->total_size = (piece_buf[HEAD_POS_TOTAL_SIZE] << 8) + (piece_buf[HEAD_POS_TOTAL_SIZE+1]);
				// è·åæ»çåçæ°é
				udp_piece->total_pieces = (piece_buf[HEAD_POS_TOTAL_PIECES] << 8) + (piece_buf[HEAD_POS_TOTAL_PIECES+1]);
				udp_piece->recv_len = 0;
				udp_piece->recv_pieces  = 0;
				if(udp_piece->recv_buf)
					free(udp_piece->recv_buf);
				udp_piece->recv_buf = (uint8_t*)malloc(udp_piece->total_size + 1);
				if ( !udp_piece->recv_buf )
				{
					UDP_ERR( "malloc recv_buf filed\n");
					return(-1);
				}
			}
			UDP_DEBUG("merge piece[%d]: buf size=%d, piece_data_len=%d, recv_pieces=%d, total_size=%d, total_pieces=%d",
				   p_index, temp_size, data_len, udp_piece->recv_pieces, udp_piece->total_size, udp_piece->total_pieces );
			temp_total_size = (piece_buf[HEAD_POS_TOTAL_SIZE] << 8) + (piece_buf[HEAD_POS_TOTAL_SIZE+1]);
			temp_total_pieces = (piece_buf[HEAD_POS_TOTAL_PIECES] << 8) + (piece_buf[HEAD_POS_TOTAL_PIECES+1]);
			udp_piece->recv_pieces++;
			// åæä¸æ°çåçæ¯å¦ååæ¥çåçç»æåºå?
			if((temp_total_size != udp_piece->total_size) || (temp_total_pieces != udp_piece->total_pieces))
			{
				udp_piece->total_size = temp_total_size;
				udp_piece->total_pieces = temp_total_pieces;
				udp_piece->recv_len = 0;
				udp_piece->recv_pieces = 1;
				if(udp_piece->recv_buf)
					free(udp_piece->recv_buf);
				udp_piece->recv_buf = (uint8_t*)malloc(udp_piece->total_size + 1);
				if ( !udp_piece->recv_buf )
				{
					UDP_ERR( "malloc recv_buf filed\n");
					return(-1);
				}
			}
			piece_buf += HEAD_SIZE;
			temp_size -= HEAD_SIZE;
			memcpy(&udp_piece->recv_buf[PIECE_FIX_SIZE * p_index], piece_buf, data_len);
			piece_buf += data_len;
			temp_size -= data_len;

			udp_piece->recv_len += data_len;		// åçæ°æ®çç´¯å?
			if(udp_piece->recv_pieces == udp_piece->total_pieces)
			{
				udp_piece->total_pieces = 0;
				udp_piece->recv_pieces = 0;
				if(udp_piece->recv_len == udp_piece->total_size)
				{
					get_all_pieces = 1;		// æææåçç»æå®æ´çæ°æ®å¸?
				}
				else
				{
					UDP_ERR( "recv_len != total_size! recv_len: %d, total_size: %d", udp_piece->recv_len, udp_piece->total_size );
					get_all_pieces = -1;
				}
			}
		}
		else
		{
			temp_size = 0;
		}
	}

	return get_all_pieces;
}

/**
 * @brief éç»åçï¼ä½¿ç¨ç¯å½¢ç¼å­?
 * @param udp_piece å¥æ
 * @param buf   åçæ°æ®çæé?
 * @param size  åçæ°æ®çé¿åº?
 * @return  è¿å-1åéç»å¤±è´¥ï¼è¿å0åæ­£å¨éç»ä¸­ï¼è¿å?åéç»æå?
 */
int udp_piece_merge_ex( udp_piece_t *udp_piece, void *buf, int size )
{
	int	tmp_total_size		= 0;
	int	tmp_total_pieces	= 0;
	int	p_index	= 0;
	int	get_all_pieces = 0;
	int	bytes_to_write = 0;
	int	bytes_to_read  = 0;
	uint8_t		value0 = 0;
	uint8_t		value1 = 0;

	bytes_to_write = circular_buffer_write( udp_piece->circular_buffer, (const uint8_t*)buf, size );
	if ( bytes_to_write != size )
	{
		UDP_ERR( "%s(%d) There is not enough space, only %d bytes, but need %d bytes\n",
			 __FUNCTION__, __LINE__, bytes_to_write, size );
		return(-1);
	}

	/*
	 * ä»æ°æ¶å°çæ°æ®æ£æµæ¯å¦åå«çå¤?
	 * æ£æµå¤´é?
	 */
	while ( circular_buffer_size( udp_piece->circular_buffer ) >= 2 )
	{
		circular_buffer_get( udp_piece->circular_buffer, 0, &value0 );          /* éè¿ç´¢å¼è·åå½åå?*/
		circular_buffer_get( udp_piece->circular_buffer, 1, &value1 );
		if ( value0 == 0xAF && value1 == 0xAE )
			break;

		circular_buffer_pop_front( udp_piece->circular_buffer, 1);        /* åºéåä¸ä¸ªåç´?*/
	}

	/* å¦æå©ä½çæ°æ®é¿åº¦ä»å¤§äºå¸§å¤´ */
	while ( circular_buffer_size( udp_piece->circular_buffer ) > HEAD_SIZE )
	{
		/* å½ååççæ°æ®é¿åº¦ï¼ä¸å«å¸§å¤´ï¼?*/
		circular_buffer_get( udp_piece->circular_buffer, HEAD_POS_P_LENGTH, &value0 ); /* éè¿ç´¢å¼è·åå½åå?*/
		circular_buffer_get( udp_piece->circular_buffer, HEAD_POS_P_LENGTH + 1, &value1 );

		uint32_t data_len = (value0 << 8) + value1;

		/* UDP_DEBUG("%s(%d)\n", __FUNCTION__, __LINE__); */
		if ( circular_buffer_size( udp_piece->circular_buffer ) >= (HEAD_SIZE + data_len) )
		{
			circular_buffer_get( udp_piece->circular_buffer, HEAD_POS_P_INDEX, &value0 );               /* éè¿ç´¢å¼è·åå½åå?*/
			circular_buffer_get( udp_piece->circular_buffer, HEAD_POS_P_INDEX + 1, &value1 );
			p_index = (value0 << 8) + value1;
			if ( udp_piece->total_size == 0 )                                                           /* éç½®åç¬¬ä¸æ¬¡æ¶å°åç?*/
			{
				/* è®¡ç®å½ååçæå±åçç»æ°æ®çæ»å¤§å°?*/
				circular_buffer_get( udp_piece->circular_buffer, HEAD_POS_TOTAL_SIZE, &value0 );        /* éè¿ç´¢å¼è·åå½åå?*/
				circular_buffer_get( udp_piece->circular_buffer, HEAD_POS_TOTAL_SIZE + 1, &value1 );
				udp_piece->total_size = (value0 << 8) + value1;

				/* è®¡ç®å½ååçæå±åçç»çæ»åçæ°é?*/
				circular_buffer_get( udp_piece->circular_buffer, HEAD_POS_TOTAL_PIECES, &value0 );      /* éè¿ç´¢å¼è·åå½åå?*/
				circular_buffer_get( udp_piece->circular_buffer, HEAD_POS_TOTAL_PIECES + 1, &value1 );
				udp_piece->total_pieces = (value0 << 8) + value1;

				/* éç½®å½åæ¥æ¶åççæ°é?*/
				udp_piece->recv_pieces = 0;
				/* éç½®å½åæ¥æ¶å°çæ°æ®é¿åº¦ */
				udp_piece->recv_len = 0;
				/* å¦æç¼å­åºææ°æ®ååéæ¾ */
				if ( udp_piece->recv_buf )
					free( udp_piece->recv_buf );
				/* åéè½å¤å­å¨ä¸ä¸ªåçç»ææçæ°æ®çç©ºé?*/
				udp_piece->recv_buf = (uint8_t*)malloc( udp_piece->total_size + 1 );
				if ( !udp_piece->recv_buf )
				{
					return(-1);
				}
			}
			UDP_DEBUG("merge piece[%d]: buf size=%d, piece_data_len=%d, recv_pieces=%d, total_size=%d, total_pieces=%d",
				   p_index, circular_buffer_size(udp_piece->circular_buffer), data_len, udp_piece->recv_pieces, udp_piece->total_size, udp_piece->total_pieces);

			/* è®¡ç®å½ååçæå±åçç»æ°æ®çæ»å¤§å°?*/
			circular_buffer_get( udp_piece->circular_buffer, HEAD_POS_TOTAL_SIZE, &value0 );        /* éè¿ç´¢å¼è·åå½åå?*/
			circular_buffer_get( udp_piece->circular_buffer, HEAD_POS_TOTAL_SIZE + 1, &value1 );
			tmp_total_size = (value0 << 8) + value1;

			/* è®¡ç®å½ååçæå±åçç»çæ»åçæ°é?*/
			circular_buffer_get( udp_piece->circular_buffer, HEAD_POS_TOTAL_PIECES, &value0 );      /* éè¿ç´¢å¼è·åå½åå?*/
			circular_buffer_get( udp_piece->circular_buffer, HEAD_POS_TOTAL_PIECES + 1, &value1 );
			tmp_total_pieces = (value0 << 8) + value1;
			udp_piece->recv_pieces++;
			if ( (tmp_total_pieces != udp_piece->total_pieces) || (tmp_total_size != udp_piece->total_size) )
			{
				UDP_DEBUG( "Discard current frame, total_pieces:%d, cur_total_pieces:%d, recv_pieces:%d, total_size:%d, cur_total_size:%d\n",
					   udp_piece->total_pieces, tmp_total_pieces, udp_piece->recv_pieces, udp_piece->total_size, tmp_total_size );
				/* one piece of a new frame is coming, reset the variables */
				udp_piece->total_size	= tmp_total_size;
				udp_piece->total_pieces = tmp_total_pieces;
				udp_piece->recv_pieces	= 1;
				udp_piece->recv_len	= 0;
				if ( udp_piece->recv_buf )
					free( udp_piece->recv_buf );
				udp_piece->recv_buf = (uint8_t*)malloc( udp_piece->total_size + 1 );
				if ( !udp_piece->recv_buf )
				{
					return(-1);
				}
			}

			// å°å¸§å¤´åºéå
			circular_buffer_pop_front( udp_piece->circular_buffer, HEAD_SIZE);
			// è¯»ååçæ°æ®
			bytes_to_read = circular_buffer_read( udp_piece->circular_buffer,
							      &udp_piece->recv_buf[PIECE_FIX_SIZE * p_index],
							      data_len );
			if ( bytes_to_read != data_len )
			{
				UDP_ERR( "%s(%d) There is not enough space, only %d bytes, but need %d bytes\n",
					 __FUNCTION__, __LINE__, bytes_to_write, size );
				return(-1);
			}

			UDP_DEBUG("      , remain size=%d", circular_buffer_size( udp_piece->circular_buffer ) );

			udp_piece->recv_len += data_len;
			if ( udp_piece->recv_pieces == udp_piece->total_pieces )
			{
				udp_piece->total_pieces = 0;
				udp_piece->recv_pieces	= 0;
				if ( udp_piece->recv_len == udp_piece->total_size ) /* current frame is received completely, call Process() */
				{
					/* ç»æäºä¸å¸§æ°æ?*/
					get_all_pieces = 1;
				}
				else  
				{
					UDP_ERR( "recv_len != total_size! recv_len: %d, total_size: %d", udp_piece->recv_len, udp_piece->total_size );
					get_all_pieces = -1;
				}
			}
		}
		else        
		{
			UDP_DEBUG( "temp_size = %d, HEAD_SIZE + data_len = %d\n",
				   circular_buffer_size( udp_piece->circular_buffer ), HEAD_SIZE + data_len );
			break;
		}
	}
	
	return(get_all_pieces);
}