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
#include <stdexcept>

using namespace std;
using std::runtime_error;

using std::string;
using std::fstream;

#define HEADER_SIZE 512
#define FILE_SIZE 32000000

#define TABLE_SIZE 32
#define ALLOC_TABLE 65536
#define TABLE 131072

class VolInfo{	
	public:
		uint32_t capasity;		/* kapasitas block datapool 65536 block */
		uint32_t firstEmpty;	/* block kosong pertama */
		uint32_t block;			/* yang blm dipake */
		time_t mount_time;
	
		/* konstruktor & destruktor */
		VolInfo();
		~VolInfo();

		void VolInit(string volname);
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
		uint32_t file_size;

		DataPool();
		~DataPool();
		void createBlock();
		void createEntry(int block, int pos, string filename, char attr, uint16_t time, uint16_t date, uint16_t index, uint32_t fsize);
		char * readFile( int begin, int number);
};

class Controller{
	public:
		VolInfo VI;
		AllocTable AT[ALLOC_TABLE];
		DataPool DP;
		
		Controller();
		~Controller();
		void setAddress(char * buffer, AllocTable A, int position, unsigned short address);
		void createNew(string filename);
		void loadFile(string filename);
		bool isEmptyEntry(int block, int entry);
};

#endif