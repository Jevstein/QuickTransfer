#include "prefixhead.h"
#include "file.h"
#include "session.h"
#include <fcntl.h>

int file_id_ = 1;

int jvt_file_new_file_id()
{
	return file_id_++;
}

int jvt_file_init(jvt_file_t *file, char* name, jvt_session_t *session)
{
	assert(file);

	strcpy(file->fileinfo_.filename, "../resource/");
	strcat(file->fileinfo_.filename, name);
	file->session_ = session;

	if (-1 == access(file->fileinfo_.filename, F_OK))
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
	
	file->fileinfo_.filesize = lseek(fd, 0, SEEK_END);
	file->data_ = mmap(NULL, file->fileinfo_.filesize, PROT_READ, MAP_SHARED, fd, 0);
	if (file->data_ == MAP_FAILED)
	{
		close(fd);
		return -3;
	}
	close(fd);

	file->block_ = 0;
	file->fileid_ = jvt_file_new_file_id();

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

char* jvt_file_read(jvt_file_t *file, int block, int *len)
{
	int offset = FILE_BLOCK * block;
	if (offset > file->fileinfo_.filesize)
		return NULL;

	*len = file->fileinfo_.filesize - offset;
	if (*len > FILE_BLOCK)
		*len = FILE_BLOCK;

	return ((char *)file->data_ + offset);
}