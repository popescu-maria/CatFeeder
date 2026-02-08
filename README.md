# CatFeeder
Robotics project, automated cat feeder with interactive menu

My project is an automated cat feeder controlled by an Arduino. It allows users to schedule feeding times or time intervals through an interactive LCD menu, or you can set it to manual and press a button to feed the cat. The system will use an RTC module for accurate timekeeping, a servo motor to open or close the food hatch, and optional LEDs and a buzzer to alert the cat when food is served. The physical housing will be made from cardboard or similar lightweight material. The feeder dispenses food automatically without human intervention once configured. For complexity, I can also add a scale (load cell + HX711 amplifier) to measure the amount of food dispensed is as set from the menu.

 ### Components  

* **1 × Arduino UNO**   
* **1 × 16x2 LCD** (HD44780 or compatible)
* **1 × RTC Module**  (DS3231 or DS1307)
* **1 x Servo Motor**
* **3 x Push Buttons**
* **1 × EEPROM** (built-in) 
* **1 × Buzzer**
* ** Foamboard Housing **
* **Resistors and wiring as required**

<img src="Images/Cat_Feeder_with_cat.jpg" alt="Cat feeder in action." width="400">
<img src="Images/Cat_Feeder.jpg" alt="Cat feeder final look." width="400">

