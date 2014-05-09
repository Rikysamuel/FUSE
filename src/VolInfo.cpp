#include "Header.h"

fstream handle;

VolInfo::VolInfo(string volname){
	capasity = 65536;
	firstEmpty = 1;
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
	
	uint32_t buffcapasity = capasity;
	uint32_t buffblockinfo = block;
	uint32_t buffinfofree = firstEmpty;
	
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
	if (first==0){
		firstEmpty=1;
	}
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
	handle.seekp(temp);
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
	int temp = handle.tellg();
	char buffer[512];
	memset(buffer,0,512);
	handle.write(buffer,512);
	handle.flush();
	handle.seekp(temp+512);
}

void DataPool::createEntry(int block, int pos, string filename, char attr, uint16_t time, uint16_t date, uint16_t index, uint32_t fsize){
	int temp = handle.tellg();
	char buffer[32];
	block = block+131584;

	handle.seekp(block+(pos*32));
	cout << "pointer : " << block+(pos*32) << endl;
	memcpy(buffer,filename.c_str(),filename.length());
	char* s_attr = (char*)&attr;
	memcpy(buffer+21,s_attr,1);
	char* s_time = (char*)&time;
	memcpy(buffer+22,s_time,2);
	char* s_date = (char*)&date;
	memcpy(buffer+24,s_date,2);
	char* s_index = (char*)&index;
	memcpy(buffer+26,s_index,2);
	char* s_fsize = (char*)&fsize;
	memcpy(buffer+28,s_fsize,4);
	handle.write(buffer,32);
	handle.flush();
	handle.seekp(temp);
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
	int temp = handle.tellg();
	cout << temp << endl;
	char *buff = new char[name.length()];
	block = block + HEADER_SIZE + TABLE;
	memcpy(buff,name.c_str(),name.length());
	handle.seekp(block+entry);
	handle.write(buff, name.length());
	handle.seekp(temp);
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
	setAddress(buffer+131070, AT[65535],65535,0xFFCD);
	setAddress(buffer+2, AT[1],1,0xFFFF);
	handle.write(buffer,TABLE);
	cout << "pointer : " << handle.tellg() << endl;
	bool found=false;
	int i =0;
	
	/* Update VolInfo */
	while(i<ALLOC_TABLE && !found){
		if (AT[0].isEmpty()){
			found = true;
		}
		else{
			i++;
		}
	}
	VI.updateVol(i);

	handle.flush();
	cout << "Pointer 1 " << handle.tellg() << endl;
	DataPool DP;
	cout << "Pointer 2 " << handle.tellg() << endl;
	DP.createBlock();
	cout << "Pointer 3 " << handle.tellg() << endl;
	DP.createEntry(0,0,"tes.txt", 7, 21, 22, 0x1122, 123);
	DP.createEntry(0,1,"tes1.txt", 7, 21, 22, 0x1122, 12345);
	DP.createEntry(0,2,"tes2.txt", 7, 21, 22, 0x1122, 12367);
	DP.createEntry(0,4,"tes2.txt", 7, 21, 22, 0x1122, 12367);
	DP.createEntry(0,15,"tescoba.txt", 7, 21, 22, 0xDDFF, 12367);
	DP.createBlock();
}

Controller::~Controller(){

}

void Controller::setAddress(char * buffer, AllocTable A, int position, unsigned short address){
	position = 2*position + 512;
	int temp = handle.tellg();
	handle.seekp(position);
	A.setAddress(address);
	unsigned short _addr = A.getAddress();
	char buff[2];
	memcpy((char*)buff,&_addr,2);
	memcpy(buffer,buff,2);
	handle.write(buffer,4);
	handle.flush();
	handle.seekp(temp); 
}