###########################################################################

                          ** Sound Sculpture **
                                March 2021
	
###########################################################################

################################# Preview #################################
Using Arduino, Rhino & Grasshopper we attempt to sculpture a virtual object with sound.
We let the user interact by using there oune voice to mold a three dimensional shape on screen.
The interface Starts as a simple ball and changes as the user sings to the microphone.
A shake of the Physical object will save the shape the user already made on the screen and start a new shape.

for This project we used:
 - Arduino
 - Arduino microphone
 - arduino accelerometer
 - Rhino - Rhinoceros 3D
 - Grasshoper.app (with additional Plug-Ins)
 - 3d printer
 
This project was biuld as a the final project in corse Introduction to
Computer Interaction.

for the projects video please see: **NOT AVLABLE YET**
###########################################################################

############################## Virtual Agent ##############################
The virtual agent is reviewed by Rhino using Grasshoper Plug-in.
The user Starts with a simpe mash spher with controle points.
When a sound is made eatch point is chosen by it's frequence and move acording to the magnitude of the sound.

This progam uses gasshope plig-ins:
 - Firefly
 - HoopSnake
 - Elefront
 - Weavebird

For mor info see the code "grasshopper design.gh"
 
##########################################################################

################################# Arduino #################################
Foe this projectwe used:
 - Arduino UNO
 - uxcell Microphone Sound Sensor Voice Detection
 - Arduino MPU-6500 accelerometer
 
To view the conected circuitד see the attached "circuit.jpg" file.

The arduino code is attached.
In a few words, It takes sound samples from the microphone and with ArduinoFFT provides the most friquent sound and the magnitudes of all the frquences in the set range.
In addtion to that the accelerometer ample in the main loop and conulation is done providing information on shaking gesture the user makes.
For more, you are wellcome to view the code, attaches as "fft_eccelo_arduino_project.ino"
###########################################################################

############################# Physical object #############################
the object is a microphone printed in 3D printer.
A STL file with the oblect  is attached under "Physical object" Folder.

We chose the object to apper in the shape of a piece of clay a hand has held.
In addition the holse of the mic are in the shape of a fingerprint.
This choice came to promote the feeling of holding and molding clay.
###########################################################################

