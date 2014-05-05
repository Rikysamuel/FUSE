#include "Header.h"

VolInfo::VolInfo(){
	time(&mount_time);
	handle.open("tes.ccfs",fstream::in | fstream::out | fstream::binary | fstream::trunc);
	capasity = (unsigned char) (65536);
	firstEmpty = (unsigned char) (1);
	writeHeader();
}

VolInfo::~VolInfo(){
	handle.close();
}

void VolInfo::writeHeader(){
	char buffer[HEADER_SIZE];
	
	/* tulis magic string ke buffer */
	buffer[0]='C';
	buffer[1]='C';
	buffer[2]='F';
	buffer[3]='S';
	buffer[4]='C';
	buffer[5]='C';
	buffer[6]='F';
	buffer[7]='S';

	for(int i=8;i<36;i++){
		buffer[i]='\0';
	}
	//36-48
	
	/* tulis available dan firstEmpty ke buffer */
	for(int i=48;i<508;i++){
		buffer[i]='\0';
	}
	/* geser pointer penulisan file ke awal */
	buffer[508] = 'S';
	buffer[509] = 'F';
	buffer[510] = 'C';
	buffer[511] = 'C';
	handle.seekp(0);
	
	/* tulis buffer */
	handle.write(buffer, HEADER_SIZE);
	handle.flush();
}