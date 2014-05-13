#include "Header.h"

fstream handle;

/* Implementasi kelas Volume Information */
VolInfo::VolInfo(){

}

VolInfo::~VolInfo(){
	
}

/* Inisialisasi jika "--new" */
void VolInfo::VolInit(string volname){
	capasity = 65536;	//statis kapasitas
	firstEmpty = 1;		//block pertama dari data pool yang kosong
	block = 65535;			//jumlah block tersisa dari datapool
	writeHeader(volname);		//tulis inisialisasi ke header
}

/* Write VolInfo ke file*/
void VolInfo::writeHeader(string volname){
	char buffer[HEADER_SIZE];
	memset(buffer,0,HEADER_SIZE);
	memcpy(buffer,"CCFS",4);
	memcpy(buffer+4,volname.c_str(),volname.length());
	
	char* buffcap = (char*)&capasity;			/* kapasitas */
	char* buffblock = (char*)&block;			/* jumlah block tersisa */
	char* buffinfo = (char*)&firstEmpty;		/* block pertama yang kosong */

	memcpy(buffer+36,buffcap,4);	/* tulis kapasitas FS ke vol info */
	memcpy(buffer+40,buffblock,4);	/* tulis sisa block ke vol info */
	memcpy(buffer+44,buffinfo,4);	/* tulis block kosong pertama ke vol info */

	buffer[508] = 'S';
	buffer[509] = 'F';
	buffer[510] = 'C';
	buffer[511] = 'C';
	handle.write(buffer, HEADER_SIZE);
}

/* Update Volume Information untuk load */
void VolInfo::updateVol(char first){
	firstEmpty = first;
	block = capasity - first;

	int temp = handle.tellg();		//simpan informasi lokasi pointer sekarang

	char* buffblock = (char*)&block;
	char* buffinfo = (char*)&firstEmpty;

	handle.seekp(40);				//set pointer ke posisi "blok yang tersisa"
	char buffer[8];

	memcpy(buffer,buffblock,4);		//update jumlah block tersisa
	memcpy(buffer+4,buffinfo,4);	//update block kosong pertama
	
	handle.write(buffer, 8);
	handle.seekp(temp);				//kembalikan posisi pointer ke posisi semula
}

/* Implementasi Kelas Allocation Table */
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
	return address == 0x0000;
}


/* Implementasi Kelas Data Pool */
DataPool::DataPool(){

}

DataPool::~DataPool(){

}

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
	block = (512*block)+131584;		/* 131584 : pointer terakhir setelah alloc table */

	file_size = fsize;
	handle.seekp(block+(pos*32));

	/* Tulis info entri */
	memcpy(buffer,filename.c_str(),filename.length());	/* nama file/dir */
	char* s_attr = (char*)&attr;
	memcpy(buffer+21,s_attr,1);							/* atribut file/dir */
	char* s_time = (char*)&time;
	memcpy(buffer+22,s_time,2);							/* jam file/dir dibuat */
	char* s_date = (char*)&date;
	memcpy(buffer+24,s_date,2);							/* tanggal file/dir dibuat */
	char* s_index = (char*)&index;
	memcpy(buffer+26,s_index,2);						/* pointer ke entry isi file */
	char* s_fsize = (char*)&fsize;
	memcpy(buffer+28,s_fsize,4);						/* ukuran file */
	
	handle.write(buffer,32);
	handle.flush();
	handle.seekp(temp);
}

char * DataPool::readFile(int begin, int number){	/* Baca file muali dari begin sebanyak number karakter */
	char *buffer;
	buffer = new char[number];
	char buff[number];
	int temp = handle.tellg();

	handle.seekp(begin);
	handle.read(buff,number);			/* Baca karakter sebanyak 'number', dimasukan kedalam buff */
	memcpy(buffer,buff,number);

	handle.seekp(temp);
	return buffer;
}

Controller::Controller(){
	
}

Controller::~Controller(){

}

void Controller::createNew(string filename){
	/* initialize filesystem */
	char buffer[TABLE];
	handle.open(filename.c_str(),fstream::in | fstream::out | fstream::binary | fstream::trunc);

	/* Volume Information initialization */
	VI.VolInit(filename);

	/* ALlocation Table initialization */
	memset(buffer,0,TABLE);
	setAddress(buffer, AT[0],0,0xFFFF);
	setAddress(buffer+131070, AT[65535],65535,0xFFFF);
	handle.write(buffer,TABLE);
	handle.flush();
	
	/* Data Pool initialization */
	for(int i=0;i<65535;i++){
		DP.createBlock();
	}
	
}

void Controller::loadFile(string filename){

	/* buka file dengan mode input-output, dan binary */
	handle.open(filename.c_str(), fstream::in | fstream::out | fstream::binary);
	
	/* cek apakah file ada */
	if (!handle.is_open()){
		handle.close();
		throw runtime_error("File not found: " + filename);
	}
	
	char header[HEADER_SIZE];
	
	/* baca buffer header */
	handle.read(header, HEADER_SIZE);
	
	/* cek magic string */
	if (string(header, 4) != "CCFS"){
		handle.close();
		throw runtime_error("File not valid");
	}
	
	/* baca available dan firstEmpty */
	int temp = handle.tellg();
	uint32_t blockinfo,firstinfo;

	handle.seekp(40);
	handle.read((char*)&blockinfo,4);
	handle.seekp(44);
	handle.read((char*)&firstinfo,4);
	cout << "block tersisa : " << blockinfo << endl;
	cout << "block kosong pertama : " << firstinfo << endl;
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

bool Controller::isEmptyEntry(int block, int entry){
	int start = 131584 + 512*block + 32*entry;
	cout << "start : " << start << endl;
	char *buff;
	buff = DP.readFile(start,1);	//cek karakter pertama dari nama file
	cout << buff << endl;
	if(strcmp(buff,"")==0){		//jika terbaca 0x00
		return true;
	} 
	return false;
}