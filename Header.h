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
#define FILE_SIZE 32000000

#define TABLE_SIZE 32
#define ALLOC_TABLE 65536
#define TABLE 131072

//fstream handle;

class VolInfo{	
	public:
		uint32_t capasity;		/* kapasitas block datapool 65536 block */
		uint32_t firstEmpty;	/* block kosong pertama */
		uint32_t block;			/* yang blm dipake */
		time_t mount_time;
	
		/* konstruktor & destruktor */
		VolInfo(string volname);
		~VolInfo();

		void writeHeader(string volname);
		void updateVol(char first);
};

class AllocTable{
	public:
		unsigned short address;

		AllocTable();
		~AllocTable();
		bool isEmpty();
		void setAddress(unsigned short _addr);	//set address
		unsigned short getAddress();
		void setEmpty();	
		void write();
};

class DataPool{
	public:
		char attribute;

		DataPool();
		~DataPool();
		void createBlock();
		void createEntry();
		char * readFile(char * buffer, int begin, int number);
		bool isDirectory();
		void setName(int block, int entry, string name);
		char getAttr();
		unsigned short MakeTime();
		unsigned short MakeDate();
		unsigned short setPointer();
		int size();
};

class Controller{
	public:
		Controller(string filename);
		~Controller();
		void setAddress(char * buffer, AllocTable A, int position, unsigned short address);
};

#endif