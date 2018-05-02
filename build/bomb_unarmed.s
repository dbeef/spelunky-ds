
@{{BLOCK(bomb_unarmed)

@=======================================================================
@
@	bomb_unarmed, 16x16@4, 
@	Transparent color : 20,9C,00
@	+ palette 16 entries, not compressed
@	+ 4 tiles Metatiled by 2x2 not compressed
@	Total size: 32 + 128 = 160
@
@	Time-stamp: 2018-05-02, 09:57:41
@	Exported by Cearn's GBA Image Transmogrifier, v0.8.14
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global bomb_unarmedTiles		@ 128 unsigned chars
	.hidden bomb_unarmedTiles
bomb_unarmedTiles:
	.word 0x00000000,0x00311000,0x03122100,0x33124210,0x33112210,0x33311130,0x03333300,0x00333000
	.word 0x33333333,0x33333333,0x33333333,0x33333333,0x33333333,0x33333333,0x33333333,0x33333333
	.word 0x33333333,0x33333333,0x33333333,0x33333333,0x33333333,0x33333333,0x33333333,0x33333333
	.word 0x33333333,0x33333333,0x33333333,0x33333333,0x33333333,0x33333333,0x33333333,0x33333333

	.section .rodata
	.align	2
	.global bomb_unarmedPal		@ 32 unsigned chars
	.hidden bomb_unarmedPal
bomb_unarmedPal:
	.hword 0x0264,0x0C64,0x2108,0x0842,0x4210,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

@}}BLOCK(bomb_unarmed)
