#ifndef _HEADER_H_
#define _HEADER_H_

#include <cstdlib>
#include <stdint.h>
#include <cstring>
#include <string>
#include <fstream>
#include <ctime>
#include <endian.h>
#include <iostream>

using namespace std;

using std::string;
using std::fstream;

#define HEADER_SIZE 512
#define FILE_SIZE 33686016

#define ALLOC_SIZE 131072
#define DATA_SIZE 33554432

#define BLOCK_SIZE 32
#define BLOCK_NUM 16

//fstream handle;

class VolInfo{	
	public:
		uint32_t capasity;			
		uint32_t firstEmpty;			
		uint32_t block;	//yg blm dipake
		time_t mount_time;		// waktu mounting, diisi di konstruktor
	
		/* konstruktor & destruktor */
		VolInfo();
		~VolInfo();

		void writeHeader();
};

class AllocTable{
	public:
		AllocTable();
		~AllocTable();
		void isEmpty();
		void setEmpty();
		void setNext();

};

class DataPool{
	private:
		char blocksize[BLOCK_SIZE];

	public:
		DataPool();
		~DataPool();
		void InitRoot();
};

class Controller{
	public:
		Controller();
		~Controller();
};

#endif