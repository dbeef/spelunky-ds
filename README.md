<h1 align="center"> Spelunky® remake for the NintendoDS. </h1>
<p align="center">(a.k.a SpelunkyDS)</p>

<p align="center">
	<a href="https://travis-ci.org/dbeef/spelunky-ds"><img src="https://api.travis-ci.org/dbeef/spelunky-ds.svg?branch=master" alt="Travis build status icon"></a>
	<a href="https://gbatemp.net/threads/spelunkyds.507192/#post-8044792"><img src="https://img.shields.io/badge/GBATemp-Thread-blue.svg"></a>
</p>

<p align="center">
	<img src="https://github.com/dbeef/spelunky-ds/blob/master/readme/gameplay.gif" alt="Works on real NDS!" width="240" height="340">
	<img src="https://github.com/dbeef/spelunky-ds/blob/master/readme/n3dsxl.gif" alt="Works on real N3DSXL!" width="240" height="340">
</p>

<h2 align="center">To-do list as for now</h2>
- ✔	Implement shopkeeper logics and randomly spawned shops
- ✔ Implement damsel
- ✔ Implement caveman
- ✔ Implement skeleton
- ✔ Implement arrow trap
- ✔ Fix rope logics - if thrown, when it touches bottom of some tile above it or it reaches its maximum travel distance of 8 tiles up, then it should expand to the bottom. Now it's the opposite - it expands as it travels upwards.
- ✔ Implement cape and jetpack
- 🗙 Putting project on Travis
- ✔ Implement 4 layers system - top one for the console and HUD, bottom one for tilemap and the rest for other creatures
- 🗙 Tune-up the level generator; make it more random; add random events like snake well
- 🗙 Implement music using streaming from filesystem - [I elaborated a bit on this on my blog](https://dbeef.wordpress.com/2018/05/24/some-words-on-developing-spelunkyds/)
- 🗙 Pausing
- 🗙 Jars dropping loot
- 🗙 Ghost when spending too much time on level
- 🗙 Shake screen when dropping a bomb
- 🗙 Add github link to the main menu's copyrights sprite - so people would find the current version even if it was downloaded from some other source
- 🗙 Attempt to make levels a bit bigger (current size is 512x512, what I can probably make is 1024x512 or 512x1024, though not sure of that)

Obviously I omit things like implement Olmec or implement lava city - let's finish cave level first.

<h2 align="center">Known bugs</h2>
- 🗙 Though overall randomness of the generator is improved it sometimes makes some ugly structures, to be fixed later
- 🗙 I messed up a bit with collision detection
- 🗙 Arrow trap is triggered even if there's a tile (or tiles) between some moving object and the trap
- 🗙 Once thrown chest can't be opened
- ✔ Sometimes there's a level where there are spikes just hanging in air
- 🗙 Sometimes you can loose your items (shotgun, pistol) when changing levels
- 🗙 UI bugs - you can throw rope / drop bomb in scores screen
- 🗙 Damage system appears to be broken in terms of shopkeeper (you can't throw arrows, rocks, jars, etc on him)
- ✔ There's a bug when carrying damsel while sprinting through spikes
- 🗙 There's a bug when jumping on more than one NPC (snake, bat, etc) at the same time
- 🗙 Displaying loot in level-transition screen
- 🗙 Shopkeeper is quite dumb, easy to steal
- 🗙 Bullets can collide with jetpack, so it can work as a bulletproof vest
- 🗙 Blood drops / bullets can trigger arrow trap
- 🗙 Game tends to crash after some time of playing (at least on my vanilla DS) 

<h2 align="center">Building</h2>
In order to compile this application on your own, you will need [devkitPro](https://devkitpro.org/) with the devkitARM toolchain, plus the necessary tools and libraries. devkitPro includes `dkp-pacman` for easy installation of all components:

```
 $ dkp-pacman -Syu devkitARM general-tools dstools ndstool libnds libfat-nds
```

Once everything is downloaded and installed, `git clone` this repository, navigate to the folder in which it was cloned, and run `make` to compile the application. If there is an error, let us know.

<h2 align="center"> Debugging </h2>
To compile SpelunkyDS with assertions fired up, type "make DEBUG=1" when compiling.
Debugging seemingly random crashes is possible only on an emulator, which will provide last executed line:

<p align="center">
<img src="https://github.com/dbeef/spelunky-ds/blob/master/readme/desmume_output.png" alt="Desmume output"
 width="489" height="30">
</p>
 
<p> Calling <b> addr2line </b>: </p>


<p align="center">
<img src="https://github.com/dbeef/spelunky-ds/blob/master/readme/debugging.png" alt="Calling addr2line"
 width="670" height="55">

<p> What would be nice to add is printing to emulator's console for debugging purposes. 
 libnds ships such function calls (nds/arm9/console.h), but they work only on an emulator called nocash, which is Windows only. </p>
<p> Please contact me if you know any Linux NDS emulator that supports printing to terminal via nocash messages or you know how to log to desmume. </p>



</p>

<h2 align="center">Tools I use</h2>

- C++
- GNU Make </li>
- libnds - as included with devkitPro's toolchain https://devkitpro.org/
- mappy - https://tilemap.co.uk/mappy.php
- desmume emulator, for testing and debugging
- R4 card for testing on a real Nintendo DS Hardware
- gimp, for converting to 256 color (8bit) indexed bmp/png </li>
- [The Spelunky source code](http://www.derekyu.com/games/spelunky_1_1_src.zip) (distributed by Derek Yu) for inspiration
- [Spelunky assets](https://github.com/tyrovc/SpelunkyCommunityUpdateProject), which are taken from the Spelunky Community Update Project (because there's no need for installing Game Maker)
- CLion for editing

<h2 align="center"> License </h2>

<p> Full license text can be accessed under this link: </p>
https://github.com/dbeef/spelunky-ds/blob/master/LICENSE
<p>
Briefly, as for my code, I distribute it under GNU General Public License, as for Spelunky game assets, I use them under Spelunky® User License v1.1b, also, libnds and tools for building .nds files are provided by devkitARM;
devkitARM is constructed from and contains components licensed under GPL, BSD, zlib, LGPL & MPL licenses.
libnds is zlib licensed.

NintendoDS is a trademark of Nintendo and is used purely for descriptive purposes. This game is not affiliated with Nintendo.
</p>


<h2 align="center">Contributing</h2>
I will accept every help with running this project, feel free to catch one or more of the todo's.

<h2 align="center">Links</h2>
I found the following materials very helpful:

- [An introduction manual to Nintendo DS programming](https://patater.com/files/projects/manual/manual.html)
- http://cpp4ds.blogspot.com/
- [A Youtube video series about the Spelunky level generator](https://www.youtube.com/watch?v=ouh7EZ5Qh9g)
- [An online demonstration of the Spelunky level generator](http://tinysubversions.com/spelunkyGen/)
- A blog entry on lirauna.com about [alpha blending](http://www.liranuna.com/nds-blending-demo/)
- [The libnds documentation](https://libnds.devkitpro.org)
- [The devkitPro forums](https://devkitpro.org)
- A blog entry on coranac.com about [Regular tiled backgrounds](https://www.coranac.com/tonc/text/regbg.htm)
- [memory access calculator](https://mtheall.com/vram.html)
- a post on stackexchange on [how to configure video modes to both print text and draw bitmaps](https://gamedev.stackexchange.com/questions/61065/using-ndslib-how-to-configure-video-modes-to-both-print-text-and-draw-bitmaps-o)
