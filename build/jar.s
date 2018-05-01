
@{{BLOCK(jar)

@=======================================================================
@
@	jar, 8x8@4, 
@	Transparent color : 20,9C,00
@	+ palette 16 entries, not compressed
@	+ 1 tiles not compressed
@	Total size: 32 + 32 = 64
@
@	Time-stamp: 2018-05-01, 08:53:12
@	Exported by Cearn's GBA Image Transmogrifier, v0.8.14
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global jarTiles		@ 32 unsigned chars
	.hidden jarTiles
jarTiles:
	.word 0x00233200,0x03211230,0x01466410,0x01355310,0x74566547,0x74555547,0x13444431,0x01777710

	.section .rodata
	.align	2
	.global jarPal		@ 32 unsigned chars
	.hidden jarPal
jarPal:
	.hword 0x0264,0x0001,0x0045,0x0487,0x08CA,0x112E,0x1DD4,0x0003
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

@}}BLOCK(jar)
