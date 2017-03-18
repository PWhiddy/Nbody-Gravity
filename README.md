# Nbody Gravity Simulator
Implements the Barnes–Hut algorithim to handle the gravitional interaction of over a million particles in 3D space. Currently set up to generate disks of particles 
Includes a renderer that produces high quailty PPM images of the simulation. These can be compiled into animations. [Example](https://goo.gl/uUEgHl)

# Setup
Use build.bash to create the program. That's it! Executing "run" will immediately start simulating and rendering to the images folder until canceled or frame limit is reached. Note: This may consume significant storage space.

Initial conditions parameters can be modified in the Constants.h file, and are baked into the program so build must be run again after making changes. 
If you have ffmpeg (if not `sudo apt-get install ffmpeg`) you can use createVideo.bash to generate a video from the images.

# Images

Initialized as a uniform disk
![alt tag](http://i.imgur.com/KarubZw.png?1)

Higher disk mass
![alt tag](http://i.imgur.com/om7hTpE.png?1)

"Hot" Disk
![alt tag](http://i.imgur.com/UVLfwuN.png)

# Outtakes 
Many interesting things happened as I frequenty broke my code
![alt tag](http://i.imgur.com/TBJekOz.png)
![alt tag](http://i.imgur.com/XblJuai.png)
![alt tag](http://i.imgur.com/HPZvu8b.png)
![alt tag](http://i.imgur.com/PkBY0PF.png?1)
