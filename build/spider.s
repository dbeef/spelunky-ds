
@{{BLOCK(spider)

@=======================================================================
@
@	spider, 96x32@4, 
@	Transparent color : 20,9C,00
@	+ palette 16 entries, not compressed
@	+ 48 tiles Metatiled by 2x2 not compressed
@	Total size: 32 + 1536 = 1568
@
@	Time-stamp: 2018-05-04, 16:47:10
@	Exported by Cearn's GBA Image Transmogrifier, v0.8.14
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global spiderTiles		@ 1536 unsigned chars
	.hidden spiderTiles
spiderTiles:
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x32000000,0x44202300
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000023,0x00320844
	.word 0x66427930,0x76642293,0x97368A26,0x751329A8,0x61292898,0x2B829862,0x08086862,0x00002820
	.word 0x03972466,0x39224667,0x62A8637A,0x8A923157,0x89829216,0x268928B2,0x26868080,0x02820000
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x32000000,0x44202300,0x66427930
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000023,0x00320844,0x03972466
	.word 0x76642293,0x97368A26,0x751329A8,0x61292898,0x2B829862,0x08886262,0x00862628,0x00888880
	.word 0x39224667,0x62A8637A,0x8A923157,0x89829216,0x268928B2,0x26268880,0x82626800,0x08888800

	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x32000000,0x44223000,0x66479300
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000023,0x00032844,0x00397466
	.word 0x76622930,0x9738A260,0x75129A80,0x61228980,0x2B898620,0x08868620,0x00868680,0x00088800
	.word 0x03922667,0x062A837A,0x08A92157,0x08982216,0x026898B2,0x02686880,0x08686800,0x00888000
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x32000000,0x44202300,0x66427930
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000023,0x00320844,0x03972466
	.word 0x76642293,0x97368A26,0x751329A8,0x61292898,0x2B829862,0x08886262,0x00862628,0x00888880
	.word 0x39224667,0x62A8637A,0x8A923157,0x89829216,0x268928B2,0x26268880,0x82626800,0x08888800

	.word 0x00002820,0x08086862,0x2B829862,0x61292898,0x751329A8,0x97368A26,0x76642293,0x66427930
	.word 0x02820000,0x26868080,0x268928B2,0x89829216,0x8A923157,0x62A8637A,0x39224667,0x03972466
	.word 0x44202300,0x32000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00320844,0x00000023,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00002820,0x08086862,0x2B829862,0x62262892,0x95268922,0x96642262,0x66426620,0x44202200
	.word 0x02820000,0x26868080,0x268928B2,0x29826226,0x22986259,0x26224669,0x02662466,0x00220844
	.word 0x22000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00000022,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000

	.word 0x00000000,0x00220000,0x00622200,0x22626820,0x28286862,0x2B829862,0x62262892,0x95268920
	.word 0x00000000,0x00002200,0x00222600,0x02862622,0x26868282,0x268928B2,0x29826226,0x02986259
	.word 0x44228800,0x22880000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00882244,0x00008822,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00220000,0x00682200,0x22286820,0x22226862,0x22222862,0x28222892,0x2B828920
	.word 0x00000000,0x00002200,0x00228600,0x02868222,0x26862222,0x26822222,0x29822282,0x029828B2
	.word 0x62228800,0x95880000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00882226,0x00008859,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000

	.word 0x00000000,0x00002200,0x00026200,0x00262022,0x22222262,0x22226920,0x22222220,0x22222222
	.word 0x00000000,0x00220000,0x00262000,0x22026200,0x26222222,0x02962222,0x02222222,0x22222222
	.word 0x22228692,0x28828220,0x9B886620,0x08822962,0x00000222,0x00000000,0x00000000,0x00000000
	.word 0x29682222,0x02282882,0x026688B9,0x26922880,0x22200000,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x00000222,0x08822962,0x9B886620,0x28828220,0x22228692,0x22222222
	.word 0x00000000,0x00000000,0x22200000,0x26922880,0x026688B9,0x02282882,0x29682222,0x22222222
	.word 0x22222220,0x22226920,0x22222262,0x00262022,0x00026200,0x00002200,0x00000000,0x00000000
	.word 0x02222222,0x02962222,0x26222222,0x22026200,0x00262000,0x00220000,0x00000000,0x00000000

	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x95880000,0x62228800,0x2B828920,0x28222892
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00008859,0x00882226,0x029828B2,0x29822282
	.word 0x22222862,0x22226862,0x22286820,0x00682200,0x00220000,0x00000000,0x00000000,0x00000000
	.word 0x26822222,0x26862222,0x02868222,0x00228600,0x00002200,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x22880000,0x44228800
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00008822,0x00882244
	.word 0x95268920,0x62262892,0x2B829862,0x28286862,0x22626820,0x00622200,0x00220000,0x00000000
	.word 0x02986259,0x29826226,0x268928B2,0x26868282,0x02862622,0x00222600,0x00002200,0x00000000

	.section .rodata
	.align	2
	.global spiderPal		@ 32 unsigned chars
	.hidden spiderPal
spiderPal:
	.hword 0x0264,0x0008,0x0842,0x1084,0x2065,0x0018,0x2108,0x2D6B
	.hword 0x0000,0x4210,0x6318,0x7FFF,0x0000,0x0000,0x0000,0x0000

@}}BLOCK(spider)