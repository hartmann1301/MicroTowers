# Micro Towers
This is a Tower Defense game I programmed for Arduboy.

![][gifCampain]  ![][gifEditor] 

Thanks to this https://felipemanga.github.io/ProjectABE/ great Emulator I could make those gifs.

[gifCampain]: https://github.com/hartmann1301/MicroTowers/blob/master/gifCampain.gif
[gifEditor]: https://github.com/hartmann1301/MicroTowers/blob/master/gifEditor.gif

# Controls
This picture should explain how to navigate and play in the game.

![][explainControls]    

[explainControls]: https://github.com/hartmann1301/MicroTowers/blob/master/Extras/explainControls.png

Editor Pro Tip: if you long press A in the DELETE mode the whole map will be cleared

maybe there are also some hidden functions 

# Explaining Pictures
If you wanna know what is happening on this tiny screen the following pictures might help you.

```diff 
- 0   this is a tower you already build
- 1   a stone, you can not build there but also enemies can not walk there
- 2   a big building, same as the stone, but looks different 
- 3   your headquarter, defend it with some more towers!
- 4   the cursor on the magic invisible 6x6 pixels raster
- 5   the entry, this is where the enemies will come from
- 6   coins you currently have, maximum is 999
- 7   lifepoints of your headquarter, if it is zero the match is over
- 8   Next wave type and number, there will be 30 waves and 6 different types
- 9   a tree, you can not build there but enemies can walk there
```
![][explainPicture1]         ![][explainPicture2]

```diff 
+ 0   an enemy that is trying to walk to your hq
+ 1   a support tower to boost those two towers next to it
+ 2   tower menu SELL, press A to sell a tower and get some money back
+ 3   tower menu INFO, see boost or press A to get to tower lexicon
+ 4   tower menu UPGRADE, press A to upgrade a tower if you can afford it
+ 5   the range of the currently selected tower
+ 6   the name of the currently selected menu index, press UP or DOWN to change
+ 7   a railgun projectile flying toward an enemy
+ 8   what the upgrade to the next level will cost you
+ 9   fast mode is active, press long A to toggle it
```
[explainPicture1]: https://github.com/hartmann1301/MicroTowers/blob/master/Extras/explainPicture1.png
[explainPicture2]: https://github.com/hartmann1301/MicroTowers/blob/master/Extras/explainPicture2.png

# Maps
In the maps list you can choose a map you wanna play. You start with 5 free maps but can unlock 15 more for a total of 20 maps.

This is how your score will be calculated:              max:   999

  for every wave you beet 20 Points               30 * 20   =  600 
  
  for every life you get 3 Points                 100 * 3   =  300
  
  for every 10 coins you get 1 Point              999 / 10  =   99

For every 250 Points you get a star.

  example: 22 waves * 30 + 0 life * 3 + 80 coins / 10 = 668 Points   => 2 Stars!

# Editor
Yes, there is a map editor where you can create new maps. To store those self created maps on the EEPROM there a 5 slots. In the slot list you can choose with left/right if you wanna play or edit the map. Creating a map is like building towers but instead of towers you can build map tiles.

# Enemies 
The five types of enemies will attack your headquarter in groups of five. They can come in 6 different combinations, it is eigther one of the five types or all of them in a mix.

# Towers
There are eight different types of towers, for more information go to the ingame tower lexicon. You access it after building a tower, selecting it and press on menu item INFOS.

Those are the rotating towers facing different directions without their sockets.

![][allRotatingTowers]    

[allRotatingTowers]: https://github.com/hartmann1301/MicroTowers/blob/master/Extras/allRotatingTowers.png



