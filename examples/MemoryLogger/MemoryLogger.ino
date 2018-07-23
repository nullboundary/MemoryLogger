#include <SPI.h>
#include <SST25VF.h>
#include <MemoryLogger.h>

#define MemCs 6 //chip select
#define MemWp 4 //write protection
#define MemHold 14 //hold

/*********************************************************/
union flexRecord
{
  
  struct{
    uint32_t index;   //record index
    char timeStamp[17]; //20130912T081226Z
    int pressure;
    int temperature;

  } record;
  
  uint8_t bytes[sizeof(record)];
  
} recData;
/*******************************************************/

MemoryLogger storage;



void setup(){
  Serial.begin(38400);
  storage.begin(MemCs,MemWp,MemHold,sizeof(recData));

}


void loop(){


  Serial.print("Creating records...");
  for (int recno = 0; recno <= 4; recno++)
  {
    int m = random(1, 12); //fake time data!
    int d = random(1, 31);
    int h = random(1, 12);
    int i = random(59);
    int s = random(59);

    recData.record.index = storage.recCount();
    sprintf(recData.record.timeStamp, "2013%02d%02dT%02d%02d%02dZ",m,d,h,i,s); //make the timestamp string
    recData.record.pressure = random(0,200); //make up some pressure value
    recData.record.temperature = random(-100, 100); //make a temp value

    storage.appendRec(recData.bytes); //save it to memory
//    Serial.println(recData.record.index);
//    Serial.println(recData.record.timeStamp);
//    Serial.println(recData.record.pressure);
//    Serial.println(recData.record.temperature);
//    Serial.println("-------Save End Record-------");
  }

  Serial.println("Reading Records....");
  recData.record.index = 0;
  sprintf(recData.record.timeStamp, "00000000T000000Z"); //make the timestamp string
  recData.record.pressure = 0;
  recData.record.temperature = 0; 

  for(int i=storage.recCount()-1;i>=0;i--)
  {
    storage.selectRec(i,recData.bytes);
    Serial.println(i); 
   
    Serial.println(recData.record.index);
    Serial.println(recData.record.timeStamp);
    Serial.println(recData.record.pressure);
    Serial.println(recData.record.temperature);
    Serial.println("-------End Record-------");

  }

  delay(200);
  Serial.println("--------------Header-------------");
  storage.printHeader();
  Serial.println("Clear All-----------------------");  
  storage.clearAllRec();
  delay(200);
}

