
@{{BLOCK(whip_left)

@=======================================================================
@
@	whip_left, 16x16@4, 
@	Transparent color : 20,9C,00
@	+ palette 16 entries, not compressed
@	+ 4 tiles Metatiled by 2x2 not compressed
@	Total size: 32 + 128 = 160
@
@	Time-stamp: 2018-05-01, 08:53:12
@	Exported by Cearn's GBA Image Transmogrifier, v0.8.14
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global whip_leftTiles		@ 128 unsigned chars
	.hidden whip_leftTiles
whip_leftTiles:
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00040000,0x04454000
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x44440000
	.word 0x42314000,0x24440000,0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x22224444,0x44442222,0x00004444,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000

	.section .rodata
	.align	2
	.global whip_leftPal		@ 32 unsigned chars
	.hidden whip_leftPal
whip_leftPal:
	.hword 0x0264,0x0CAC,0x1171,0x1194,0x0422,0x19D5,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

@}}BLOCK(whip_left)
