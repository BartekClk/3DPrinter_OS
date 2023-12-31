#include <Arduino.h>

class Pin{
  protected:
    byte pin;
    String mode;
  public:
    Pin(byte pin, String mode){
      this->pin = pin;
      this->mode = mode;
    }
    byte getPin(){return pin;}
};

class Fan{
  protected:
    int speed = 0;
    Pin *pin;

  public:
    Fan(byte pin, String pinMode){
      this->pin = new Pin(pin, pinMode);
    }
    byte getPin(){return pin->getPin();}
    void setSpeed(int speed){
      this->speed = speed;
    }
};

class Heater{
  protected:
    int actualTemp = 10;
    int wantedTemp = 60;
    bool isHeating = 0;
    byte Pin;
    byte defState = 0;

  public:
    Heater(byte Pin){
      this->Pin = Pin;
      pinMode(this->Pin, OUTPUT);
      digitalWrite(this->Pin, HIGH);
    }

  int getTemp(bool a=0){ // 0 = actualTemp, 1 = wantedTemp;
    switch(a){
      case 0:
        return actualTemp;
        break;
      case 1:
        return wantedTemp;
        break;
    }
  }

  bool getIsHeating(){return isHeating;}
  byte getPin(){return Pin;}

  void def(){
    // pinMode(Pin, )
    if(defState == 0);
  }
};

class HeatBed : public Heater{
  private:
  public:
    HeatBed(byte transistorPin) : Heater(transistorPin){}
};

class HotEnd : public Heater{
  private:
    float AxisPosition[3] = {0,0,0}; //x,y,z
    float AxisPositionHome[3] = {0,0,0}; //x,y,z
    bool positionTrusted = false;
    bool levelingAfterHome = true;
  public:
    HotEnd(byte transistorPin) : Heater(transistorPin){}
};

class Limiter{
  private:
    String name;
    bool status = 0;
    byte read_pin;
  public:
    Limiter(byte read_pin){
      this->read_pin = read_pin;
      name = "unnamed";
      }
    
    Limiter(byte read_pin, String name){
      this->read_pin = read_pin;
      this->name = name;
    }

    void checkStatus(){
        switch(digitalRead(read_pin)){
          case HIGH:
            status = 1;
            break;
          case LOW:
            status = 0;
            break;
        }
    }

    bool getStatus(){return status;}
    String getName(){return name;}
    byte getPin(){return read_pin;}
};

class Printer{
  private:
    HeatBed *heatBed = new HeatBed(22);
    HotEnd *hotEnd = new HotEnd(23);
    Fan *hotEndFan = new Fan(2, "OUTPUT");
    int printSpeed = 1;
    bool absoluteExtrusion = true;
    bool imperialUnits = false;
    bool absolutePositioning = true;
  public:
    Limiter *endStops[3][2] = {
      {}, //x dimmension
      {}, //y dimmension
      {}  //z dimmension
    };

    Printer(){

    }

  void setEndStops(byte a[]){
    endStops[0][0] = new Limiter(a[0], "y-front");
    endStops[0][1] = new Limiter(a[1], "y-back");

    endStops[1][0] = new Limiter(a[2], "x-left");
    endStops[1][1] = new Limiter(a[3], "x-right");

    endStops[2][0] = new Limiter(a[4], "z-top");
    endStops[2][1] = new Limiter(a[5], "z-bottom");
  }

  int getBedT(bool a=0){return heatBed->getTemp(a);}
  int getHotEndT(bool a=0){return hotEnd->getTemp(a);}
  int getPrintSpeed(){return printSpeed;}
  bool getendStopstatus(byte row, byte collumn){return endStops[row][collumn]->getStatus();}
};

Printer *printer = new Printer();

HeatBed *a = new HeatBed(22);

void setup() {
  byte endStopsPins[6] = {26,27,28,29,30,31};
  printer->setEndStops(endStopsPins);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
}
