```
Name    : MemoryLogger                        
Author  : Noah Shibley                         
Date    : Sep 7th, 2013                                    
Version : 0.1                                              
Notes   : Manager data logging and retrieval for the arduino      
Depends : SST25VF flash memory library 
Hardware: SST25VF flash nor memory chip
```

### List of functions

```cpp
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
```
   
