#include <stdlib.h>
#include <string.h>
#include "inner.h"
#include "udp_piece.h"

// #define UDP_ERR		printf
// #define UDP_DEBUG	printf

/**
 * @brief 初始化资�?
 * @param buf_size 设置缓冲区数据的最大长�?
 * @return 成功则返回一个句柄，失败则返回NULL
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
 * @brief 释放资源
 * @param udp_piece 句柄
 */
void udp_piece_deinit( udp_piece_t *udp_piece )
{
	/* 释放资源 */
	if ( udp_piece )
	{
		if ( udp_piece->recv_buf )
			free( udp_piece->recv_buf );
		udp_piece->recv_buf	= NULL;
		udp_piece->recv_pieces	= 0;
		udp_piece->total_size	= 0;    /* 总数据大�?*/
		udp_piece->total_pieces = 0;    /* 分片总数�?*/
		udp_piece->left			= 0;    /* 最后一片的大小 */
		udp_piece->piece_size	= 0;    /* 分片大小 */
		udp_piece->recv_len	= 0;
		circular_buffer_deinit( udp_piece->circular_buffer );
		udp_piece->circular_buffer = NULL;
	}
}

/**
 * @brief 重置，这里不会重新分配资源，只是讲部分参数重置到初始化状�?
 * @param udp_piece 句柄
 */
void udp_piece_reset( udp_piece_t *udp_piece )
{
	/* 数据重置 */
	if ( udp_piece )
	{
		if ( udp_piece->recv_buf )
			free( udp_piece->recv_buf );
		udp_piece->recv_buf	= NULL;
		udp_piece->send_ptr = NULL;
		udp_piece->recv_pieces = 0;
		udp_piece->total_size = 0;    	/* 总数据大�?*/
		udp_piece->total_pieces = 0;    /* 分片总数�?*/
		udp_piece->left = 0;    		/* 最后一片的大小 */
		udp_piece->piece_size = 0;    	/* 分片大小 */
		udp_piece->recv_len	= 0;
		circular_buffer_reset( udp_piece->circular_buffer );
	}
}

/**
 * @brief 根据长度进行切割，返回切割后的分片数�?
 * @param udp_piece 句柄
 * @param buf       要分片数据的指针
 * @param size      要分片数据的长度
 * @return 返回分片的数�?
 */
int udp_piece_cut( udp_piece_t *udp_piece, const void *buf, int size )
{
	if (!udp_piece || size < 0)
		return 0;

	udp_piece->send_ptr = (uint8_t*)buf;
	udp_piece->total_size = size;
	udp_piece->left = size % PIECE_FIX_SIZE;	// 最后一个分片数据的大小
	udp_piece->total_pieces = 
		(udp_piece->left > 0) ? (size / PIECE_FIX_SIZE + 1) : (size / PIECE_FIX_SIZE);

	return udp_piece->total_pieces;
}

/**
 * @brief 根据分片编号获取切片指针及分片数据大�?
 * @param udp_piece 句柄
 * @param index     分片编号
 * @param got_piece_size 获取指定编号分片数据的长�?
 * @return 返回指定分片编号的数据指�?
 */
uint8_t *udp_piece_get( udp_piece_t *udp_piece, int index, int *got_piece_size )
{
	int piece_size = 0;

	if (!udp_piece || !got_piece_size)
		return NULL;

	*got_piece_size = 0;

	// 分片大小
	if(((udp_piece->total_pieces - 1) == index)		// 是不是最后一个分�?
		&& (udp_piece->left > 0))
	{
		piece_size = udp_piece->left;
	}
	else
	{	
		piece_size = PIECE_FIX_SIZE;
	}

	// 初始化为0
	memset( udp_piece->piece_buf, 0, sizeof(udp_piece->piece_buf) );
	// 填充分片帧头
	// 同步�?
	udp_piece->piece_buf[HEAD_POS_SYNC_WORD] = 0xAF;
	udp_piece->piece_buf[HEAD_POS_SYNC_WORD + 1] = 0xAE;
	// 所有分片数据的大小
	udp_piece->piece_buf[HEAD_POS_TOTAL_SIZE] = udp_piece->total_size >> 8;
	udp_piece->piece_buf[HEAD_POS_TOTAL_SIZE + 1] = (udp_piece->total_size & 0xff);
	// 所有分片的数量
	udp_piece->piece_buf[HEAD_POS_TOTAL_PIECES] = udp_piece->total_pieces >> 8;
	udp_piece->piece_buf[HEAD_POS_TOTAL_PIECES + 1] = (udp_piece->total_pieces & 0xff);
	// 分片编号，从0开�?
	udp_piece->piece_buf[HEAD_POS_P_INDEX] = index >> 8;
	udp_piece->piece_buf[HEAD_POS_P_INDEX + 1] = (index & 0xff);
	// 分片数据的大�?
	udp_piece->piece_buf[HEAD_POS_P_LENGTH] = piece_size >> 8;
	udp_piece->piece_buf[HEAD_POS_P_LENGTH + 1] = (piece_size & 0xff);

	// 把用户数据拷贝到分片数据�?
	memcpy(&udp_piece->piece_buf[HEAD_SIZE], &udp_piece->send_ptr[PIECE_FIX_SIZE * index], piece_size);
	*got_piece_size = piece_size + HEAD_SIZE;
	
	return udp_piece->piece_buf;
}

/**
 * @brief 重组分片
 * @param udp_piece 句柄
 * @param buf   分片数据的指�?
 * @param size  分片数据的长�?
 * @return  返回-1则重组失败，返回0则正在重组中，返�?则重组成�?
 */
int udp_piece_merge( udp_piece_t *udp_piece, void *buf, int size )
{
	uint8_t *piece_buf = NULL;
	int temp_size = 0;
	int temp_total_size = 0;
	int temp_total_pieces = 0;
	int p_index = 0;
	int get_all_pieces = 0;
	
	// 检测头部是否有同步�?
	piece_buf = (uint8_t*)buf;
	temp_size = size;
	for(int i = 0; i < size; i++)
	{
		if((piece_buf[0] == 0xAF) && (piece_buf[1] == 0xAE))
			break;
		piece_buf++;
		temp_size--;
	}

	// 如果检测到同步字，且剩余数据长度还超过 分片帧头长度
	while (temp_size > HEAD_SIZE)
	{
		// 当前分片的数据长�?
		int data_len = (piece_buf[HEAD_POS_P_LENGTH] << 8) + (piece_buf[HEAD_POS_P_LENGTH+1]);
		if(temp_size >= (HEAD_SIZE + data_len))
		{
			// 获取分片编号
			p_index = (piece_buf[HEAD_POS_P_INDEX] << 8) + (piece_buf[HEAD_POS_P_INDEX+1]);
			if(udp_piece->total_size == 0)		// 重置后第一次收到数�?
			{
				// 获取分片数据的总大�?
				udp_piece->total_size = (piece_buf[HEAD_POS_TOTAL_SIZE] << 8) + (piece_buf[HEAD_POS_TOTAL_SIZE+1]);
				// 获取总的分片数量
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
			// 分析下新的分片是否和原来的分片组有区�?
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

			udp_piece->recv_len += data_len;		// 分片数据的累�?
			if(udp_piece->recv_pieces == udp_piece->total_pieces)
			{
				udp_piece->total_pieces = 0;
				udp_piece->recv_pieces = 0;
				if(udp_piece->recv_len == udp_piece->total_size)
				{
					get_all_pieces = 1;		// 把所有分片组成完整的数据�?
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
 * @brief 重组分片，使用环形缓�?
 * @param udp_piece 句柄
 * @param buf   分片数据的指�?
 * @param size  分片数据的长�?
 * @return  返回-1则重组失败，返回0则正在重组中，返�?则重组成�?
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
	 * 从新收到的数据检测是否包含片�?
	 * 检测头�?
	 */
	while ( circular_buffer_size( udp_piece->circular_buffer ) >= 2 )
	{
		circular_buffer_get( udp_piece->circular_buffer, 0, &value0 );          /* 通过索引获取当前�?*/
		circular_buffer_get( udp_piece->circular_buffer, 1, &value1 );
		if ( value0 == 0xAF && value1 == 0xAE )
			break;

		circular_buffer_pop_front( udp_piece->circular_buffer, 1);        /* 出队列一个元�?*/
	}

	/* 如果剩余的数据长度仍大于帧头 */
	while ( circular_buffer_size( udp_piece->circular_buffer ) > HEAD_SIZE )
	{
		/* 当前分片的数据长度（不含帧头�?*/
		circular_buffer_get( udp_piece->circular_buffer, HEAD_POS_P_LENGTH, &value0 ); /* 通过索引获取当前�?*/
		circular_buffer_get( udp_piece->circular_buffer, HEAD_POS_P_LENGTH + 1, &value1 );

		uint32_t data_len = (value0 << 8) + value1;

		/* UDP_DEBUG("%s(%d)\n", __FUNCTION__, __LINE__); */
		if ( circular_buffer_size( udp_piece->circular_buffer ) >= (HEAD_SIZE + data_len) )
		{
			circular_buffer_get( udp_piece->circular_buffer, HEAD_POS_P_INDEX, &value0 );               /* 通过索引获取当前�?*/
			circular_buffer_get( udp_piece->circular_buffer, HEAD_POS_P_INDEX + 1, &value1 );
			p_index = (value0 << 8) + value1;
			if ( udp_piece->total_size == 0 )                                                           /* 重置后第一次收到分�?*/
			{
				/* 计算当前分片所属分片组数据的总大�?*/
				circular_buffer_get( udp_piece->circular_buffer, HEAD_POS_TOTAL_SIZE, &value0 );        /* 通过索引获取当前�?*/
				circular_buffer_get( udp_piece->circular_buffer, HEAD_POS_TOTAL_SIZE + 1, &value1 );
				udp_piece->total_size = (value0 << 8) + value1;

				/* 计算当前分片所属分片组的总分片数�?*/
				circular_buffer_get( udp_piece->circular_buffer, HEAD_POS_TOTAL_PIECES, &value0 );      /* 通过索引获取当前�?*/
				circular_buffer_get( udp_piece->circular_buffer, HEAD_POS_TOTAL_PIECES + 1, &value1 );
				udp_piece->total_pieces = (value0 << 8) + value1;

				/* 重置当前接收分片的数�?*/
				udp_piece->recv_pieces = 0;
				/* 重置当前接收到的数据长度 */
				udp_piece->recv_len = 0;
				/* 如果缓存区有数据则先释放 */
				if ( udp_piece->recv_buf )
					free( udp_piece->recv_buf );
				/* 分配能够存储一个分片组所有的数据的空�?*/
				udp_piece->recv_buf = (uint8_t*)malloc( udp_piece->total_size + 1 );
				if ( !udp_piece->recv_buf )
				{
					return(-1);
				}
			}
			UDP_DEBUG("merge piece[%d]: buf size=%d, piece_data_len=%d, recv_pieces=%d, total_size=%d, total_pieces=%d",
				   p_index, circular_buffer_size(udp_piece->circular_buffer), data_len, udp_piece->recv_pieces, udp_piece->total_size, udp_piece->total_pieces);

			/* 计算当前分片所属分片组数据的总大�?*/
			circular_buffer_get( udp_piece->circular_buffer, HEAD_POS_TOTAL_SIZE, &value0 );        /* 通过索引获取当前�?*/
			circular_buffer_get( udp_piece->circular_buffer, HEAD_POS_TOTAL_SIZE + 1, &value1 );
			tmp_total_size = (value0 << 8) + value1;

			/* 计算当前分片所属分片组的总分片数�?*/
			circular_buffer_get( udp_piece->circular_buffer, HEAD_POS_TOTAL_PIECES, &value0 );      /* 通过索引获取当前�?*/
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

			// 将帧头出队列
			circular_buffer_pop_front( udp_piece->circular_buffer, HEAD_SIZE);
			// 读取分片数据
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
					/* 组成了一帧数�?*/
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