    const String SN="SP0002";
     int SCLK = 4;
    int RCLK = 3;
    int DIO = 2; //这里定义了那三个脚

    
    const int debounceDelay=10;//开头防抖延迟毫秒数
    const int swithDigit=8;//开关针脚
    bool initShow=true;
    String serialString="";
    bool readCompleted=false;

    int numberOfDisplay=0;
    unsigned char LED_0F[] = 
    {// 0	 1	  2	   3	4	 5	  6	   7	8	 9	  A	   b	C    d	  E    F    -
    	0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x8C,0xBF,0xC6,0xA1,0x86,0xFF,0xbf
    };
    const int numerOfDigits=4;
    unsigned char LED[4];	//用于LED的4位显示缓存
    unsigned char digits[]={0x01,0x02,0x04,0x08};
  
    void setup ()
    {
      Serial.begin(9600);
      pinMode(swithDigit,INPUT);
      digitalWrite(swithDigit,HIGH);
      
      pinMode(SCLK,OUTPUT);
      pinMode(RCLK,OUTPUT);
      pinMode(DIO,OUTPUT); //让三个脚都是输出状态

      numberOfDisplay=6789;
    }
    void loop()
    {
      if(readCompleted){
      hanldeMessageFromSerial(serialString);
      readCompleted=false;
      serialString="";
      }
      
      boolean switchValue=digitalRead(swithDigit);
      if(switchValue&&initShow){
        DisplayNumber (numberOfDisplay);
       }
       else
       {
//        if(!switchValue){
//          initShow=!initShow;
//         } 
          initShow=false;
          turnOff();
        }
        
    }
  void turnOn(){
    digitalWrite(swithDigit,HIGH);
    initShow=true;
   }
   void turnOff(){
     for(int i=0;i<numerOfDigits;i++){
         displayNumberAtIndex(1,digits[i],false);
      }
    }

    void DisplayNumber(int number){
      for(int i=0;i<numerOfDigits;i++){
         int digitalNumer=number%10;
         boolean t_on=true;
         if(number==0&&i>0){
            t_on=false;
          }
         displayNumberAtIndex(digitalNumer,digits[i],t_on);
         number=number/10;
      }
    }

   /* 显示或者关闭某一位
    *  number:index参数所在位显示的数字(十六进制)
    * index:第几位,位数起始位置是1，从右开始数
    * turnOn:显示或者关闭index所在位的LED
   */
    void displayNumberAtIndex(unsigned char number,unsigned char index,boolean turnOn){
    
      if(turnOn){
          unsigned char *led_table;          // 查表指针
          unsigned char i;
          led_table = LED_0F + number;
          i = *led_table;
          LED_OUT(i);     
          LED_OUT(index);    
          digitalWrite(RCLK,LOW);
          digitalWrite(RCLK,HIGH);
       }
       else
       {
          //digitalWrite(RCLK,HIGH);
          //digitalWrite(RCLK,LOW);
        }
    }
    
    void LED_OUT(unsigned char X)
    {
    	unsigned char i;
    	for(i=8;i>=1;i--)
    	{
    		if (X&0x80) 
                {
                  digitalWrite(DIO,HIGH);
                 }  
                else 
                {
                  digitalWrite(DIO,LOW);
                }
    		X<<=1;
                digitalWrite(SCLK,LOW);
                digitalWrite(SCLK,HIGH);
    	}
    }
//串口事件-用于读取串口消息
void serialEvent()
{
  while(Serial.available())
  {
    char inChar = (char)Serial.read();
    if(inChar != '\n')//以换行符作为读取结束标志
    {
      serialString += inChar;
    }
    else
    {
      readCompleted = true;
    }
  }
}

//处理来自串口的消息
/*
消息格式: 消息类型|消息内容
定义的消息类型列表:
1.turn_off(数码管熄灭),无消息内容
2.turn_on(数码管点亮)，无消息内容
3.SN(发送该设备的序列号给串口设备)，无消息内容
4.receive_order(接收订单)，消息内容格式为:订单号-产品编号/数量
5.test_number(显示测试数字),消息内容为数字
*/
void hanldeMessageFromSerial(String message){
  String msgType="";
  String msgBody="";
  int splitCharPosition=message.indexOf('|');
  if(splitCharPosition==-1){
    msgType=message;
   }
   else{
      msgType=message.substring(0,splitCharPosition);
      msgBody=message.substring(splitCharPosition+1,message.length());
    }
    if(msgType=="turn_off"){
      digitalWrite(swithDigit,LOW);
     }
     else if(msgType=="turn_on"){
      digitalWrite(swithDigit,HIGH);
     }
     else if(msgType=="SN"){
        Serial.println(SN);
     }
     else if(msgType=="test_number"){
        numberOfDisplay=msgBody.toInt();
        turnOn();
      }
      else if(msgType=="receive_order"){
         int orderPosition=msgBody.indexOf('/');
         String ordNo_P_No="";
         String ord_number="";
        if(orderPosition>=0){
            ordNo_P_No=msgBody.substring(0,orderPosition);
            ord_number=msgBody.substring(orderPosition+1,msgBody.length());
         }
         numberOfDisplay=ord_number.toInt();
         turnOn();
      }
      else
      {
        Serial.println(message);
      }
}
//开头防抖
boolean debounce(int pin){
  boolean state;
  boolean previousState;
  
  previousState=digitalRead(pin);
  for(int counter=0;counter<debounceDelay;counter++){
    delay(1);
    state=digitalRead(pin);
    if(state!=previousState){
        counter=0;
        previousState=state;
      }
    }
  return state;
}
    

