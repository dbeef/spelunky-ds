
@{{BLOCK(rope)

@=======================================================================
@
@	rope, 16x16@4, 
@	Transparent color : 20,9C,00
@	+ palette 16 entries, not compressed
@	+ 4 tiles Metatiled by 2x2 not compressed
@	Total size: 32 + 128 = 160
@
@	Time-stamp: 2018-04-27, 14:24:46
@	Exported by Cearn's GBA Image Transmogrifier, v0.8.14
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global ropeTiles		@ 128 unsigned chars
	.hidden ropeTiles
ropeTiles:
	.word 0x11000000,0x87100000,0x55710000,0x50550000,0x50000000,0x55555000,0x56524500,0x52654250
	.word 0x00000111,0x00001778,0x00017553,0x00055053,0x00000056,0x00000552,0x00005456,0x00054252
	.word 0x55255450,0x52655250,0x26555450,0x55556250,0x26262550,0x55555550,0x55555500,0x00000000
	.word 0x00562526,0x00525565,0x00565526,0x00526555,0x00552626,0x00555555,0x00055555,0x00000000

	.section .rodata
	.align	2
	.global ropePal		@ 32 unsigned chars
	.hidden ropePal
ropePal:
	.hword 0x0264,0x0842,0x0CAC,0x2108,0x1171,0x0422,0x15F9,0x6318
	.hword 0x7FFF,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

@}}BLOCK(rope)
