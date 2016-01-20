Install GLUT before running:

Make a folder called c:\glut and download from the following link: http://ivl.calit2.net/wiki/images/0/0a/Glut-3.7.6-bin.zip

Open c:\glut and right click on glut-3.7.6-bin.zip and select Extract Here.

Open the new folder (glut-3.7.6-bin) and verify that you see the following five files: glut.def, glut.h, glut32.dll, glut32.lib, README-win32.txt

Move glut32.dll to c:\Windows\System32 (or SysWOW64 for 64bit) so that all programs can access it. If you don't have write privileges for this directory, you can put the .dll file into the directory your executable (.exe) resides in. This is typically your Debug or Release directory, just below your project directory.

Create a new directory under c:\glut\glut-3.7.6-bin and name it "GL". Move glut.h to this new directory (should be "c:\glut\glut-3.7.6-bin\GL").

.exe download: https://www.dropbox.com/sh/oixph79cdhy7kb3/AAArzKeyR1vcg30SECVrLmR9a?dl=0

Controls:

1 - toggle toon shader

2 - toggle per-pixel illumination

3 - toggle bump mapping

e - switch between spaceship mode and camera mode

t - generate new plant

r - reset camera and spaceship position

x - toggle hyper speed

q - exit application

spacebar - stop spaceship movement

**Camera Mode**

w - move forward

a - strafe left

s - move backward

d - strafe right

left-click + mouse motion - look around

**Spaceship Mode**

w - speed up

a - turn left (yaw)

s - slow down

d - turn right (yaw)

left-click + mouse motion - roll/pitch
