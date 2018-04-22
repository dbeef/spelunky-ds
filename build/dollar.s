
@{{BLOCK(dollar)

@=======================================================================
@
@	dollar, 16x16@4, 
@	Transparent color : 20,9C,00
@	+ palette 16 entries, not compressed
@	+ 4 tiles Metatiled by 2x2 not compressed
@	Total size: 32 + 128 = 160
@
@	Time-stamp: 2018-04-22, 23:44:03
@	Exported by Cearn's GBA Image Transmogrifier, v0.8.14
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global dollarTiles		@ 128 unsigned chars
	.hidden dollarTiles
dollarTiles:
	.word 0x00000000,0x11100000,0x66110000,0x55661000,0x55556100,0x55155100,0x55155100,0x55655100
	.word 0x00000000,0x00001111,0x01111661,0x01666556,0x01555555,0x01111551,0x01111551,0x00166556
	.word 0x55551100,0x55111000,0x55111100,0x55666100,0x55555100,0x34111100,0x11111100,0x11100000
	.word 0x01655555,0x01551551,0x01551551,0x01556556,0x01155555,0x00111221,0x00011111,0x00001111

	.section .rodata
	.align	2
	.global dollarPal		@ 32 unsigned chars
	.hidden dollarPal
dollarPal:
	.hword 0x0264,0x0422,0x27DF,0x2BDF,0x2BDF,0x37DF,0x7FFF,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

@}}BLOCK(dollar)
