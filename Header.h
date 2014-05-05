#ifndef VOLINFO_H
#define VOLINFO_H

#include <cstdlib>
#include <cstring>
#include <string>
#include <fstream>
#include <ctime>
#include <iostream>

using namespace std;

using std::string;
using std::fstream;


#define HEADER_SIZE 512

class VolInfo{
	private:
		fstream handle;			// file simple.fs
	
	public:
		int capasity;			// jumlah slot yang masih kosong
		int firstEmpty;			// slot pertama yang masih kosong
		time_t mount_time;		// waktu mounting, diisi di konstruktor
	
		/* konstruktor & destruktor */
		VolInfo();
		~VolInfo();

		void writeHeader();
};

#define ALLOC_SIZE 131072


class AllocTable{
	private:
		fstream handle;

	public:
		AllocTable();
		~AllocTable();
};

#define DATA_SIZE 33554432
#define BLOCK_SIZE 32
#define BLOCK_NUM 16
#define BLOCK_TOTAL 65536

class DataPool{
	private:
		char blocksize[BLOCK_SIZE];

	public:
		DataPool();
		~DataPool();
		void InitRoot();
};

#endif