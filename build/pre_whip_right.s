
@{{BLOCK(pre_whip_right)

@=======================================================================
@
@	pre_whip_right, 16x16@4, 
@	Transparent color : 20,9C,00
@	+ palette 16 entries, not compressed
@	+ 4 tiles Metatiled by 2x2 not compressed
@	Total size: 32 + 128 = 160
@
@	Time-stamp: 2018-04-03, 19:49:54
@	Exported by Cearn's GBA Image Transmogrifier, v0.8.14
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global pre_whip_rightTiles		@ 128 unsigned chars
	.hidden pre_whip_rightTiles
pre_whip_rightTiles:
	.word 0x00000000,0x00000000,0x40000000,0x24000000,0x24000000,0x40000000,0x00000000,0x00000000
	.word 0x00004440,0x00042224,0x00424442,0x04240004,0x42400444,0x42404222,0x24042444,0x40424000
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00424040,0x00042454,0x00004314,0x00000440,0x00000000,0x00000000,0x00000000,0x00000000

	.section .rodata
	.align	2
	.global pre_whip_rightPal		@ 32 unsigned chars
	.hidden pre_whip_rightPal
pre_whip_rightPal:
	.hword 0x0264,0x0CAC,0x1171,0x1173,0x0422,0x19D5,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

@}}BLOCK(pre_whip_right)
