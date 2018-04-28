
@{{BLOCK(rubies_big)

@=======================================================================
@
@	rubies_big, 24x8@4, 
@	Transparent color : 20,9C,00
@	+ palette 16 entries, not compressed
@	+ 3 tiles not compressed
@	Total size: 32 + 96 = 128
@
@	Time-stamp: 2018-04-28, 11:42:25
@	Exported by Cearn's GBA Image Transmogrifier, v0.8.14
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global rubies_bigTiles		@ 96 unsigned chars
	.hidden rubies_bigTiles
rubies_bigTiles:
	.word 0x00011000,0x00149100,0x01FF9410,0x144FC941,0xA1112221,0x0A112210,0x00A12100,0x000A1000
	.word 0x00033000,0x0037D300,0x03FFD730,0x377FED73,0x33336663,0x03336630,0x00336300,0x00033000
	.word 0x00055000,0x005BB500,0x05FFBB50,0x5BBFFBB5,0x55558885,0x05558850,0x00558500,0x00055000

	.section .rodata
	.align	2
	.global rubies_bigPal		@ 32 unsigned chars
	.hidden rubies_bigPal
rubies_bigPal:
	.hword 0x0264,0x0008,0x0010,0x0CC0,0x0018,0x34C0,0x19C0,0x0D87
	.hword 0x5DC0,0x1CFF,0x0422,0x7AE6,0x3E5E,0x1F11,0x33DF,0x7FFF

@}}BLOCK(rubies_big)
