/************************************************************************************
 * 	
 * 	Name    : Memory Logger                        
 * 	Author  : Noah Shibley                         
 * 	Date    : Sep 7th, 2013                                    
 * 	Version : 0.1                                              
 * 	Notes   : Manager data logging and retrieval               
 * 
 * 	Copyright (c) 2013 All right reserved.
 * 
 ***********************************************************************************/

/* Example Data Structure, anything is fine they just always need to be the same! */
/*
struct MyRec {
  char timestamp[11];
  float pressure;
  int temperature;
} myrec;

*/


#ifndef MemoryLogger_H
#define MemoryLogger_H

#include <SST25VF.h>

class MemoryLogger {
 
public: 

	MemoryLogger();
    void begin(int chipSelect,int writeProtect,int hold,uint8_t recordSize); //setup the memory
	void appendRec(const uint8_t dataBuffer[]); //add a record. 
	void selectRec(uint32_t index,uint8_t dataBuffer[]); //select a certain record by index number
	void selectAll(); //read all records. 
	uint32_t recCount(); //get current number of records
	uint32_t recsRemain(); //get how many records are remaining till full
	uint32_t bytesRemain(); //get how many bytes are remaining
	boolean limit(); //check whether you have reached the record limit
	void clearAllRec(); //erase everything!!! (will reset header data to 0 but keep header.upload count)
	void printHeader(); //serial print header data stats. 

 
private: 
	
	/*********************  Header Data Structure  **************************/
	struct Header
	{	
	  uint32_t numRec;   //count of current records
	  uint8_t  recSize; //size of records in bytes
	  uint32_t nextAddress; //flash mmemory address
	  uint32_t totalRec; //total possible records
	  uint16_t uploadCount; //number of times the data has been uploaded and cleared. 
	};
	/***********************************************************************/
	
	SST25VF flash;
	Header head;
	uint8_t recordSizeTemp;
	
	void writeHead(); //save header data to flash for longterm storage
    void readHead(); //read header data from flash at startup.
	
	
	
 
}; 
 
#endif
