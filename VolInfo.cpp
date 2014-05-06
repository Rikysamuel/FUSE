#include "Header.h"

fstream handle;

VolInfo::VolInfo(){
	capasity = (65536);
	firstEmpty = (1);
	block = (65536);
	cout << capasity << firstEmpty << endl;
	writeHeader();
}

VolInfo::~VolInfo(){
	
}

void VolInfo::writeHeader(){
	char buffer[HEADER_SIZE];
	
	/* tulis magic string ke buffer */
	memset(buffer,0,HEADER_SIZE);
	memcpy(buffer,"CCFS",4);
	memcpy(buffer+4,"CCFS",4);
	
	uint32_t buffcapasity = htobe32(capasity);
	uint32_t buffblockinfo = htobe32(block);
	uint32_t buffinfofree = htobe32(firstEmpty);
	
	char* buffcap = (char*)&buffcapasity;
	char* buffblock = (char*)&buffblockinfo;
	char* buffinfo = (char*)&buffinfofree;

	//36-48
	memcpy(buffer+36,buffcap,4);
	memcpy(buffer+40,buffblock,4);
	memcpy(buffer+44,buffinfo,4);

	/* geser pointer penulisan file ke awal */
	buffer[508] = 'S';
	buffer[509] = 'F';
	buffer[510] = 'C';
	buffer[511] = 'C';
	handle.seekp(0);
	handle.write(buffer, HEADER_SIZE);
	/* tulis buffer */
}

Controller::Controller(){
	handle.open("tes.ccfs",fstream::in | fstream::out | fstream::binary | fstream::trunc);
	VolInfo VI;
	handle.flush();
}

Controller::~Controller(){

}

int main(){
	Controller CI;
}