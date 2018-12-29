
<h1 align="center"> Spelunky® remake for the NintendoDS. </h1><p align="center">(a.k.a SpelunkyDS)</p>

<p align="center">
<img src="https://github.com/dbeef/spelunky-ds/blob/master/readme/gameplay.gif" alt="Works on real NDS!"
 width="240" height="340">
<img src="https://github.com/dbeef/spelunky-ds/blob/master/readme/n3dsxl.gif" alt="Works on real N3DSXL!"
 width="240" height="340">

</p>
<h2 align="center">To-do list as for now</h2>
<ul>
 
<li> ✔	Implement shopkeeper logics and randomly spawned shops  </li>
<li> ✔ Implement damsel </li>
<li> ✔ Implement caveman </li>
<li> ✔ Implement skeleton </li>
<li> ✔ Implement arrow trap </li>
<li> ✔ Fix rope logics - if thrown, when it touches bottom of some tile above it or it reaches its maximum travel distance of 8 tiles up, then it should expand to the bottom. Now it's the opposite - it expands as it travels upwards. </li>
<li> ✔ Implement cape and jetpack </li>
<li> 🗙 Putting project on Travis </li>
<li> ✔ Implement 4 layers system - top one for the console and HUD, bottom one for tilemap and the rest for other creatures </li>
<li> 🗙 Tune-up the level generator; make it more random; add random events like snake well </li>
<li> 🗙 Implement music using streaming from filesystem - I elaborated a bit on this on my blog: https://dbeef.wordpress.com/2018/05/24/some-words-on-developing-spelunkyds/ </li> 
<li> 🗙 Pausing </li>
<li> 🗙 Jars dropping loot </li>
<li> 🗙 Ghost when spending too much time on level </li>
<li> 🗙 Shake screen when dropping a bomb </li>
<li> 🗙 Add github link to the main menu's copyrights sprite - so people would find the current version even if it was downloaded from some other source </li>
<li> 🗙 Attempt to make levels a bit bigger (current size is 512x512, what I can probably make is 1024x512 or 512x1024, though not sure of that) </li>


</ul>
 
 Obviously I omit things like implement Olmec or implement lava city - let's finish cave level first.

<p></p>
 <p> Development thread: </p>
 <p> https://gbatemp.net/threads/spelunkyds.507192/#post-8044792 </p>
 
<h2 align="center">Known bugs</h2>
<ul>
 
<li> 🗙 Though overall randomness of the generator is improved it sometimes makes some ugly structures, to be fixed later</li>
<li> 🗙 I messed up a bit with collision detection</li>
<li> 🗙 Arrow trap is triggered even if there's a tile (or tiles) between some moving object and the trap</li>
<li> 🗙 Once thrown chest can't be opened</li>
<li> ✔ Sometimes there's a level where there are spikes just hanging in air</li>
<li> 🗙 Sometimes you can loose your items (shotgun, pistol) when changing levels</li>
<li> 🗙 UI bugs - you can throw rope / drop bomb in scores screen</li>
<li> 🗙 Damage system appears to be broken in terms of shopkeeper (you can't throw arrows, rocks, jars, etc on him)</li>
<li> ✔ There's a bug when carrying damsel while sprinting through spikes</li>
<li> 🗙 There's a bug when jumping on more than one NPC (snake, bat, etc) at the same time</li>
<li> 🗙 Displaying loot in level-transition screen</li>
<li> 🗙 Shopkeeper is quite dumb, easy to steal</li>
<li> 🗙 Bullets can collide with jetpack, so it can work as a bulletproof vest</li>
<li> 🗙 Blood drops / bullets can trigger arrow trap</li>
<li> 🗙 Game tends to crash after some time of playing (at least on my vanilla DS) </li>

</ul>
 

<h2 align="center"> Building </h2>

<p> 1) Download devkitPro for your platform from their official website: https://devkitpro.org/wiki/Getting_Started </p>
<p> 2) Set up a few environment variables, as shown in their tutorial  </p>
<p> 3) cd to the spelunky-ds directory and type 'make'  </p>

<h2 align="center"> Running </h2>
<p> For running on a PC I preffer an emulator called Desmume. type 'desmume spelunky.nds' for running. </p>
<p> For running on an NDS I use 'R4i-3DS-RTS-WiFi' (oh boy) card, but it's probably going to work on any other card. Just make sure, you have NDS program called 'DSiMenu++', which helps running homebrew. Installation instructions:
 https://gbatemp.net/threads/ds-i-3ds-dsimenu-ds-i-menu-replacement.472200/

<h2 align="center"> Tools I use </h2>
<ul>
  <li> C++ </li>
  <li> GNU Make </li>
  <li> libnds - as included with devkitPro's toolchain https://devkitpro.org/ </li>
  <li> mappy - https://tilemap.co.uk/mappy.php </li>
  <li> desmume emulator </li>
  <li> R4 card for testing on a real Nintendo </li>
  <li> gimp, for converting to 256 color (8bit) indexed bmp/png </li>
  <li> a real Spelunky code for inspiration ofcourse, distributed by Derek Yu on this address: http://www.derekyu.com/games/spelunky_1_1_src.zip </li>
  <li> Spelunky assets, I take them from the Spelunky Community Update Project because there's no need for installing Game Maker: https://github.com/tyrovc/SpelunkyCommunityUpdateProject </li>
  <li> CLion for editing </li>
</ul>

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

<h2 align="center"> Links </h2>
I found following materials very helpful:

<ul> 
  <li> A great, great manual which introduced me to NDS programming: https://patater.com/files/projects/manual/manual.html</li>
  <li> http://cpp4ds.blogspot.com/ </li>
  <li> A great video series about Spelunky level generator https://www.youtube.com/watch?v=ouh7EZ5Qh9g</li>
  <li> This online demonstration for Spelunky level generator http://tinysubversions.com/spelunkyGen/ </li>
  <li> This blog, in particular this entry about alpha blending: http://www.liranuna.com/nds-blending-demo/ </li>
  <li> libnds documentation https://libnds.devkitpro.org </li>
  <li> devkitPro forum https://devkitpro.org </li>
  <li> this blog, that post in particular: https://www.coranac.com/tonc/text/regbg.htm </li>
  <li> memory access calculator: https://mtheall.com/vram.html </li>
  <li> this post: https://gamedev.stackexchange.com/questions/61065/using-ndslib-how-to-configure-video-modes-to-both-print-text-and-draw-bitmaps-o </li>
 
</ul>
