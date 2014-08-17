/*
 * Software: Controlling a fan with an ultrasonic sensor
 * Date: 15/08/2014
 * Author: Cristóbal Medina López
 * Web: http://www.cristobalmedinalopez.es
 * License:
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
*/

const int ON=1, OFF=0;
const int RELAY=2, LED=13; //Relay PIN 2, LED PIN 13
const int TRIG=9, ECHO=10; //Trig PIN 9, Echo PIN 10

long cm=0; 
int current_state=OFF, lap=0;

int error_margin=2; 
int distance=65; //cm - maximum distance to fire the fan
int frequency=500; //milliseconds

void setup(){
  pinMode(RELAY, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  digitalWrite(RELAY, HIGH); //OFF
  Serial.begin(9600);
}

// blink LED
void alert(){
  digitalWrite(LED, HIGH);
  delay(500);
  digitalWrite(LED, LOW); 
}

//switch for the fan
void setFan(int state){
  if (state==ON){
    if (current_state==OFF){
      digitalWrite(RELAY, LOW); //ON
      current_state=ON;
      alert();
    }
  }else{
    if (current_state==ON){
      digitalWrite(RELAY, HIGH); //OFF
      current_state=OFF;
      alert();
    }
  }
}

long ranging(){
  long duration;
  
  // LOW pulse beforehand to ensure a clean HIGH pulse.
  digitalWrite(TRIG, LOW); 
  delayMicroseconds(2);
  
  // A HIGH pulse >=2 ms to trigger a ping
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  
  duration = pulseIn(ECHO, HIGH);
  
  // The speed of sound is 340 m/s (29 ms/cm).
  // The object distance is half of the total distance.
  return duration / 29 / 2;
}



void loop(){
  //Get distance
  cm=ranging(); 
  Serial.println(cm);
  
  if (cm<=distance){
    lap++;
  }else{
    lap--;
  }
  
  if (lap>=error_margin){
    lap=error_margin;
    setFan(ON);
  }
  
  if (lap<=-error_margin){
    lap=-error_margin;
    setFan(OFF);
  }  
  
  delay(frequency);
}



