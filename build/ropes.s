
@{{BLOCK(ropes)

@=======================================================================
@
@	ropes, 24x8@4, 
@	Transparent color : 20,9C,00
@	+ palette 16 entries, not compressed
@	+ 3 tiles not compressed
@	Total size: 32 + 96 = 128
@
@	Time-stamp: 2018-04-29, 16:50:48
@	Exported by Cearn's GBA Image Transmogrifier, v0.8.14
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global ropesTiles		@ 96 unsigned chars
	.hidden ropesTiles
ropesTiles:
	.word 0x00466400,0x00451400,0x00466400,0x00451400,0x00466400,0x00451400,0x00466400,0x00451400
	.word 0x00454400,0x04454140,0x43434414,0x45145341,0x45414454,0x43544134,0x04355340,0x00444400
	.word 0x00044000,0x00455400,0x04344540,0x43477434,0x41422414,0x04141144,0x00414440,0x00434000

	.section .rodata
	.align	2
	.global ropesPal		@ 32 unsigned chars
	.hidden ropesPal
ropesPal:
	.hword 0x0264,0x0CAC,0x1D07,0x1171,0x0422,0x15F9,0x1EBD,0x6F9B
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

@}}BLOCK(ropes)
