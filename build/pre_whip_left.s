
@{{BLOCK(pre_whip_left)

@=======================================================================
@
@	pre_whip_left, 16x16@4, 
@	Transparent color : 20,9C,00
@	+ palette 16 entries, not compressed
@	+ 4 tiles Metatiled by 2x2 not compressed
@	Total size: 32 + 128 = 160
@
@	Time-stamp: 2018-05-02, 17:56:05
@	Exported by Cearn's GBA Image Transmogrifier, v0.8.14
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global pre_whip_leftTiles		@ 128 unsigned chars
	.hidden pre_whip_leftTiles
pre_whip_leftTiles:
	.word 0x04440000,0x42224000,0x24442400,0x40004240,0x44400424,0x22240424,0x44424042,0x00042404
	.word 0x00000000,0x00000000,0x00000004,0x00000042,0x00000042,0x00000004,0x00000000,0x00000000
	.word 0x04042400,0x45424000,0x41340000,0x04400000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000

	.section .rodata
	.align	2
	.global pre_whip_leftPal		@ 32 unsigned chars
	.hidden pre_whip_leftPal
pre_whip_leftPal:
	.hword 0x0264,0x0CAC,0x1171,0x1173,0x0422,0x19D5,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

@}}BLOCK(pre_whip_left)
