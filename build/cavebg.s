
@{{BLOCK(cavebg)

@=======================================================================
@
@	cavebg, 32x48@8, 
@	Transparent color : 20,9C,00
@	+ palette 256 entries, not compressed
@	+ 24 tiles Metatiled by 2x2 not compressed
@	Total size: 512 + 1536 = 2048
@
@	Time-stamp: 2018-03-18, 15:18:24
@	Exported by Cearn's GBA Image Transmogrifier, v0.8.14
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global cavebgTiles		@ 1536 unsigned chars
	.hidden cavebgTiles
cavebgTiles:
	.word 0x10645204,0x04641018,0x64101810,0x04521810,0x52181018,0x04181018,0x04101810,0x04041810
	.word 0x10040404,0x04040418,0x04101810,0x18101804,0x04040404,0x10181018,0x04041810,0x04101810
	.word 0x10180404,0x10181018,0x18040404,0x10101810,0x04041004,0x10181004,0x04041810,0x04040404
	.word 0x04040404,0x10180404,0x64100404,0x18040464,0x10181004,0x04045264,0x18101804,0x04646410
	.word 0x04181018,0x04041018,0x04101810,0x04040404,0x04040404,0x04181018,0x04040410,0x04101810
	.word 0x04645218,0x04181004,0x64521810,0x04040404,0x52181018,0x04040464,0x18101810,0x18100464
	.word 0x10181018,0x52641018,0x18101810,0x64101810,0x10181018,0x10181018,0x64101810,0x18106452
	.word 0x10181018,0x10641018,0x18101810,0x64101810,0x10181018,0x10181018,0x18101804,0x18101810

	.word 0x10181018,0x04045218,0x18101810,0x04040410,0x04040418,0x04040404,0x18040404,0x04046410
	.word 0x10180418,0x04641018,0x18100410,0x64101810,0x10180418,0x10181018,0x18100404,0x18101810
	.word 0x10181018,0x10181018,0x18101810,0x04101810,0x10181018,0x04181018,0x18101804,0x04101810
	.word 0x10180404,0x10181018,0x04040464,0x18101810,0x04645218,0x04040404,0x64101810,0x18040404
	.word 0x10180404,0x10181018,0x18040464,0x10101810,0x04045264,0x10181018,0x04646410,0x18101804
	.word 0x52641018,0x04040404,0x64101810,0x04040464,0x10181064,0x04040464,0x18101852,0x18040464
	.word 0x10181018,0x10040464,0x18101810,0x10040464,0x10181018,0x04040404,0x04041810,0x18100404
	.word 0x04040404,0x10181018,0x18101804,0x18101810,0x52181018,0x10185264,0x18101810,0x18646410

	.word 0x10181018,0x10040464,0x18101810,0x04040464,0x10181018,0x52180404,0x04101810,0x64101810
	.word 0x10040418,0x10181018,0x18100404,0x18101810,0x10180418,0x10641018,0x18100410,0x64641810
	.word 0x10181004,0x10181018,0x18100404,0x10101810,0x10040464,0x10181018,0x04046464,0x18101804
	.word 0x04645264,0x04040404,0x64526410,0x18100404,0x52641018,0x10180464,0x64101810,0x18100452
	.word 0x10180418,0x52181018,0x18100404,0x10101810,0x04040404,0x10181018,0x64641810,0x18040404
	.word 0x10181018,0x04040464,0x18101810,0x04046410,0x10181018,0x04641018,0x18101810,0x04641810
	.word 0x10181064,0x10040464,0x18101864,0x04040410,0x10181018,0x10040404,0x04041810,0x18100404
	.word 0x04040404,0x10181018,0x18040410,0x18101810,0x10180404,0x10181018,0x18100404,0x18101810

	.word 0x10181064,0x10040464,0x18101810,0x10040464,0x10181018,0x10040464,0x18101810,0x04040404
	.word 0x04040404,0x52180404,0x04046464,0x18101810,0x04645218,0x52641018,0x04641810,0x18101810
	.word 0x10181018,0x10641018,0x18101810,0x10521810,0x10181018,0x10181018,0x18100404,0x18101810
	.word 0x04045264,0x10181018,0x04521810,0x04040404,0x04641018,0x10181004,0x64101852,0x18101804
	.word 0x04641018,0x10181004,0x04040404,0x10100404,0x52645264,0x10040464,0x64641810,0x04046452
	.word 0x10181018,0x04045264,0x18101810,0x04046464,0x10181018,0x04045218,0x18101810,0x04046410
	.word 0x52181064,0x10181004,0x04101810,0x10101804,0x04040418,0x10180404,0x18101804,0x04040404
	.word 0x10181018,0x04040404,0x04101810,0x18100404,0x04040404,0x10181018,0x18101804,0x18101810

	.word 0xF9C9C64F,0x404F404F,0xC9C6C940,0xC9404FFD,0xC6C9404F,0xC64F40FD,0x4F404FC6,0xC9404F40
	.word 0xC64F404F,0x404F40C9,0xC9C64F40,0xF5958240,0xC64F40FD,0xEDB7954F,0x4F40FDC6,0xB7B7B782
	.word 0x404F404F,0x404F404F,0x4F404FFD,0x4F40C9C9,0x404FF9C9,0x40C9C6C9,0x4F40FDC6,0x4FC6C940
	.word 0x404F404F,0x404F404F,0xC9404FF5,0x4F404FFD,0xC64FEDF5,0xC64F40FD,0xC940F5ED,0xC9C64FC6
	.word 0xF9C9C64F,0x404F404F,0xC9C6C940,0xC9404FFD,0xC6C9404F,0xC64F40FD,0x4F404FC6,0xC9C64F40
	.word 0xC64F404F,0xC6C940C9,0xC9C64F40,0x4F404FC9,0xC64F40FD,0xC6C9404F,0x4F40FDC6,0xC9C6C940
	.word 0xC64F404F,0x404F40C9,0x4F404FC9,0xC9404F40,0x404FC6C9,0x404FF9C9,0xC940C9C6,0x4FFDC9C6
	.word 0x404F40C9,0x40C9C64F,0xFDC64F40,0x4F404F40,0xC6C9C64F,0xC64F40FD,0xC9C6C940,0xC9C64FFD

	.word 0x404FC6C9,0x95B79585,0x4F404FC9,0xB7958282,0xF9C9404F,0x9585824F,0xC9C94FC6,0x4F404FFD
	.word 0xC6C940C9,0x404F40C9,0x4F404F40,0x4FC64F40,0x40C9C64F,0x40C9C64F,0xC9C6C9C6,0x4F404F40
	.word 0xC64FEDB7,0xC6C940C9,0x4F40F5B7,0xC9404FC9,0x404F4085,0x404F404F,0x4FFD4F40,0xC9404F40
	.word 0x40FDC64F,0x404FC6C9,0x4F40C9C9,0x4FC6C9C9,0xC64F404F,0x40C9C6C9,0xC9404FC6,0x4F404FC6
	.word 0x404FC6C9,0x40C9C64F,0xC9C64FC9,0xC9404F40,0xC6C9404F,0xC64F404F,0x4F404FC6,0xC9404F40
	.word 0x404F40C9,0xC64FC6C9,0x4FC64F40,0x4FC6C9C9,0x40C9C64F,0x40C9C64F,0xC9C6C940,0x4F404F40
	.word 0xC6C9404F,0xC6C940C9,0xC9404FC9,0xC9404FC9,0x404FF9C9,0x404F404F,0x4FFDC9C6,0xC9404F40
	.word 0x40C9C6C9,0x404FC6C9,0x4F40C9C9,0x4FC6C9C9,0xC64F404F,0x40C9C6C9,0xC9404FC6,0x4F40C9C6

	.section .rodata
	.align	2
	.global cavebgPal		@ 512 unsigned chars
	.hidden cavebgPal
cavebgPal:
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0400,0x0400,0x0400
	.hword 0x0401,0x0420,0x0401,0x0401,0x0820,0x0820,0x0420,0x0820
	.hword 0x0420,0x0421,0x0420,0x0421,0x0820,0x0821,0x0820,0x0821
	.hword 0x0820,0x0821,0x0820,0x0821,0x0822,0x0821,0x0840,0x0423
	.hword 0x0841,0x0822,0x0C41,0x0C21,0x0423,0x0822,0x0841,0x0842
	.hword 0x0841,0x0823,0x0823,0x0C41,0x0841,0x0423,0x0443,0x0823
	.hword 0x0C41,0x0843,0x0444,0x0444,0x0C42,0x0C41,0x0444,0x0C42
	.hword 0x0C42,0x0844,0x0843,0x0844,0x0C43,0x0844,0x0C62,0x0844

	.hword 0x0445,0x0844,0x0C43,0x0843,0x0C62,0x0445,0x0844,0x0444
	.hword 0x0845,0x0C62,0x0C62,0x0445,0x0844,0x0444,0x1062,0x0844
	.hword 0x0C62,0x0845,0x1062,0x1062,0x0844,0x0C62,0x0845,0x0C63
	.hword 0x1062,0x1063,0x0C63,0x0845,0x0864,0x1062,0x0865,0x1062
	.hword 0x1063,0x0C63,0x0865,0x0865,0x1062,0x0C64,0x0C65,0x0866
	.hword 0x1063,0x0866,0x1063,0x1063,0x0866,0x1063,0x1063,0x0866
	.hword 0x1083,0x0865,0x0C65,0x0866,0x1083,0x0866,0x0866,0x0866
	.hword 0x1083,0x0865,0x1083,0x0866,0x0865,0x0866,0x1483,0x0C66

	.hword 0x0867,0x1483,0x0866,0x0866,0x0C66,0x1083,0x0C66,0x0867
	.hword 0x0866,0x0867,0x0867,0x0868,0x0886,0x0867,0x0868,0x0887
	.hword 0x0887,0x0868,0x0887,0x0887,0x0C88,0x0C87,0x0888,0x0887
	.hword 0x0C88,0x0889,0x0889,0x0C89,0x0889,0x0889,0x0C89,0x0889
	.hword 0x0C89,0x0C8A,0x0C89,0x0C8A,0x0C8A,0x0CA9,0x10AA,0x0CAA
	.hword 0x10A9,0x0CAA,0x08A9,0x0CAB,0x0CAA,0x0CAA,0x08A9,0x0CAB
	.hword 0x0CAB,0x0CAA,0x0CAB,0x0CAA,0x0CAB,0x0CAA,0x0CAB,0x0CAA
	.hword 0x0CAB,0x0CAA,0x0CAB,0x0CAB,0x10AA,0x0CAB,0x10AB,0x0CAC

	.hword 0x0CAC,0x0CAC,0x0CAC,0x10CB,0x10AB,0x0CAC,0x0CAC,0x0CCB
	.hword 0x0CCB,0x0CAC,0x0CAD,0x0CCC,0x10CC,0x10CC,0x0CCC,0x0CCD
	.hword 0x0CCC,0x0CCC,0x0CCD,0x10CB,0x0CCC,0x0CCD,0x10CD,0x0CCD
	.hword 0x10CD,0x10CD,0x10CC,0x10CD,0x10CE,0x0CCD,0x10CD,0x0CCD
	.hword 0x0CED,0x0CEE,0x10EE,0x0D0C,0x10EE,0x10EF,0x0D0D,0x10EE
	.hword 0x0D0D,0x0D0E,0x0D0D,0x0D0D,0x110E,0x112D,0x110E,0x112E
	.hword 0x0D2F,0x0D2F,0x112F,0x112F,0x112F,0x154F,0x1130,0x0D50
	.hword 0x1150,0x1150,0x1151,0x1151,0x1151,0x1172,0x1172,0x1172

@}}BLOCK(cavebg)
