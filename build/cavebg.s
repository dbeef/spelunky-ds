
@{{BLOCK(cavebg)

@=======================================================================
@
@	cavebg, 32x144@8, 
@	Transparent color : 20,9C,00
@	+ palette 256 entries, not compressed
@	+ 72 tiles Metatiled by 2x2 not compressed
@	Total size: 512 + 4608 = 5120
@
@	Time-stamp: 2018-04-28, 11:42:25
@	Exported by Cearn's GBA Image Transmogrifier, v0.8.14
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global cavebgTiles		@ 4608 unsigned chars
	.hidden cavebgTiles
cavebgTiles:
	.word 0x00000002,0x02000202,0x02020202,0x00000202,0x02020202,0x00000202,0x02020202,0x00000202
	.word 0x02020202,0x05000002,0x02020202,0x02050002,0x02020202,0x02050000,0x00020202,0x02020500
	.word 0x02000202,0x02020202,0x02000000,0x05020202,0x00000000,0x05050202,0x00000505,0x02050202
	.word 0x00020202,0x02050200,0x00020202,0x02020200,0x02020202,0x02020200,0x02020202,0x02020000
	.word 0x00000000,0x02020500,0x00000000,0x02020205,0x05000005,0x02020202,0x02000202,0x02020202
	.word 0x02000202,0x02020202,0x00000202,0x02020202,0x00000202,0x00000000,0x00020202,0x00020202
	.word 0x02020202,0x02000000,0x02020202,0x00000000,0x02020202,0x00000000,0x00020202,0x05050500
	.word 0x00000202,0x02020505,0x05000002,0x02020205,0x05050000,0x02020202,0x02050500,0x05050202

	.word 0x02020202,0x02020002,0x02020505,0x02020002,0x02020202,0x02020002,0x02020202,0x00000002
	.word 0x02020202,0x05000000,0x02020202,0x02050500,0x00020202,0x02020205,0x00020202,0x05020205
	.word 0x00020202,0x02020200,0x00020202,0x02020200,0x00000202,0x02020000,0x00000000,0x02020000
	.word 0x02000005,0x02000002,0x02000202,0x02000202,0x02000202,0x02000202,0x00020205,0x00000202
	.word 0x00020202,0x02050205,0x00000000,0x02050202,0x00000000,0x02020200,0x00050505,0x02020200
	.word 0x02020202,0x00000000,0x02020202,0x05050000,0x02020202,0x02050500,0x02020205,0x02020500
	.word 0x00020202,0x00000000,0x00000202,0x00000000,0x05000002,0x05050505,0x05050000,0x02020505
	.word 0x05050000,0x02020202,0x05050005,0x02020202,0x02050002,0x02020202,0x02050002,0x02020202

	.word 0x00020202,0x00000202,0x00020202,0x05000002,0x00020202,0x05050000,0x00000202,0x02050500
	.word 0x00000202,0x02020500,0x00000002,0x02020200,0x02020000,0x02020000,0x02000000,0x00000000
	.word 0x02020505,0x02020502,0x02020205,0x02020205,0x05020202,0x02020202,0x05020202,0x02020202
	.word 0x05020202,0x02020202,0x02020202,0x02020202,0x02020202,0x02020202,0x02020202,0x00020202
	.word 0x00000000,0x00020200,0x00020505,0x02020200,0x02020202,0x02020200,0x02020202,0x02020002
	.word 0x00050502,0x00000002,0x02020205,0x00000200,0x02020205,0x02000200,0x02020200,0x02000200
	.word 0x00000000,0x02000000,0x02020000,0x02000002,0x02020002,0x00000002,0x02020002,0x05050000
	.word 0x00000000,0x02050500,0x00000202,0x02020505,0x00000202,0x02020205,0x02000202,0x02020202

	.word 0x02020202,0x02020200,0x02020202,0x02020200,0x02020202,0x00000000,0x00020202,0x05050000
	.word 0x00020202,0x05050500,0x00000202,0x02050505,0x05000002,0x02020505,0x05050000,0x02020205
	.word 0x02020002,0x02020202,0x02000000,0x02020202,0x00000000,0x02020202,0x00000205,0x02020202
	.word 0x00020202,0x02020200,0x00020202,0x02020000,0x02020202,0x00000000,0x02020505,0x00000200
	.word 0x05050002,0x05020202,0x02020000,0x05050202,0x02000000,0x02020202,0x02000505,0x02020202
	.word 0x00020202,0x02020202,0x02020202,0x02020200,0x02020202,0x00000000,0x02020202,0x02020002
	.word 0x02020205,0x00000000,0x00020202,0x05050000,0x00020202,0x02020500,0x00020202,0x02020205
	.word 0x05000202,0x02020202,0x05000202,0x02020202,0x02000000,0x02020202,0x02000002,0x02020202

	.word 0x100A0904,0x03040304,0x0A090A03,0x0A030410,0x090A0304,0x09040310,0x04030409,0x0A030403
	.word 0x09040304,0x0304030A,0x0A090403,0x0E070703,0x09040310,0x0E090704,0x04031009,0x09090907
	.word 0x03040304,0x03040304,0x04030410,0x04030A0A,0x0304100A,0x030A090A,0x04031009,0x04090A03
	.word 0x03040304,0x03040304,0x0A03040E,0x04030410,0x09040E0E,0x09040310,0x0A030E0E,0x0A090409
	.word 0x0304090A,0x07090706,0x0403040A,0x09070707,0x100A0304,0x07060704,0x0A0A0409,0x04030410
	.word 0x090A030A,0x0304030A,0x04030403,0x04090403,0x030A0904,0x030A0904,0x0A090A09,0x04030403
	.word 0x09040E09,0x090A030A,0x04030E09,0x0A03040A,0x03040306,0x03040304,0x04100403,0x0A030403
	.word 0x03100904,0x0304090A,0x04030A0A,0x04090A0A,0x09040304,0x030A090A,0x0A030409,0x04030409

	.word 0x100A0904,0x03040304,0x0A090A03,0x0A030410,0x090A0304,0x09040310,0x04030409,0x0A090403
	.word 0x09040304,0x090A030A,0x0A090403,0x0403040A,0x09040310,0x090A0304,0x04031009,0x0A090A03
	.word 0x09040304,0x0304030A,0x0403040A,0x0A030403,0x0304090A,0x0304100A,0x0A030A09,0x04100A09
	.word 0x0304030A,0x030A0904,0x10090403,0x04030403,0x090A0904,0x09040310,0x0A090A03,0x0A090410
	.word 0x0304090A,0x030A0904,0x0A09040A,0x0A030403,0x090A0304,0x09040304,0x04030409,0x0A030403
	.word 0x0304030A,0x0904090A,0x04090403,0x04090A0A,0x030A0904,0x030A0904,0x0A090A03,0x04030403
	.word 0x090A0304,0x090A030A,0x0A03040A,0x0A03040A,0x0304100A,0x03040304,0x04100A09,0x0A030403
	.word 0x030A090A,0x0304090A,0x04030A0A,0x04090A0A,0x09040304,0x030A090A,0x0A030409,0x04030A09

	.word 0x1414140B,0x14141414,0x11111114,0x11111111,0x11111111,0x11111111,0x07111111,0x07070707
	.word 0x07111111,0x0B0B0B0B,0x0B111111,0x11111111,0x0B070B11,0x11111111,0x07111111,0x11111111
	.word 0x14141414,0x0B141414,0x11110B11,0x14111111,0x11111111,0x11111111,0x07070707,0x11111107
	.word 0x0B0B0B0B,0x1107110B,0x11111111,0x110B110B,0x11111111,0x1111110B,0x11111111,0x1111110B
	.word 0x0B111111,0x11111111,0x0B111111,0x11111111,0x0B111111,0x11111111,0x0B111111,0x0B0B0B0B
	.word 0x14111111,0x14141414,0x11111111,0x11111111,0x1111110B,0x0B111111,0x0B0B0B07,0x070B0B0B
	.word 0x11111111,0x1111110B,0x11111111,0x0B11110B,0x11111111,0x1111110B,0x0B0B0B0B,0x1111110B
	.word 0x14141414,0x11111114,0x11111111,0x11111111,0x11111111,0x0B111111,0x0B0B0B0B,0x070B0B0B

	.word 0x03030903,0x09090303,0x10030303,0x09030309,0x09100303,0x03030309,0x09090309,0x10100303
	.word 0x09030309,0x09091003,0x03030303,0x09090903,0x09090303,0x09090303,0x09090903,0x03030309
	.word 0x09030909,0x03030303,0x03030309,0x03090903,0x03030303,0x09090909,0x03030909,0x09090909
	.word 0x03030909,0x09090303,0x10030309,0x03030309,0x09100303,0x03030909,0x09090903,0x09030909
	.word 0x09090901,0x09090309,0x09030101,0x09090309,0x01010103,0x09010303,0x03030308,0x08030308
	.word 0x03090803,0x01030903,0x09090901,0x03090901,0x09080108,0x08080101,0x01010000,0x09010000
	.word 0x09090903,0x01030309,0x01010109,0x01090903,0x01030109,0x03090909,0x01030108,0x03080801
	.word 0x03080901,0x01010103,0x08090908,0x09090101,0x08010101,0x08010109,0x01000109,0x01000001

	.word 0x03030303,0x09090303,0x09090303,0x09030303,0x09090903,0x09090303,0x03090903,0x16160903
	.word 0x03030303,0x17171609,0x03030303,0x13131309,0x09090303,0x11131309,0x16160903,0x03030909
	.word 0x09030909,0x03030303,0x03030309,0x03090909,0x09030309,0x03090316,0x16090916,0x09090313
	.word 0x03090313,0x09090303,0x09090311,0x03030309,0x16160903,0x03030903,0x16171609,0x09030903
	.word 0x13160903,0x16161603,0x03090303,0x16171603,0x03030303,0x11131609,0x03101003,0x03030909
	.word 0x09090910,0x03090909,0x16090909,0x03030903,0x03030909,0x10030303,0x09030303,0x09100303
	.word 0x13161609,0x03030303,0x03030313,0x03090903,0x03161703,0x03090909,0x03131609,0x03030909
	.word 0x03030903,0x09030303,0x03090903,0x09090303,0x03030309,0x09090303,0x09030909,0x09030309

	.word 0x03030303,0x09090303,0x09090303,0x03030303,0x03030903,0x16030303,0x16130303,0x03030303
	.word 0x03090303,0x17161613,0x13030303,0x13171616,0x16030303,0x13131716,0x11030903,0x16131313
	.word 0x09030909,0x03030303,0x03030303,0x03090909,0x09030313,0x03090316,0x16030903,0x09090313
	.word 0x03030309,0x09090303,0x09090313,0x03030303,0x16130311,0x03030903,0x16170309,0x09030909
	.word 0x03160903,0x09161311,0x03090303,0x13090303,0x03030303,0x11130309,0x03101003,0x03030909
	.word 0x09090910,0x17030909,0x16090909,0x16030903,0x03030909,0x03030303,0x09030303,0x09100303
	.word 0x13161303,0x03030303,0x03090911,0x03090309,0x03161703,0x03030313,0x03131609,0x03161703
	.word 0x03030917,0x03131603,0x03031316,0x03131303,0x03031313,0x09030303,0x09030303,0x09030309

	.word 0x01010910,0x13010101,0x13130309,0x10090110,0x10131303,0x03030309,0x09091003,0x10131303
	.word 0x03030303,0x09091010,0x03030303,0x09090909,0x10100303,0x09090903,0x09091003,0x03030310
	.word 0x09101010,0x09010101,0x09091010,0x03101303,0x03030303,0x10131313,0x03031010,0x09090910
	.word 0x03100909,0x09090903,0x10030909,0x03030310,0x09100309,0x03031009,0x09091003,0x09030909
	.word 0x09090903,0x09090309,0x09030303,0x09090309,0x03030303,0x09030303,0x03030903,0x03030909
	.word 0x03090909,0x03090909,0x03090909,0x03030903,0x03030909,0x09030303,0x09030303,0x09090303
	.word 0x09090903,0x03030309,0x03030309,0x03090903,0x03030309,0x03090909,0x03030303,0x03030909
	.word 0x03090903,0x09030303,0x03090903,0x09090303,0x03030309,0x09090303,0x09030909,0x09030309

	.word 0x01010910,0x13010101,0x13130309,0x10090110,0x10131303,0x03030309,0x09091003,0x10131303
	.word 0x03030303,0x09091010,0x03030303,0x09090909,0x10100303,0x09090903,0x09091003,0x03030310
	.word 0x09101010,0x09010101,0x09091010,0x03101303,0x03030303,0x10131313,0x03031010,0x09090910
	.word 0x03100909,0x09090903,0x10030909,0x03030310,0x09100309,0x03031009,0x09091003,0x09030909
	.word 0x09090901,0x09090309,0x09030101,0x09090309,0x01010303,0x09010303,0x03030308,0x08030308
	.word 0x03090803,0x01030901,0x09090901,0x03090901,0x09080108,0x08080101,0x01010000,0x09010000
	.word 0x09090903,0x01030309,0x01010109,0x01090901,0x01030109,0x03090909,0x01030108,0x03080801
	.word 0x03080901,0x01010103,0x08090908,0x09090101,0x08010101,0x08010109,0x01000109,0x01000001

	.word 0x12150000,0x00000000,0x12150000,0x15151500,0x12150000,0x12120C0C,0x12150000,0x00000000
	.word 0x12150000,0x00000000,0x12150000,0x15151500,0x12150000,0x12120C0C,0x12150000,0x00000000
	.word 0x00000000,0x00001215,0x17151515,0x00001215,0x12121212,0x00001215,0x00000000,0x00001215
	.word 0x00000000,0x00001215,0x17151515,0x00001215,0x12121212,0x00001215,0x00000000,0x00001215
	.word 0x12150000,0x00000000,0x12150000,0x15151500,0x12150000,0x12120C0C,0x12150000,0x00000000
	.word 0x12150000,0x00000000,0x12150000,0x15151500,0x12150000,0x12120C0C,0x12150000,0x00000000
	.word 0x00000000,0x00001215,0x17151515,0x00001215,0x12121212,0x00001215,0x00000000,0x00001215
	.word 0x00000000,0x00001215,0x17151515,0x00001215,0x12121212,0x00001215,0x00000000,0x00001215

	.word 0x00090909,0x00000000,0x09090909,0x09090909,0x10101010,0x10101010,0x03030303,0x03030303
	.word 0x0C120000,0x00000000,0x0C120000,0x12121200,0x0C120000,0x0C0C0C0C,0x0C120000,0x00000000
	.word 0x00000000,0x09090900,0x09090909,0x09090909,0x10101010,0x10101010,0x03030303,0x03030303
	.word 0x00000000,0x00000C12,0x12121212,0x00000C12,0x0C0C0C0C,0x00000C12,0x00000000,0x00000C12
	.word 0x0C120000,0x00000000,0x12150000,0x15151500,0x12150000,0x12120C0C,0x12150000,0x00000000
	.word 0x12150000,0x00000000,0x12150000,0x15151500,0x12150000,0x12120C0C,0x12150000,0x00000000
	.word 0x00000000,0x00000C12,0x17151515,0x00001215,0x12121212,0x00001215,0x00000000,0x00001215
	.word 0x00000000,0x00001215,0x17151515,0x00001215,0x12121212,0x00001215,0x00000000,0x00001215

	.word 0x07070707,0x07070707,0x11111407,0x14140B14,0x03071107,0x11111411,0x14141107,0x11111111
	.word 0x0B0B0307,0x030B0B03,0x11110700,0x07111107,0x11070000,0x0B11070B,0x07070700,0x0B070B0B
	.word 0x0B0B0707,0x07070B0B,0x11111414,0x07141111,0x07071111,0x07110307,0x140B1111,0x07110914
	.word 0x110B1111,0x07110B11,0x0B0B1103,0x07110B0B,0x14141109,0x07111414,0x11111114,0x07111111
	.word 0x14141407,0x14141414,0x070B0B07,0x11110307,0x14141407,0x11110B14,0x070B0B07,0x070B0B11
	.word 0x11141407,0x14141411,0x070B0B07,0x11110B07,0x11111107,0x0B110B11,0x03030307,0x03030303
	.word 0x07111111,0x07111103,0x14071111,0x03110314,0x11140711,0x03031411,0x11110B07,0x03091111
	.word 0x110B0714,0x070B1111,0x11110B11,0x030B0711,0x11110B11,0x030B0707,0x03030303,0x03030303

	.word 0x0B0B0707,0x07070B0B,0x11111407,0x14141111,0x07031107,0x11110707,0x14091107,0x11110B14
	.word 0x110B1107,0x11110B11,0x0B0B1107,0x03110B0B,0x14141107,0x09111414,0x11111107,0x14111111
	.word 0x07070707,0x07070707,0x140B1414,0x07141111,0x11141111,0x07110703,0x11111111,0x07111414
	.word 0x030B0B03,0x07030B0B,0x07111107,0x00071111,0x0B07110B,0x00000711,0x0B0B070B,0x00070707
	.word 0x03111107,0x11111107,0x14031103,0x11110714,0x11140303,0x11071411,0x11110903,0x070B1111
	.word 0x11110B07,0x14070B11,0x11070B03,0x110B1111,0x07070B03,0x110B1111,0x03030303,0x03030303
	.word 0x14141414,0x07141414,0x07031111,0x070B0B07,0x140B1111,0x07141414,0x110B0B07,0x070B0B07
	.word 0x11141414,0x07141411,0x070B1111,0x070B0B07,0x110B110B,0x07111111,0x03030303,0x07030303

	.word 0x00000000,0x00000000,0x0505050D,0x05050505,0x0F0F0D05,0x0F0F0F0F,0x02050D05,0x00000202
	.word 0x02020D05,0x02020202,0x05000D05,0x05050505,0x05000D05,0x05020505,0x05000D05,0x050D0505
	.word 0x00000000,0x00000000,0x05050505,0x0D020205,0x0F0F0F0F,0x020D0F0F,0x00000000,0x02050502
	.word 0x02020202,0x02050202,0x05050505,0x02050205,0x05050505,0x02050205,0x02050505,0x02050202
	.word 0x05000D05,0x05050505,0x05000D05,0x05050505,0x02000D05,0x05050502,0x0D000D05,0x0505050D
	.word 0x05000D05,0x05050505,0x05000D05,0x05050505,0x05000D05,0x05050502,0x05000D05,0x0505050D
	.word 0x0D050505,0x0205020D,0x05050505,0x02050205,0x05050505,0x02050205,0x05050505,0x02050205
	.word 0x05050205,0x02050205,0x05050D02,0x02050205,0x0505050D,0x02050205,0x05050505,0x02050205

	.word 0x00000000,0x00000000,0x0505050D,0x05050505,0x0F0F0D05,0x0F0F0F0F,0x02050D05,0x00000202
	.word 0x02020D05,0x00000000,0x00000D05,0x00000000,0x00000D05,0x00000000,0x00000D05,0x00000000
	.word 0x00000000,0x00000000,0x05050505,0x0D020205,0x0F0F0F0F,0x020D0F0F,0x00000000,0x02050502
	.word 0x00000000,0x02050200,0x00000000,0x02050000,0x00000000,0x02050000,0x00000000,0x02050200
	.word 0x00000D05,0x00000000,0x00000D05,0x00000000,0x00000D05,0x00000000,0x00000D05,0x00000000
	.word 0x00000D05,0x00000000,0x02000D05,0x00000000,0x02000D05,0x00000002,0x02000D05,0x00000002
	.word 0x00000000,0x02050000,0x00000000,0x02050000,0x00000000,0x02050500,0x00000000,0x02050205
	.word 0x00000000,0x02050202,0x00000000,0x02050202,0x00020200,0x02050202,0x00020202,0x02050202

	.section .rodata
	.align	2
	.global cavebgPal		@ 512 unsigned chars
	.hidden cavebgPal
cavebgPal:
	.hword 0x0000,0x0402,0x0820,0x0844,0x0844,0x1462,0x1063,0x0C84
	.hword 0x0868,0x0CAC,0x0CAC,0x14E9,0x18E8,0x2D06,0x112D,0x3D88
	.hword 0x1171,0x258E,0x3E0F,0x15F9,0x3675,0x5F17,0x33DF,0x7FFF
	.hword 0x0C63,0x0C63,0x0C63,0x0C63,0x0C63,0x0C63,0x0C63,0x0C63
	.hword 0x1084,0x1084,0x1084,0x1084,0x1084,0x1084,0x1084,0x1084
	.hword 0x14A5,0x14A5,0x14A5,0x14A5,0x14A5,0x14A5,0x14A5,0x14A5
	.hword 0x18C6,0x18C6,0x18C6,0x18C6,0x18C6,0x18C6,0x18C6,0x18C6
	.hword 0x1CE7,0x1CE7,0x1CE7,0x1CE7,0x1CE7,0x1CE7,0x1CE7,0x1CE7

	.hword 0x2108,0x2108,0x2108,0x2108,0x2108,0x2108,0x2108,0x2108
	.hword 0x2529,0x2529,0x2529,0x2529,0x2529,0x2529,0x2529,0x2529
	.hword 0x294A,0x294A,0x294A,0x294A,0x294A,0x294A,0x294A,0x294A
	.hword 0x2D6B,0x2D6B,0x2D6B,0x2D6B,0x2D6B,0x2D6B,0x2D6B,0x2D6B
	.hword 0x318C,0x318C,0x318C,0x318C,0x318C,0x318C,0x318C,0x318C
	.hword 0x35AD,0x35AD,0x35AD,0x35AD,0x35AD,0x35AD,0x35AD,0x35AD
	.hword 0x39CE,0x39CE,0x39CE,0x39CE,0x39CE,0x39CE,0x39CE,0x39CE
	.hword 0x3DEF,0x3DEF,0x3DEF,0x3DEF,0x3DEF,0x3DEF,0x3DEF,0x3DEF

	.hword 0x4210,0x4210,0x4210,0x4210,0x4210,0x4210,0x4210,0x4210
	.hword 0x4631,0x4631,0x4631,0x4631,0x4631,0x4631,0x4631,0x4631
	.hword 0x4A52,0x4A52,0x4A52,0x4A52,0x4A52,0x4A52,0x4A52,0x4A52
	.hword 0x4E73,0x4E73,0x4E73,0x4E73,0x4E73,0x4E73,0x4E73,0x4E73
	.hword 0x5294,0x5294,0x5294,0x5294,0x5294,0x5294,0x5294,0x5294
	.hword 0x56B5,0x56B5,0x56B5,0x56B5,0x56B5,0x56B5,0x56B5,0x56B5
	.hword 0x5AD6,0x5AD6,0x5AD6,0x5AD6,0x5AD6,0x5AD6,0x5AD6,0x5AD6
	.hword 0x5EF7,0x5EF7,0x5EF7,0x5EF7,0x5EF7,0x5EF7,0x5EF7,0x5EF7

	.hword 0x6318,0x6318,0x6318,0x6318,0x6318,0x6318,0x6318,0x6318
	.hword 0x6739,0x6739,0x6739,0x6739,0x6739,0x6739,0x6739,0x6739
	.hword 0x6B5A,0x6B5A,0x6B5A,0x6B5A,0x6B5A,0x6B5A,0x6B5A,0x6B5A
	.hword 0x6F7B,0x6F7B,0x6F7B,0x6F7B,0x6F7B,0x6F7B,0x6F7B,0x6F7B
	.hword 0x739C,0x739C,0x739C,0x739C,0x739C,0x739C,0x739C,0x739C
	.hword 0x77BD,0x77BD,0x77BD,0x77BD,0x77BD,0x77BD,0x77BD,0x77BD
	.hword 0x7BDE,0x7BDE,0x7BDE,0x7BDE,0x7BDE,0x7BDE,0x7BDE,0x7BDE
	.hword 0x7FFF,0x7FFF,0x7FFF,0x7FFF,0x7FFF,0x7FFF,0x7FFF,0x7FFF

@}}BLOCK(cavebg)
