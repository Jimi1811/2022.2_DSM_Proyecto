
#include <Servo.h>

Servo servo1;

int servo = 11;
int MAXPULSE = 2200;
int MINPULSE = 800;

#define MOTOR_FREQUENCY 1000   //hz

#define PMW_M  5
#define DIR_M  6
#define ENC_IN_RIGHT_A 2
#define ENC_IN_RIGHT_B 3
#define puls 7
#define done 12
#define DT 8
#define SCK 9

float right_motor_ref = 0;
int right_wheel_tick_count = 0;
int last_right_wheel_tick_count = 0;
float right_enc_w = 0;
int valor;

void setup() 
{
  pinMode(PMW_M, OUTPUT);
  pinMode(DIR_M, OUTPUT);
  pinMode(done, OUTPUT);
  pinMode(puls, INPUT);

  pinMode(DT,INPUT);
  pinMode(SCK,OUTPUT);
 
  digitalWrite(PMW_M, LOW);     // MOTOR
  digitalWrite(DIR_M, LOW);
  digitalWrite(done, LOW);

  attachInterrupt(digitalPinToInterrupt(ENC_IN_RIGHT_A), right_wheel_tick, RISING);

  servo1.attach(servo);
  servo1.write(50);

  Serial.begin(9600);
}

void loop() 
{
  if(digitalRead(puls))
  {
    digitalWrite(done, LOW);
    
    servo1.write(70);
    //while (!(galga()<30000)){}    
    delay(1000);
    servo1.write(50);
    
    delay(500);

    right_motor_ref = -14;
    motor_controller();
    Serial.println("Fin primer control");
    delay(3000);
    right_motor_ref = 0;
    motor_controller();

    digitalWrite(done, HIGH);

    valor = galga();

    //Serial.write(medicion);
    //Serial.println((valor-5000.0)*100.0/15000.0);
    Serial.println(valor*100/20000);
  }
}



void motor_controller()
{
    bool flag = 0;

    unsigned long  oldtime_motor = 0;
    unsigned long  newtime_motor;
    
    float motor_control = 0;
    
    float input = 0;
    float right_motor_err = 0;
    
    float last_input = 0;
    float last_right_motor_err = 0;
    
    unsigned long tTime[4];
    
    int ENCODER_FREQUENCY = 500;
    float delta_tiempo = 0;
    
    float vel_motor = 0;
    float der = 0; float inte = 0; float pro = 0;
    float kd = 0; float kp = 5; float ki = 0;
    
    float err_integrativo = 0;
    float u = 0;

    bool set_dir = 1;

    Serial.println(right_enc_w);
    
    while(!flag)
    {
      unsigned long t = micros();

      if ((t - tTime[0]) >= (1000000 / ENCODER_FREQUENCY))
      {
        // Encoder
        
        newtime_motor = micros();
      
        delta_tiempo = 1.0/ENCODER_FREQUENCY;
        right_enc_w = (right_wheel_tick_count - last_right_wheel_tick_count)/delta_tiempo;
        vel_motor = right_enc_w*(2.0*PI)/(1000);
        last_right_wheel_tick_count = right_wheel_tick_count;
  
        // Control

        right_motor_err = right_motor_ref - right_wheel_tick_count*(2.0*PI)/(1000);

        der = kd*(right_motor_err - last_right_motor_err)/delta_tiempo;        
        pro = kp*right_motor_err;
        err_integrativo = err_integrativo + right_motor_err*delta_tiempo;        
        inte = ki*(err_integrativo);
      
        u = pro + der + inte;
      
        motor_control = (u*255.0)/7.9;
        //Serial.println("senal");
        //Serial.println(motor_control);
        //Serial.println("error");
        //Serial.println(right_motor_err);

        last_right_motor_err = right_motor_err;
        Serial.println(right_motor_err);
  
        // Motor

        

        if (abs(right_motor_err) < 0.1)
        {
           digitalWrite(DIR_M, LOW);
           digitalWrite(PMW_M, LOW);

           flag = 1; // SALIR DEL LOOP
        }
        else
        {
          if (motor_control == 0) 
          {
            digitalWrite(PMW_M, LOW);
            //Serial.println(1);
          }
          else if (motor_control > 0 and motor_control <= 255) 
          {
            digitalWrite(DIR_M, set_dir);
            analogWrite(PMW_M, motor_control);
            //Serial.println(2);
          }
          else if (motor_control > 255) 
          {
            digitalWrite(DIR_M, set_dir);
            digitalWrite(PMW_M, HIGH);
            //Serial.println(3);
          }
          else if (motor_control < 0 and motor_control >= -255) 
          {
            digitalWrite(DIR_M, !set_dir);
            analogWrite(PMW_M, abs(motor_control));
            //Serial.println(4);
            //Serial.println(abs(motor_control));
            //Serial.println(t);
          }
          else if (motor_control < -255) 
          {
            digitalWrite(DIR_M, !set_dir);
            digitalWrite(PMW_M, HIGH);
            //Serial.println(5);
          }
        }

        oldtime_motor = newtime_motor;
        tTime[0] = t;
      }
    }
}

// Increment the number of ticks
void right_wheel_tick() {
   
  // Read the value for the encoder for the right wheel
  int val = digitalRead(ENC_IN_RIGHT_B);
 
  if(val == LOW) {
  right_wheel_tick_count++;
  }
  else {
  right_wheel_tick_count--;
  }
   
}

// Medición de galga, promedio de 20 muestras
float galga()
{
  long int promedio = 0;
  int num = 20;
  long int medicion = 0;
  
  for (int i2 = 0; i2 < num; i2++)
  {
    medicion = 0;
    while(digitalRead(DT))
    {}
    
    for(int i = 0; i < 25; i++)
    {
      digitalWrite(SCK,true);
      delay(0.04);
      if(digitalRead(DT))
      {medicion++;}
      medicion = medicion<<1;
      digitalWrite(SCK,false);
      delay(0.004);
    }

    promedio = promedio + medicion;
    delay(1);
  }

  return promedio/num;
}
