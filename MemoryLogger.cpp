/************************************************************************************
 * 	
 * 	Name    : MemoryLogger                        
 * 	Author  : Noah Shibley                         
 * 	Date    : Sep 7th, 2013                                    
 * 	Version : 0.1                                              
 * 	Notes   : Manager data logging and retrieval      
 * 
 * 	Copyright (c) 2013 All right reserved.
 * 
 ***********************************************************************************/

#include "MemoryLogger.h"

// =========================================================
// = Constructor                                           =
// =========================================================
MemoryLogger::MemoryLogger(){




}

// =========================================================
// = Begin                                                 =
// =========================================================

void MemoryLogger::begin(int chipSelect,int writeProtect,int hold,uint8_t recordSize){

	flash.begin(chipSelect,writeProtect,hold);
	delay(50);
	readHead(); //try to read the header data from the flash
//	if((head.numRec == 255) && (head.nextAddress == 255)) //flash is blank
//	{
		head.numRec = 0;   
		head.recSize = recordSize; 
		head.nextAddress = sizeof(head); 
		head.totalRec = recsRemain();
		head.uploadCount = 0;
		
		delay(50);
		writeHead();
		delay(50);
//	}

}

// =========================================================
// = appendRec                                             =
// =========================================================
void MemoryLogger::appendRec(const uint8_t dataBuffer[]){
		
	if (!limit())
	{	
		uint32_t lastAddress = flash.writeArray(head.nextAddress, dataBuffer,head.recSize); //store the bytes
		head.nextAddress = lastAddress;
		head.numRec++;
		head.totalRec = recsRemain(); 

		//writeHead();
	}
}

// =========================================================
// = selectRec                                             =
// =========================================================
void MemoryLogger::selectRec(uint32_t index,uint8_t dataBuffer[]){
	
	//readHead();
	if(index < head.numRec)
	{
		uint32_t recAfter = head.numRec - index; //number of records after the id record
		uint32_t bytesUsed = head.recSize * recAfter; //total amount of bytes used by them. 
		uint32_t selectAddress = head.nextAddress - bytesUsed; //current address - those bytes, = correct address.
		flash.readArray(selectAddress,dataBuffer,head.recSize); //get the rec as an array
	
	}
	
}

// =========================================================
// = clearAllRec                                           =
// =========================================================
void MemoryLogger::clearAllRec(){
	//readHead();
	head.numRec = 0;   
	//head.recSize = 0; 
	head.nextAddress = sizeof(head); 
	head.totalRec = 0; 
	flash.totalErase();
	//writeHead(); //rewrite the header into memory
}

// =========================================================
// = recCount                                              =
// =========================================================
uint32_t MemoryLogger::recCount(){
	return head.numRec;
}

// =========================================================
// = recsRemain                                            =
// =========================================================
uint32_t MemoryLogger::recsRemain(){
	
	uint32_t maxBytes = FLASH_MAX_BYTES - sizeof(head);
	uint32_t maxRec =  maxBytes / head.recSize;
	return maxRec - head.numRec;
	
}

// =========================================================
// = bytesRemain                                           =
// =========================================================
uint32_t MemoryLogger::bytesRemain(){
	
	uint32_t usedBytes = head.numRec * head.recSize;
	usedBytes = usedBytes - sizeof(head); //don't forget the header takes up space!
	return FLASH_MAX_BYTES - usedBytes;
	
}

// =========================================================
// = limit                                                 =
// =========================================================
boolean MemoryLogger::limit(){
	
	if(bytesRemain() < head.recSize )
	{
		return true;
	}
	return false;
     
}

// =========================================================
// = printHeader                                           =
// =========================================================
void MemoryLogger::printHeader(){
	
	//writeHead();
	//readHead();
	
	Serial.print("Num of Records: ");
	Serial.println(head.numRec);
	
	Serial.print("Record Size in bytes: ");   
	Serial.println(head.recSize);
	
	Serial.print("current highest record address: "); 
	Serial.println(head.nextAddress); 
	
	Serial.print("total number of records: "); 
	Serial.println(head.totalRec);
	
	Serial.print("number of data uploads to PC: "); 
	Serial.println(head.uploadCount);
}


// =========================================================
// = readHead                                              =
// =========================================================
void MemoryLogger::readHead(){
	
	uint8_t dataBuffer[sizeof(head)]; //make a char array of the correct size. 
	flash.readArray(0UL,dataBuffer,sizeof(head)); //get the header as an array
	memcpy(&head, dataBuffer, sizeof(head)); //copy it to the struct.

}

// =========================================================
// = writeHead                                             =
// =========================================================
void MemoryLogger::writeHead(){
	
	uint8_t dataBuffer[sizeof(head)]; //make a char array of the correct size. 
    memcpy(dataBuffer, &head, sizeof(head)); //copy struct bytes to the array
	flash.writeArray(0UL, dataBuffer,sizeof(head)); //store the bytes

}

