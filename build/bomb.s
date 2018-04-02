
@{{BLOCK(bomb)

@=======================================================================
@
@	bomb, 16x16@4, 
@	Transparent color : 20,9C,00
@	+ palette 16 entries, not compressed
@	+ 4 tiles Metatiled by 2x2 not compressed
@	Total size: 32 + 128 = 160
@
@	Time-stamp: 2018-04-02, 20:28:38
@	Exported by Cearn's GBA Image Transmogrifier, v0.8.14
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global bombTiles		@ 128 unsigned chars
	.hidden bombTiles
bombTiles:
	.word 0x00000000,0x44000000,0x11440000,0x22114000,0x33221400,0x35321400,0x23532140,0x22332140
	.word 0x00000000,0x00010044,0x00161411,0x00461512,0x00446152,0x00434432,0x04143342,0x04144411
	.word 0x12222140,0x11221440,0x11114400,0x44444400,0x44444000,0x44440000,0x44000000,0x00000000
	.word 0x04144111,0x04144111,0x00414411,0x00414444,0x00041144,0x00004411,0x00000044,0x00000000

	.section .rodata
	.align	2
	.global bombPal		@ 32 unsigned chars
	.hidden bombPal
bombPal:
	.hword 0x0264,0x0842,0x0C64,0x2108,0x0000,0x4210,0x6318,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

@}}BLOCK(bomb)
