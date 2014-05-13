#include "fuse_impl.hpp"

extern Controller filesystem;

int ccfs_getattr(const char* path, struct stat* stbuf) {
	if (string(path) == "/"){
		stbuf->st_nlink = 1;
		stbuf->st_mode = S_IFDIR | 0777; // file dengan permission rwxrwxrwx
		stbuf->st_mtime = filesystem.mount_time;
		return 0;
	}
	else {
		if (!isEmptyEntry(0, path)) {
			return -ENOENT;
		}
		stbuf->st_nlink = 1;
		stbuf->st_mode = S_IFDIR | 0777;
		stbuf->st_size = filesystem.DP.file_size;
		//stbuf->st_mtime = ;

		return 0;
	}
}