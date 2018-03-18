#include <TimerOne.h>
#define outled() DDRE=0x10;
char data[32],data_frame[35];
#define sync 0xD5;
#define etx 0x02;
int i=0,k=0;
int m=0;
int tem=0,len=0,flag=0;
int data_bit[560];
void setup()
{
  Serial.begin(9600); 
  outled();
  Timer1.initialize(1000);
  Timer1.attachInterrupt(send_data);
 // PORTE=0x10;
 }

void loop() {
  
  while(Serial.available())
  {
    if(i>32){
     Serial.println("frame size is too long");
     break;
    }
    data[i++]=Serial.read();
    //i=i+1;    
    k=0;
    m=0;
    len=i;
    //Serial.println(len);
    
  }
   
    
  //Serial.println(len);
  
 delay(1000);
  if(i>0){
   i=0;
   flag=0;
   create_frame(); 
  }
  
}
int p=0;
void send_data()
{    
     if(flag==0){
      if(k==1&&(m<8*(3+len)))
      {  
          if(data_bit[m]==1){
           // Serial.print("1");
            PORTE=0x10;}
           else{
           // Serial.print("0");
            PORTE=0x00;}
      //Serial.print(data_bit[m]);
           //Serial.println(p);
           m++;
           
      }
      
     
      if(m==8*(3+len))   
      {
        flag=1;
        i=0;
        k=0;
       }}

}

void to_manchester(char *data,int j)
{       
       int temp=0,tem=0;
       for(temp=0;temp<j;temp++)
       {
        for ( uint8_t bitMask = 128; bitMask != 0; bitMask = bitMask >> 1 ){ 
            if (data[temp] & bitMask ) {
                data_bit[tem++]=1;
                Serial.print("1");
               // data_bit[tem++]=0; 
               }
               else{
                data_bit[tem++]=0;
                Serial.print("0");
             //   data_bit[tem++]=1;
                
               }
               
        }
       }
 /*    PORTE=0x10;
       delay(1000);
       PORTE=0x00;
       delay(6);*/
       k=1;

    
}
void create_frame()
{ 
  //Serial.println(len);
  int v;
  data_frame[0]=sync;
  data_frame[1]=len;
  for(v=0;v<len;v++)
   data_frame[2+v]=data[v];
  data_frame[2+len]=etx;
  to_manchester(data_frame,3+len);
 
}

