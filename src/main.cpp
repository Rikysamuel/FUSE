#include <iostream>
//#include "fuse_impl.hpp"
#include "Header.h"

using namespace std;
//struct fuse_operations ccfs_op;
//CCFS filesystem;

/* inisialisasi mapping fungsi-fungsi fuse */
/*
void init_fuse(){
	ccfs_op.getattr		= ccfs_getattr;
	ccfs_op.open		= ccfs_open;
	ccfs_op.read		= ccfs_read;
	ccfs_op.write		= ccfs_write;
	ccfs_op.truncate	= ccfs_truncate;
	ccfs_op.rename		= ccfs_rename;
	ccfs_op.readdir		= ccfs_readdir;
	ccfs_op.mknod		= ccfs_mknod;
	ccfs_op.unlink		= ccfs_unlink;
	ccfs_op.rmdir		= ccfs_rmdir;
	ccfs_op.mkdir		= ccfs_mkdir;
}
*/

int main(int argc, char** argv){
/*	
	// jika terdapat argumen --new, buat baru
	if (argc > 3 && string(argv[3]) == "--new")
		filesystem.create(argv[2]);
	else
		filesystem.load(argv[2]);		
	
	// inisialisasi fuse
	int fuse_argc = 2; 
	char* fuse_argv[2] = {argv[0], argv[1]};
	
	init_fuse();
	
	// serahkan ke fuse
	return fuse_main(fuse_argc, fuse_argv, &ccfs_op, NULL);
*/
	Controller CI("apaaja.ccfs");
}
