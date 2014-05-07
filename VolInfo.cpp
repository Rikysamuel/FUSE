#include "Header.h"

	fstream handle;

VolInfo::VolInfo(string volname){
	capasity = (65536);
	firstEmpty = (1);
	block = capasity - firstEmpty;
	writeHeader(volname);
}

VolInfo::~VolInfo(){
	
}

void VolInfo::writeHeader(string volname){
	char buffer[HEADER_SIZE];
	memset(buffer,0,HEADER_SIZE);
	memcpy(buffer,"CCFS",4);
	memcpy(buffer+4,volname.c_str(),volname.length());
	
	uint32_t buffcapasity = htobe32(capasity);
	uint32_t buffblockinfo = htobe32(block);
	uint32_t buffinfofree = htobe32(firstEmpty);
	
	char* buffcap = (char*)&buffcapasity;			/* kapasitas */
	char* buffblock = (char*)&buffblockinfo;		/* jumlah block tersisa */
	char* buffinfo = (char*)&buffinfofree;			/* block pertama yang kosong */

	memcpy(buffer+36,buffcap,4);
	memcpy(buffer+40,buffblock,4);
	memcpy(buffer+44,buffinfo,4);

	buffer[508] = 'S';
	buffer[509] = 'F';
	buffer[510] = 'C';
	buffer[511] = 'C';
	handle.write(buffer, HEADER_SIZE);
}

void VolInfo::updateVol(char first){
	firstEmpty = first;
	block = capasity - first;

	int temp = handle.tellg();
	uint32_t buffblockinfo = htobe32(block);
	uint32_t buffinfofree = htobe32(firstEmpty);
	char* buffcap = (char*)&buffblockinfo;
	char* buffinfo = (char*)&buffinfofree;

	handle.seekp(40);
	char buffer[8];

	memcpy(buffer,buffcap,4);
	memcpy(buffer+4,buffinfo,4);
	handle.write(buffer, 8);
}

AllocTable::AllocTable(){

}

AllocTable::~AllocTable(){

}

void AllocTable::setAddress(unsigned short _addr){
	address = _addr;
}

unsigned short AllocTable::getAddress(){
	return address;
}

void AllocTable::setEmpty(){
	address = 0x0000;
}

bool AllocTable::isEmpty(){
	return address = 0x0000;
}

DataPool::DataPool(){
	//createEntry();
}

DataPool::~DataPool(){

}

/* Juga update firstEmpty di VolInfo */
void DataPool::createBlock(){
	char buffer[512];
	memset(buffer,0,512);
	handle.write(buffer,512);
	handle.flush();
}

void DataPool::createEntry(){

}

char * DataPool::readFile(char * buffer,int begin, int number){
	buffer[number];
	int temp = handle.tellg();
	handle.seekp(begin);
	handle.read(buffer,number);
	handle.seekp(temp);
	return buffer;
}

bool DataPool::isDirectory(){

}

void DataPool::setName(int block, int entry, string name){
	char *buff = new char[name.length()];
	block = block + HEADER_SIZE + TABLE;
	memcpy(buff,name.c_str(),7);
	handle.seekp(block+entry);
	handle.write(buff, 7);
}

Controller::Controller(string filename){
	/* initialize filesystem */
	char buffer[TABLE];
	handle.open(filename.c_str(),fstream::in | fstream::out | fstream::binary | fstream::trunc);

	/* Volume Information initialization */
	VolInfo VI(filename);

	/* ALlocation Table initialization */
	AllocTable AT[ALLOC_TABLE];
	memset(buffer,0,TABLE);
	setAddress(buffer, AT[0],0,0xFFFF);
	setAddress(buffer, AT[65535],65535,0xFFFF);
	handle.write(buffer,TABLE);
	bool found=false;
	int i =0;
	while(i<ALLOC_TABLE && !found){
		if (AT[0].isEmpty()){
			found = true;
		}
		else{
			i++;
		}
	}
	VI.updateVol(i);

	//handle.seekp(TABLE+HEADER_SIZE-1);
	handle.flush();
	DataPool DP;
	DP.createBlock();
	VI.firstEmpty++;
	DP.setName(0,1,"tes.txt");
}

Controller::~Controller(){

}

void Controller::setAddress(char * buffer, AllocTable A, int position, unsigned short address){
	position = 2*position + 512;
	int temp = handle.tellg();
	handle.seekp(position);
	A.setAddress(address);
	buffer[position-512] = A.getAddress();
	buffer[position-512+1] = A.getAddress();
	handle.write(buffer,4);
	handle.flush();
	handle.seekp(temp);
}

int main(){
	Controller CI("apaaja.ccfs");
}
