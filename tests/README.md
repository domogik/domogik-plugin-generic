Purpose
=======

The scripts in this folder can simulate some external xpl clients.

temperature.py
--------------

This script can be used to check the temperature feature

* xPL source : arduino-temp.myarduino
* xPL message sent : ::

    self.send_xpl(schema = "sensor.basic", data = {"current" : 13, "device" : "tempsensor1", "type" : "temp"})

humidity.py
-----------

This script can be used to check the humidity feature

* xPL source : arduino-humidity.myarduino
* xPL message sent : ::

    self.send_xpl(schema = "sensor.basic", data = {"current" : 75, "device" : "humiditysensor1", "type" : "humidity"})

co2.py
------

This script can be used to check the co2 in ppm feature

* xPL source : arduino-co2.myarduino
* xPL message sent : ::

    self.send_xpl(schema = "sensor.basic", data = {"current" : 213, "device" : "co2sensor1", "type" : "co2"})

insolation.py
-------------

This script can be used to check the solar exposure (W/m2) feature

* xPL source : arduino-insolation.myarduino
* xPL message sent : ::

    self.send_xpl(schema = "sensor.basic", data = {"current" : 1050, "device" : "insolationsensor1", "type" : "insolationhumidity"})


pressure.py
-----------

This script can be used to check the pressure (Pa) feature

* xPL source : arduino-humidity.myarduino
* xPL message sent : ::

    self.send_xpl(schema = "sensor.basic", data = {"current" : 75, "device" : "pressuresensor1", "type" : "pressure"})


tvoc.py
-------

This script can be used to check the total volatile organic compound feature

* xPL source : arduino-tvoc.myarduino
* xPL message sent : ::

    self.send_xpl(schema = "sensor.basic", data = {"current" : 45, "device" : "tvocsensor1", "type" : "tvoc"})


rgb.py
------

This script can be used to simulate a rgb device

* xPL source : arduino-humidity.myarduino
* xPL message sent : ::

    self.send_xpl(schema = "arduino.rgb", data = {"command" : "setcolor", "device" : "parentsbedroom", "color" : "00ff00"}



