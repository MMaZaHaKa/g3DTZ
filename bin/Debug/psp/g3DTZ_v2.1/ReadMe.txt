g3DTZ v2.1 - a GAME.DTZ extraction utility - by guard3

Under no circumstances should you host this anywhere else; respect my work!

=== GitHub ===
https://github.com/guard3/g3DTZ

=== What's included ===
lcs_psp.exe
lcs_ps2.exe
vcs_psp.exe
vcs_ps2.exe
ReadMe.txt

=== How to use ===
Just drag n' drop a GAME.DTZ/GAME.DAT to the appropriate executable
Alternatively, you can drag n' drop a .ANIM file, it will be detected automatically
eg. game.dtz from lcs psp -> lcs_psp.exe etc...

The tool will automatically generate a folder in the same location as the specified GAME.DTZ specified

=== What's extracted ===
IDE
IPL
SFX.SDT sets (PSP only)
Zones (info.zon/map.zon/navig.zon)
Path data (flight.dat/ferry1.dat/tracks.dat/tracks2.dat)
Colour data (carcols.dat/pedcols.dat)
Weapon data (weapon.dat/weapon_multi.dat)
Animations
CULL.IPL
OBJECT.DAT
PED.DAT
PEDGRP.DAT
PEDSTATS.DAT
FISTFITE.DAT (LCS)
PARTICLE.CFG
SURFACE.DAT
HANDLING.CFG
TIMECYC.DAT
WATERPRO.DAT
cdimage DIR
cutscene DIR
GAME.DAT (aka decompressed GAME.DTZ)

=== Notes ===
- The last 5 values of object.dat from VCS are unknown, probably different way of assigning damage effects
- There are no actual handling names in VCS, so the tool extracts with modelnames instead
- In LCS, the handling flags are the same as in VC, for VCS they are the same as SA.
- VCS has some extra special handling for hovercar and jetski, but I haven't looked into it
- VCS has some modelnames missing because not all of them have been cracked, and most likely won't ever be!
- VCS PS2 timecyc.dat has 4 extra values for radiosity and blur, so its format isn't very standard
- VCS most likely ditched FISTFITE.DAT for some other data format for its special fighting system


=== New stuff in g3DTZ v2.0 ===
- IFP support for individual .ANIM files (Cutscene anims not tested!)

=== Credits ===
The Hero, for his structs and helping me out when needed
cj2000, for stuff from his documentation