# fastmeasurment

Simple Arduino sketch to read voltage in a useful manner for monitoring batteries and solar power circuits

Output works fine in serial plotter.

It reads A0 as fast as possible (with analogRead that is) and print an average over a cycle
of 250 milliseconds. It also prints maximum and minimum for this period
as well as the global max and min. 
