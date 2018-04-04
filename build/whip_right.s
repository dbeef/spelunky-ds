
@{{BLOCK(whip_right)

@=======================================================================
@
@	whip_right, 16x16@4, 
@	Transparent color : 20,9C,00
@	+ palette 16 entries, not compressed
@	+ 4 tiles Metatiled by 2x2 not compressed
@	Total size: 32 + 128 = 160
@
@	Time-stamp: 2018-04-04, 21:49:28
@	Exported by Cearn's GBA Image Transmogrifier, v0.8.14
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global whip_rightTiles		@ 128 unsigned chars
	.hidden whip_rightTiles
whip_rightTiles:
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00004444
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00004000,0x00045440
	.word 0x44442222,0x22224444,0x44440000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00041324,0x00004442,0x00000004,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000

	.section .rodata
	.align	2
	.global whip_rightPal		@ 32 unsigned chars
	.hidden whip_rightPal
whip_rightPal:
	.hword 0x0264,0x0CAC,0x1171,0x1194,0x0422,0x19D5,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

@}}BLOCK(whip_right)
