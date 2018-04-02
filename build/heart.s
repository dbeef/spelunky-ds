
@{{BLOCK(heart)

@=======================================================================
@
@	heart, 16x16@4, 
@	Transparent color : 20,9C,00
@	+ palette 16 entries, not compressed
@	+ 4 tiles Metatiled by 2x2 not compressed
@	Total size: 32 + 128 = 160
@
@	Time-stamp: 2018-04-02, 21:33:51
@	Exported by Cearn's GBA Image Transmogrifier, v0.8.14
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global heartTiles		@ 128 unsigned chars
	.hidden heartTiles
heartTiles:
	.word 0x00000000,0x05550000,0x57775000,0x77777500,0x71177750,0x14417750,0x43326650,0x33266550
	.word 0x00000000,0x00005550,0x00057775,0x00577777,0x05777117,0x05771441,0x05662334,0x05566233
	.word 0x32665500,0x26655000,0x66550000,0x65500000,0x55000000,0x50000000,0x00000000,0x00000000
	.word 0x00556623,0x00055662,0x00005566,0x00000556,0x00000055,0x00000005,0x00000000,0x00000000

	.section .rodata
	.align	2
	.global heartPal		@ 32 unsigned chars
	.hidden heartPal
heartPal:
	.hword 0x0264,0x0008,0x0010,0x0018,0x211F,0x0000,0x6318,0x7FFF
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

@}}BLOCK(heart)
