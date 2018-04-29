
@{{BLOCK(blood)

@=======================================================================
@
@	blood, 56x8@4, 
@	Transparent color : 20,9C,00
@	+ palette 16 entries, not compressed
@	+ 7 tiles not compressed
@	Total size: 32 + 224 = 256
@
@	Time-stamp: 2018-04-29, 16:50:48
@	Exported by Cearn's GBA Image Transmogrifier, v0.8.14
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global bloodTiles		@ 224 unsigned chars
	.hidden bloodTiles
bloodTiles:
	.word 0x00000000,0x00022200,0x00222220,0x00122210,0x00511100,0x00055000,0x00000000,0x00000000
	.word 0x00000000,0x00022200,0x00122222,0x01132221,0x05111150,0x00055000,0x00000000,0x00000000
	.word 0x00000000,0x00022200,0x00122220,0x00112222,0x00511115,0x00051500,0x00000000,0x00000000
	.word 0x00000000,0x00022000,0x00222200,0x01222220,0x05142210,0x00511100,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x00221200,0x00122220,0x00011100,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x00000000,0x00022200,0x00011100,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x00000000,0x00002000,0x00001000,0x00000000,0x00000000,0x00000000

	.section .rodata
	.align	2
	.global bloodPal		@ 32 unsigned chars
	.hidden bloodPal
bloodPal:
	.hword 0x0264,0x0010,0x0018,0x0C74,0x0C79,0x0008,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

@}}BLOCK(blood)
