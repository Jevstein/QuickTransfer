#include "prefixhead.h"
#include "file.h"
#include "session.h"


int jvt_file_init(jvt_file_t *file, char* name, jvt_session_t *session)
{
	assert(file);

	strcpy(file->fileinfo_.filename, "../resource/" name);
	file->session_ = session;

	if (-1 == access(FIFO_FILE_PATH, F_OK))
    {
        LOG_ERR("the file[%s] is not exist!", file->fileinfo_.filename);
		return -1;
    }

	int fd = open(file->fileinfo_.filename, O_RDONLY, 0644);
	if (fd < 0)
	{
        LOG_ERR("failed to open file[%s]!", file->fileinfo_.filename);
		return -2;
	}
	
	file->data_ = mmap(NULL, sizeof(struct STU), PROT_READ, MAP_SHARED, fd, 0);
	if (p == MAP_FAILED)
	{
		close(fd);
		return -3;
	}
	file->filesize = lseek(fd, 0, SEEK_END);
	close(fd);

	file->block_ = 0;

	return 0;
}

int jvt_file_open(jvt_file_t *F)
{
	
	return 0;
}

void jvt_file_uninit(jvt_file_t *S)
{
	// int ret = munmap(p, sizeof(student));
	// if (ret < 0)
	// {
	// 	perror("mmumap");
	// 	exit(4);
	// }
}

void* jvt_file_read(jvt_file_t *F, int block, int *len)
{
	return NULL;
}