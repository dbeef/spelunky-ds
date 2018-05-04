
@{{BLOCK(triple_goldbar)

@=======================================================================
@
@	triple_goldbar, 16x16@4, 
@	Transparent color : 20,9C,00
@	+ palette 16 entries, not compressed
@	+ 4 tiles Metatiled by 2x2 not compressed
@	Total size: 32 + 128 = 160
@
@	Time-stamp: 2018-05-04, 13:51:11
@	Exported by Cearn's GBA Image Transmogrifier, v0.8.14
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global triple_goldbarTiles		@ 128 unsigned chars
	.hidden triple_goldbarTiles
triple_goldbarTiles:
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x33330000,0x65530000
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00003333,0x00004666
	.word 0x52230000,0x52230000,0x44230000,0x33333333,0x46666553,0x15555223,0x15555223,0x33114430
	.word 0x00001555,0x00001555,0x00003311,0x33333333,0x46666553,0x15555223,0x15555223,0x33114423

	.section .rodata
	.align	2
	.global triple_goldbarPal		@ 32 unsigned chars
	.hidden triple_goldbarPal
triple_goldbarPal:
	.hword 0x0264,0x10E9,0x1D6F,0x0844,0x0E5F,0x37DF,0x7FFF,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

@}}BLOCK(triple_goldbar)
