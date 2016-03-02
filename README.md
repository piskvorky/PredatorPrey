About
======

Predator&Prey is a little OpenGL game that shows emergent behaviour of independent agents.
In particular, it showcases herding of groups of animals, ala the [boid model](http://www.red3d.com/cwr/boids/), in the presence of predators.

The behaviour is not hardwired. Rather, the agents follow a simple set of rules (using a built-in rules scripting engine), letting the patterns in their behaviour emerge naturally.

There are two agents in the world:

* **Blue Prey**: graze leisurely; want to stay near each other; run away from predators.
* **Red Predators**: wander around; if hungry go hunting; if exhausted or well-fed, go to sleep.


![screenshot](http://radimrehurek.com/predatorprey.png)

Another, less visible but equally cool feature is the simulation itself.
Rendering (as in frames per second, FPS) is completely decoupled from the agent's AI thinking (builds per second, BPS).
You can render at 80 FPS, but still have the agents process their environment only once per second (or vice versa).

Also the simulation time is independent of the "real" time (your wall clock), so you can arbitrarily slow down/accelerate the simulation, or even freeze it.

Pro Tip
=======

View the world directly from above, zoom out and keep pressing `0` (random world reset) until you get a world with no predators.
Then increase the simulation speed to 1.0 and just watch the hordes of prey roam the world, making it barren (the grass they eat doesn't grow back).

**The boid model is just mesmerizing.** I could watch it for hours :)

![prey roam](http://radimrehurek.com/roam.png)

Controls
=========

`1`: decrease the rendering speed (default is 60 FPS, minimum 0 FPS)

`2`: increase FPS

`3`: decrease the build speed (default 40 BPS, min 0 BPS)

`4`: increase BPS

`5`: decrease simulation speed (default 0.3; minimum 0.0)

`6`: increase simulation speed

`0`: reset the world (random size, mountains, number of prey&predators)

`mouse`: change view+zoom in/out

`esc`: exit the application

Install
=======

Under **Linux or OS X**, just type `make`. Make sure you have the [OpenGL Utility Toolkit](http://www.opengl.org/resources/libraries/glut/) installed (OS X has it by default, in some Linuxes you have to `apt-get install libglut3-dev` or equivalent).

Under **Windows**, either use the VS project files in folder `build_windows` or just run the already-compiled executable there.
I also included `glut32.dll` in `build_windows`, in case your Windows didn't come with the [OpenGL Utility Toolkit](http://www.opengl.org/resources/libraries/glut/).

Tested on

* Ubuntu with gcc 4.4.3 (2011)
* OS X 10.5.8 with gcc 4.0.1 (2011)
* Windows XP with VS 7.0 (2004)

The demo was created using Microsoft Visual Studio 7.0. I don't have MVS anymore, so I gently hacked the code to compile under Linux and OS X as well (with a few `#ifdef`s) in 2011.

**I created this little demo ages ago (in 2004), when researching some AI game technologies. Don't expect any updates or support here.**
