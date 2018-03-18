
#include<TimerOne.h>
int raw_data[300]; char sync[8]={'1','1','0','1','0','1','0','1'};
int index=0,flag=1,th=0,w=0;
char  tem;int flag_bit=1,ff=0;

void setup() {
 Serial.begin(9600);
 ADC_setup();                                   //enabling the internal registers for receiving data
 Timer1.initialize(1000);                      //taking data through the photodiode from LED every 1 ms
 Timer1.attachInterrupt(sample_signal_edge);

}

void loop() {
  int i=0;
  if(index==50&&flag_bit==1){
   for(i=1;i<=index;i++) 
     th+=raw_data[i];
  
  th=th/(index);
  Serial.println("Reciever is ready to receive data!!");
  //Serial.println(th);
  index=0;
  flag_bit=2;
  }
}

//if the sync bit is matched this function is called and the data is decoded 
void state_final(){
  int i=0,count,len=0,k=0,pos=0;
  int mul=128,value=0,m=8;
  int temp_store[32],temp_pos=0;
  pos=0;
  
  for(i=0;i<8;i++){
    if((raw_data[i]-th)>17){
     value+=mul;}
    mul/=2;
  }
  count=value;
  len=value;
  i=0;
  mul=128;
  while(count){
   mul=128;
   value=0;
   for(k=8+i;k<16+i;k++){
   if((raw_data[k]-th)>17)
     value+=mul;
    mul/=2;
   }
   temp_store[pos++]=value;
   i=i+8;
   count=count-1;
  }
  temp_pos=8+8*len;
  value=0;
  mul=128;
   for(k=temp_pos;k<temp_pos+8;k++){
   if((raw_data[k]-th)>17)
     value+=mul;
    mul/=2;
   }
 if(value==0x02){
  for(k=0;k<len;k++)
  Serial.write(temp_store[k]);
  Serial.print("\n");}
 else
  Serial.println("sorry!! ETX has not been received  correctly , please transmit data again!!");
 flag_bit=1;
 ff=0;
 index=0;
 w=0;
 flag=1;
}
void ADC_setup(){
  ADCSRA=bit(ADEN);//enabling ADEN port for receiver to take input
  ADCSRA |=bit(ADPS0) | bit(ADPS1) | bit(ADPS2);//prescaller of 128;
  ADMUX=bit(ADLAR);
  ADMUX=bit(REFS0) | bit(REFS1);//internal 1.1v reference
}


void sample_signal_edge(){
 bitSet(ADCSRA,ADSC);
 raw_data[index++]=ADC;
 if(raw_data[index-1]-th>17)
 //converting into binary encoded data in receiver 
  tem='1';
 else
  tem='0';
  
 if(flag_bit==4&&ff==0){
  if(index>273){
   ff=1;
   state_final();}}

  
 if(flag_bit==2){
  index=0;
 if(sync[w]==tem) {
     w++;
    if(w==8){
       flag_bit=4;
    }  
 }
 //the state machine for synchronizing with the sync bits
 else{
  switch (w) {
   case 2:
    w=1;
    break;
   case 3:
    w=0;
    break;
   case 4:
    w=2;
    break;
   case 5:
    w=0;
    break;
   case 6:
    w=2;
    break;
   case 7:
    w=0;
    break;
   case 0 :
     w=0;
     break;
   case 1:
     w=0;
     break;
   }
  }
  }
 
}
//Serial.println(index);

  


