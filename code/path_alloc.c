#ifdef PATH_MAX
static size_t pathmax=PATH_MAX;
#else
static size_t pathmax=0;
#endif

static long posix_version=0;
static long xsi_version=0;

#define PATH_MAX_GUESS 1024

char* path_alloc(size_t *sizep)
{
	char *ptr;
	size_t size;

	if(posix_version == 0)
		posix_version = sysconf(_SC_VERSION);
	if(xsi_version == 0)
		xsi_version = sysconf(_SC_XOPEN_VERSION);

	if(pathmax == 0){ /* return PATH_MAX+1  for null in path end */
		errno=0;
		if((pathmax=pathconf("/",_PC_PATH_MAX))<0){ /* _PC_PATH_MAX 是相对路径名最大字节数，包括null字节（看标准的版本） */
			if(errno == 0)
				pathmax=PATH_MAX_GUESS;
			else
				err_sys("pathconf error for _PC_PATH_MAX");
		}else{
			pathmax++;
		}
	}

	if((posix_version < 200112L) && (xsi_version < 4))
		size=pathmax+1;
	else
		size=pathmax;
	if((ptr=(char *)malloc(size))==NULL)
		err_sys("malloc error for pathname");
	if(sizep !=NULL)
		*sizep=size;
	return ptr;
}

