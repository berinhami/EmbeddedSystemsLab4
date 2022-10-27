# EmbeddedSystemsLab4
The objective of this lab is to learn the basics of IoT by connecting multiple arduino devices over a MQTT broker.
Building a state machine to design how the system will communicate with its components and the other systems linked to the broker.
1 system that lights up different LEDs based on requests that are sent from system #2, which uses a sensor to record the distance of an object and then passes requests to system 1 based on that distance:
Red to signal stop approaching the sensor
Yellow to signal you are getting to destination distance
Green to signal keep approaching the sensor
Flashing red to signal the need to backup from the sensor
1 system to report when a door is open and closed using a magnet sensor. This system will convey the status to the second system which will then either go on or off depending on the state of the door.
1 MQTT broker which will handle all the messages being sent and received between the systems.
