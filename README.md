# octo-sensor

OCTO-SENSOR is a system composed by 8 laser range-finder "sensor pods" synced to a central.

8 x SENSOR PODS:
Each of the sensor nodes are capable of controling animations on RGB LED strips, and send wirelessly it's information to the central while keeping in sync. 

1x CENTRAL:
The central generates the master sync signal that is broadcasted to all the sensors pods and it also receives their data. This data can be used to control interactions like sound effects and animation feedback. The central will also transmit all the 8 sensors info through OSC data over Wi-Fi IP network.

