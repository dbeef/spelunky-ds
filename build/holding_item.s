
@{{BLOCK(holding_item)

@=======================================================================
@
@	holding_item, 16x16@4, 
@	Transparent color : 20,9C,00
@	+ palette 16 entries, not compressed
@	+ 4 tiles Metatiled by 2x2 not compressed
@	Total size: 32 + 128 = 160
@
@	Time-stamp: 2018-04-07, 15:21:32
@	Exported by Cearn's GBA Image Transmogrifier, v0.8.14
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global holding_itemTiles		@ 128 unsigned chars
	.hidden holding_itemTiles
holding_itemTiles:
	.word 0x03333300,0x33444430,0x33333443,0x33333343,0x33333343,0x33333343,0x33333333,0x33333333
	.word 0x00333330,0x03444433,0x34433333,0x34333333,0x34333333,0x34333333,0x33333333,0x33333333
	.word 0x33333333,0x33333343,0x33333343,0x33333343,0x33333443,0x33444433,0x33333333,0x03333120
	.word 0x33333333,0x34333333,0x34333333,0x34333333,0x34433333,0x33444433,0x33333333,0x03333330

	.section .rodata
	.align	2
	.global holding_itemPal		@ 32 unsigned chars
	.hidden holding_itemPal
holding_itemPal:
	.hword 0x0264,0x0042,0x0842,0x0000,0x7FFF,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

@}}BLOCK(holding_item)
