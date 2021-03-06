#include <AFMotor.h>
#include <Servo.h>

AF_DCMotor motor(1);
Servo servo;
int trigPin = 14;
int echoPin = 15;
long current_center_distance,current_left_distance,current_right_distance;
int right_angle,left_angle,center_angle,current_angle,vehicle_speed,check_distance;


// initialized two variables for distance and the angle current angle is set to 90 
// tigger pin and echo pin are the unutilized pin of the motor shield pin 14 and 15
void setup()
{
  Serial.begin(9600);
  // initialize the angles according to your vehicle design
  right_angle = 45;
  left_angle = 135;
  center_angle = 90;


  // distance to be checked for
  check_distance = 30 ;

  // initialize pins 
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // initialize vehicle speed
  vehicle_speed = 100 ;

  // attach servo motors
  servo.attach(9);

  // final setup for motor
  current_angle = center_angle;
  motor.setSpeed(vehicle_speed);
  motor.run(RELEASE);
  initial_check();

  if(current_center_distance < check_distance || current_left_distance < check_distance || current_right_distance < check_distance )
  {

    current_center_distance = current_left_distance = current_right_distance = 0;

  }
  else if(current_center_distance > current_right_distance && current_center_distance > current_left_distance && current_center_distance > check_distance)
  {
    current_right_distance = current_left_distance = 0 ;
  }
  else if( current_left_distance > current_center_distance && current_left_distance > current_right_distance && current_left_distance > check_distance)
  {
    current_center_distance = current_right_distance = 0;
  }
  else if( current_right_distance > current_center_distance && current_right_distance > current_left_distance && current_right_distance > check_distance )
  {
    current_center_distance = current_left_distance = 0 ;
  }



}


// main code 
void loop()
{

  if(current_center_distance == 0 && current_right_distance == 0 && current_left_distance == 0 )
  {
    while(current_center_distance < check_distance + 10 && current_right_distance < check_distance + 10 && current_left_distance + 10)
    {
      move_backward();
      hold();

      cal_distance_right();
      delay(1000);
      cal_distance_left();
      delay(1000);
      cal_distance_center();
      delay(1000);

    }
    hold();
    cal_distance_left();
    delay(1000);
    cal_distance_right();
    delay(1000);

    if( current_left_distance > current_right_distance )
    {
      servo_left();
      motor.run(FORWARD);
      delay(1000);
      hold();
      servo_center();
      delay(1000);
    }
    else if(current_right_distance > current_left_distance)
    {
      servo_right();
      motor.run(FORWARD);
      delay(1000);
      hold();
      servo_center();
      delay(1000);
    }

  }
  else if(current_right_distance == 0 && current_left_distance == 0)
  {
    move_forward();
  }
  else if (current_center_distance == 0 && current_right_distance ==0 )
  {

    servo_left();
    motor.run(FORWARD);
    delay(1000);
    hold();
    initial_check();
  }
  else if( current_center_distance == 0 && current_left_distance ==0 )
  {
    servo_right();
    servo_left();
    motor.run(FORWARD);
    delay(1000);
    hold();
    initial_check();
    hold();
    initial_check();
  }


  cal_distance_center();

  if(current_center_distance > check_distance)
  {
    current_right_distance = current_left_distance = 0 ;
  }
  else
  {
    hold();
    initial_check();

    if(current_center_distance < check_distance || current_left_distance < check_distance || current_right_distance < check_distance )
    {

      current_center_distance = current_left_distance = current_right_distance = 0;

    }
    else if(current_center_distance > current_right_distance && current_center_distance > current_left_distance && current_center_distance > check_distance)
    {
      current_right_distance = current_left_distance = 0 ;
    }
    else if( current_left_distance > current_center_distance && current_left_distance > current_right_distance && current_left_distance > check_distance)
    {
      current_center_distance = current_right_distance = 0;
    }
    else if( current_right_distance > current_center_distance && current_right_distance > current_left_distance && current_right_distance > check_distance )
    {
      current_center_distance = current_left_distance = 0 ;
    }

  }



}



// calculates the distance according to the reflection of sound with respect to time
// returns a long variable with the distance calculated
long cal_distance()
{
  Serial.println("Distance");
  long distance,duration;
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(1000);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH); // calculating the distance with respect to duration of sound released by trigger
  distance = (duration/2)/29.1;
  Serial.println(distance);
  return distance;
}

// stop the venhicle
void hold()
{
  motor.run(RELEASE);
  delay(2000);
}


// move the vehicle forward   
void move_forward()
{
  Serial.println("Moving Forward");
  motor.run(FORWARD);
  delay(200);
}
// move the vehicle backward
void move_backward()
{
  Serial.println("Moving Backward");
  motor.run(BACKWARD);
  delay(500);

}

// checks the directions before the vehicles operates
void initial_check()
{
  Serial.println("initial checking...");

  cal_distance_left();
  delay(1000);
  cal_distance_right();
  delay(1000);
  cal_distance_center();
  delay(1000);


}

// moves servo to the left 
void servo_left()
{
  Serial.println("moving servo left");
  current_angle = left_angle;
  servo.write(left_angle);

}

// moves servo to the right
void servo_right()
{
  Serial.write("moving servo right");
  current_angle = right_angle;
  servo.write(right_angle);
}

// move servo to center
void servo_center()
{
  Serial.println("moving servo center");
  current_angle = center_angle;
  servo.write(center_angle);
}

// calculate the left distance
long cal_distance_left()
{
  Serial.println("calculating left distance");
  servo_left();
  current_left_distance = cal_distance();
}

// calculate the central distance
long cal_distance_center()
{
  Serial.println("calculating the central distance");
  servo_center();
  current_center_distance = cal_distance();
}


// calculate the right distance
long cal_distance_right()
{
  Serial.println("calculating the right distance");
  servo_right();
  current_right_distance = cal_distance();
}










