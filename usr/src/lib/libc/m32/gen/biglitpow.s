	.ident	"@(#)libc-m32:gen/biglitpow.s	1.4"
#
#	arrays used by both _dtop.c and _ptod.c
#     seperated out for space conservation.
#
# Format of a simulated extended precision power of ten.  These 
# numbers contain 71 bits; the first 70 bits are obtained by taking
# the ceiling at the 70th bit.  The idea is roughly to compensate
# for truncation errors during multiplication.  The 71st bit is whatever
# actually belongs there.  For obtaining 64 bits of accuracy, exactly
# how the 71st bit is handled is not critical.
#
# Each number comes with a binary exponent.

#struct simex {
#	short signif[5];
#	short expo;
#};

#
# This table contains, in simulated extended format, 1e320, 1e288,
# 1e256, 1e224, etc., descending by factors of 1e32.
# The lowest power in the table is 1e-352.

#struct simex _bigpow[] = {
	.section RODATA
	.globl	_bigpow
	.align	4
_dgdef2_(_bigpow, 264):
	.half	0x40c2, 0x05e5, 0x0f96, 0x19c6, 0x3af5,  1063
	.half	0x6913, 0x1f83, 0x0ae3, 0x145f, 0x063b,   956
	.half	0x553f, 0x1d7f, 0x1cef, 0x33bd, 0x06f0,   850
	.half	0x4529, 0x12df, 0x3f19, 0x3992, 0x17c3,   744
	.half	0x7038, 0x1ef1, 0x29c9, 0x0eaa, 0x3544,   637
	.half	0x5b0b, 0x1425, 0x1bff, 0x0c2f, 0x152d,   531
	.half	0x49dd, 0x08f9, 0x0c07, 0x1319, 0x2fe5,   425
	.half	0x77d9, 0x3562, 0x362c, 0x3629, 0x1164,   318
	.half	0x613c, 0x03e9, 0x0ffe, 0x1f4d, 0x2aa0,   212
	.half	0x4ee2, 0x35b5, 0x015b, 0x216b, 0x0efa,   106
	.half	0x4000,   0000,   0000,   0000,   0000,     0
	.half	0x67d8, 0x23d5, 0x2a29, 0x3329, 0x1d35,  -107
	.half	0x543f, 0x3d44, 0x3d29, 0x33d3, 0x12a1,  -213
	.half	0x445a, 0x005e, 0x3feb, 0x2aa7, 0x0d8a,  -319
	.half	0x6ee8, 0x08cf, 0x2325, 0x39c9, 0x1058,  -426
	.half	0x59fa, 0x1c12, 0x1edb, 0x2741, 0x09ae,  -532
	.half	0x48ff, 0x306e, 0x3aa1, 0x0798, 0x033f,  -638
	.half	0x7672, 0x279d, 0x2251, 0x229d, 0x1ed4,  -745
	.half	0x6018, 0x2864, 0x2b1b, 0x3432, 0x1cff,  -851
	.half	0x4df6, 0x19cc, 0x141b, 0x158a, 0x3ba9,  -957
	.half	0x7e80, 0x1712, 0x3a3c, 0x0471, 0x2846, -1064
	.half	0x66a1, 0x1422, 0x1a37, 0x269e, 0x3e82, -1170
#};

#
# This table contains 1e16, 1e15, 1e14, etc., down to 1e-15.
#

#struct simex _litpow[] = {
	.globl	_litpow
	.section RODATA
	.align	4
_dgdef2_(_litpow,384):
	.half	0x470d, 0x3937, 0x3820,   0000,   0000,    53
	.half	0x71af, 0x3526, 0x0d00,   0000,   0000,    49
	.half	0x5af3, 0x041e, 0x2400,   0000,   0000,    46
	.half	0x48c2, 0x1ce5, 0x1000,   0000,   0000,    43
	.half	0x746a, 0x14a2,   0000,   0000,   0000,    39
	.half	0x5d21, 0x36e8,   0000,   0000,   0000,    36
	.half	0x4a81, 0x1f20,   0000,   0000,   0000,    33
	.half	0x7735, 0x2500,   0000,   0000,   0000,    29
	.half	0x5f5e, 0x0400,   0000,   0000,   0000,    26
	.half	0x4c4b, 0x1000,   0000,   0000,   0000,    23
	.half	0x7a12,   0000,   0000,   0000,   0000,    19
	.half	0x61a8,   0000,   0000,   0000,   0000,    16
	.half	0x4e20,   0000,   0000,   0000,   0000,    13
	.half	0x7d00,   0000,   0000,   0000,   0000,     9
	.half	0x6400,   0000,   0000,   0000,   0000,     6
	.half	0x5000,   0000,   0000,   0000,   0000,     3
	.half	0x4000,   0000,   0000,   0000,   0000,     0
	.half	0x6666, 0x1999, 0x2666, 0x1999, 0x2668,    -4
	.half	0x51eb, 0x2147, 0x2b85, 0x07ae, 0x0520,    -7
	.half	0x4189, 0x0dd2, 0x3c6a, 0x1fbe, 0x1db4,   -10
	.half	0x68db, 0x22eb, 0x0710, 0x32ca, 0x15eb,   -14
	.half	0x53e2, 0x3588, 0x38da, 0x0f08, 0x1189,   -17
	.half	0x431b, 0x37a0, 0x2d7b, 0x18d3, 0x1ad5,   -20
	.half	0x6b5f, 0x329a, 0x2f2b, 0x3485, 0x1e20,   -24
	.half	0x55e6, 0x0ee2, 0x0c23, 0x039d, 0x3e80,   -27
	.half	0x44b8, 0x0be8, 0x09b5, 0x294b, 0x0b9a,   -30
	.half	0x6df3, 0x1fd9, 0x35ef, 0x1bab, 0x1f5c,   -34
	.half	0x57f5, 0x3fe1, 0x1e59, 0x0955, 0x3f7d,   -37
	.half	0x465e, 0x1981, 0x0b7a, 0x2111, 0x25fe,   -40
	.half	0x7097, 0x0268, 0x125d, 0x281c, 0x0996,   -44
	.half	0x5a12, 0x1b86, 0x284a, 0x39b0, 0x07ab,   -47
	.half	0x480e, 0x2f9e, 0x39d5, 0x2159, 0x2c89,   -50
#};
