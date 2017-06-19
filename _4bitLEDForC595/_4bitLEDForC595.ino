    unsigned char LED_0F[] = 
    {// 0	 1	  2	   3	4	 5	  6	   7	8	 9	  A	   b	C    d	  E    F    -
    	0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x8C,0xBF,0xC6,0xA1,0x86,0xFF,0xbf
    };
    const int numerOfDigits=4;
    unsigned char LED[4];	//用于LED的4位显示缓存
    int SCLK = 4;
    int RCLK = 3;
    int DIO = 2; //这里定义了那三个脚
    void setup ()
    {
      pinMode(SCLK,OUTPUT);
      pinMode(RCLK,OUTPUT);
      pinMode(DIO,OUTPUT); //让三个脚都是输出状态
    }
    void loop()
    {clearDigits();
    		DisplayNumber (8765);
       //delay(300);
    }

   void clearDigits(){
     for(int i=0;i<numerOfDigits;i++){
         int index=0;
         int x=2;
          index=ceil(pow(2,i));
         displayNumberAtIndex(0,index,false);
      }
    }

    void DisplayNumber(int number){
      for(int i=0;i<numerOfDigits;i++){
         int digitalNumer=number%10;
         int index=0;
         int x=2;
          index=ceil(pow(2,i));
         displayNumberAtIndex(digitalNumer,index,true);
         number=number/10;
      }
    }

   /* 显示或者关闭某一位
    *  number:index参数所在位显示的数字
    * index:第几位,位数起始位置是1，从右开始数
    * turnOn:显示或者关闭index所在位的LED
   */
    void displayNumberAtIndex(unsigned char number,int index,boolean turnOn){
    
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
          digitalWrite(RCLK,HIGH);
          digitalWrite(RCLK,LOW);
        }
    }
    
    void LED4_Display (void)
    {
    	unsigned char *led_table;          // 查表指针
    	unsigned char i;
    	//显示第1位
    	led_table = LED_0F + LED[0];
    	i = *led_table;
    	LED_OUT(i);			
    	LED_OUT(0x01);		
        digitalWrite(RCLK,LOW);
        digitalWrite(RCLK,HIGH);
    	//显示第2位
    	led_table = LED_0F + LED[1];
    	i = *led_table;
    	LED_OUT(i);		
    	LED_OUT(0x02);		
        digitalWrite(RCLK,LOW);
        digitalWrite(RCLK,HIGH);
    	//显示第3位
    	led_table = LED_0F + LED[2];
    	i = *led_table;
    	LED_OUT(i);			
    	LED_OUT(0x04);	
        digitalWrite(RCLK,LOW);
        digitalWrite(RCLK,HIGH);
    	//显示第4位
    	led_table = LED_0F + LED[3];
    	i = *led_table;
    	LED_OUT(i);			
    	LED_OUT(0x08);		
        digitalWrite(RCLK,HIGH);
        digitalWrite(RCLK,LOW);
        
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

    

