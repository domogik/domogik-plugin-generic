Purpose
=======

The scripts in this folder can simulate some external xpl clients.

temperature.py
--------------

This script can be used to check the temperature feature

* xPL source : arduino-temp.myarduino
* xPL message sent : ::

    self.send_xpl(schema = "sensor.basic", data = {"current" : 13, "address" : "tempsensor1", "type" : "temp"})

humidity.py
-----------

This script can be used to check the humidity feature

* xPL source : arduino-humidity.myarduino
* xPL message sent : ::

    self.send_xpl(schema = "sensor.basic", data = {"current" : 75, "address" : "humiditysensor1", "type" : "humidity"})

