#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
// #include <sys/sendfile.h>
#include <time.h>
#include "PrefixHead.h"

#ifdef YI_OS_MAC
    #define basename(name) (name)
#endif
 
int main( int argc, char* argv[] )
{
    if( argc <= 3 )
    {
        printf( "usage: %s ip_address port_number filename\n", basename( argv[0] ) );
        return 1;
    }
    static char buf[1024];
    memset(buf,'\0',sizeof(buf));
    const char* ip = argv[1];
    int port = atoi( argv[2] );
    const char* file_name = argv[3];
 
    int filefd = open( file_name, O_WRONLY );
    if(filefd<=0)
    	printf("open error:%s",strerror(errno));
    assert( filefd > 0 );
		
		FILE *fp=fdopen(filefd,"w");
		
    struct sockaddr_in address;
    socklen_t len=sizeof(address);
    bzero( &address, sizeof( address ) );
    address.sin_family = AF_INET;
    inet_pton( AF_INET, ip, &address.sin_addr );
    address.sin_port = htons( port );
 
    int sock = socket( PF_INET, SOCK_STREAM, 0 );
    assert( sock >= 0 );
		int num;
		int ret=connect(sock,(struct sockaddr*)&address,len);
    if ( ret < 0 )
    {
        printf( "connect errno: %s\n", strerror(errno) );
    }
    else
    {
        while( (num=recv(sock,buf,sizeof(buf),0))>0 ){
        	fprintf(fp,"%s",buf);
        	memset(buf,'\0',sizeof(buf));
        }
        
        close( sock );
    }
 
    close( sock );
    return 0;
}
