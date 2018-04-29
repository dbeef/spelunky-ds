
@{{BLOCK(rock)

@=======================================================================
@
@	rock, 8x8@4, 
@	Transparent color : 20,9C,00
@	+ palette 16 entries, not compressed
@	+ 1 tiles not compressed
@	Total size: 32 + 32 = 64
@
@	Time-stamp: 2018-04-29, 19:44:50
@	Exported by Cearn's GBA Image Transmogrifier, v0.8.14
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global rockTiles		@ 32 unsigned chars
	.hidden rockTiles
rockTiles:
	.word 0x00000000,0x00444400,0x04444450,0x54444540,0x25566440,0x22225440,0x32222410,0x03222100

	.section .rodata
	.align	2
	.global rockPal		@ 32 unsigned chars
	.hidden rockPal
rockPal:
	.hword 0x0264,0x10E9,0x2108,0x14A4,0x4210,0x6318,0x7FFF,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

@}}BLOCK(rock)
