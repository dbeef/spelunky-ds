
@{{BLOCK(bomb)

@=======================================================================
@
@	bomb, 16x16@4, 
@	Transparent color : 20,9C,00
@	+ palette 16 entries, not compressed
@	+ 4 tiles Metatiled by 2x2 not compressed
@	Total size: 32 + 128 = 160
@
@	Time-stamp: 2018-04-27, 09:05:04
@	Exported by Cearn's GBA Image Transmogrifier, v0.8.14
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global bombTiles		@ 128 unsigned chars
	.hidden bombTiles
bombTiles:
	.word 0x00000000,0x00555000,0x05533500,0x55536350,0x55553350,0x55555550,0x05555500,0x00555000
	.word 0x00000000,0x00244000,0x02477400,0x21477740,0x21447740,0x21144420,0x02111200,0x00222000
	.word 0x55555555,0x55555555,0x55555555,0x55555555,0x55555555,0x55555555,0x55555555,0x55555555
	.word 0x55555555,0x55555555,0x55555555,0x55555555,0x55555555,0x55555555,0x55555555,0x55555555

	.section .rodata
	.align	2
	.global bombPal		@ 32 unsigned chars
	.hidden bombPal
bombPal:
	.hword 0x0264,0x0010,0x0018,0x1D07,0x1CFF,0x0863,0x3E0F,0x423F
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

@}}BLOCK(bomb)
