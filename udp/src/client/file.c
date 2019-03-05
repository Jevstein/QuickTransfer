#include "prefixhead.h"
#include "file.h"
#include "session.h"
#include <fcntl.h>

int file_id_ = 1;

int jvt_file_new_file_id()
{
	return file_id_++;
}

int jvt_file_init(jvt_file_t *file, int fileid, int filesize, jvt_session_t *session)
{
	assert(file);

	// strcpy(file->fileinfo_.filename, "../resource/");
	// strcat(file->fileinfo_.filename, name);
	file->session_ = session;

	int fd = open(file->fileinfo_.filename, O_RDWR|O_CREAT|O_TRUNC, 0644);
	if (fd < 0)
	{
        LOG_ERR("failed to open file[%s]!", file->fileinfo_.filename);
		return -2;
	}
	ftruncate(fd, filesize);
	
	file->data_ = mmap(NULL, filesize, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if (file->data_ == MAP_FAILED)
	{
        LOG_ERR("failed to mmap file[%s]!", file->fileinfo_.filename);
		close(fd);
		return -3;
	}
	close(fd);

	file->fileinfo_.filesize = filesize;
	file->block_ = 0;
	file->fileid_ = fileid;

	return 0;
}

void jvt_file_uninit(jvt_file_t *file)
{
	if (!file)
		return;

	munmap(file->data_, file->fileinfo_.filesize);

	file->fileid_ = 0;
	file->fileinfo_.filesize = 0;
    file->fileinfo_.filename[0] = '\0';
    file->session_ = NULL;

    file->data_ = NULL;
    file->block_ = 0;
}

// char* jvt_file_read(jvt_file_t *file, int block, int *len)
// {
// 	int offset = FILE_BLOCK * block;
// 	if (offset > file->fileinfo_.filesize)
// 		return NULL;

// 	*len = file->fileinfo_.filesize - offset;
// 	if (*len > FILE_BLOCK)
// 		*len = FILE_BLOCK;

// 	return ((char *)file->data_ + offset);
// }


int jvt_file_write(jvt_file_t *file, int block, char *data, int len)
{
	int offset = FILE_BLOCK * block;
	if (offset > file->fileinfo_.filesize)
		return -1;

	memcpy((void *)((char *)file->data_ + block), (void *)data, len);
	file->block_ = block;

	return 0;
}