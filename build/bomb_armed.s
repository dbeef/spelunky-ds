
@{{BLOCK(bomb_armed)

@=======================================================================
@
@	bomb_armed, 16x16@4, 
@	Transparent color : 20,9C,00
@	+ palette 16 entries, not compressed
@	+ 4 tiles Metatiled by 2x2 not compressed
@	Total size: 32 + 128 = 160
@
@	Time-stamp: 2018-04-07, 13:26:13
@	Exported by Cearn's GBA Image Transmogrifier, v0.8.14
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global bomb_armedTiles		@ 128 unsigned chars
	.hidden bomb_armedTiles
bomb_armedTiles:
	.word 0x00000000,0x00122000,0x01244200,0x13245420,0x13224420,0x13322210,0x01333100,0x00111000
	.word 0x33333333,0x33333333,0x33333333,0x33333333,0x33333333,0x33333333,0x33333333,0x33333333
	.word 0x33333333,0x33333333,0x33333333,0x33333333,0x33333333,0x33333333,0x33333333,0x33333333
	.word 0x33333333,0x33333333,0x33333333,0x33333333,0x33333333,0x33333333,0x33333333,0x33333333

	.section .rodata
	.align	2
	.global bomb_armedPal		@ 32 unsigned chars
	.hidden bomb_armedPal
bomb_armedPal:
	.hword 0x0264,0x0018,0x211F,0x0010,0x421F,0x631F,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

@}}BLOCK(bomb_armed)
