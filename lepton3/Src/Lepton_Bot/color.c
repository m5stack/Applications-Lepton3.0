/*
const unsigned char camColors[] = {
0X04,0XDF,0X04,0XDF,0X04,0XDF,0X04,0XFF,0X05,0X1F,0X05,0X1F,0X05,0X3F,0X05,0X3F,
0X05,0X5E,0X05,0X7E,0X05,0X9E,0X05,0X9E,0X05,0XBE,0X05,0XDE,0X05,0XDE,0X05,0XFD,
0X06,0X1D,0X06,0X1D,0X06,0X3D,0X06,0X5D,0X06,0X5C,0X06,0X5C,0X06,0X7C,0X06,0X9C,
0X06,0XBC,0X06,0XBC,0X06,0XDB,0X06,0XFB,0X06,0XFB,0X07,0X1B,0X07,0X3B,0X07,0X3B,
0X07,0X5A,0X07,0X7A,0X07,0X7A,0X07,0X7A,0X07,0XBA,0X07,0XBA,0X07,0XD9,0X07,0XF9,
0X07,0XF9,0X07,0XF8,0X07,0XF8,0X07,0XF8,0X07,0XF8,0X07,0XF7,0X07,0XF7,0X07,0XF6,
0X07,0XF5,0X07,0XF5,0X0F,0XF5,0X0F,0XF5,0X0F,0XF4,0X0F,0XF4,0X0F,0XF3,0X0F,0XF3,
0X0F,0XF3,0X0F,0XF2,0X0F,0XF2,0X0F,0XF1,0X17,0XF1,0X17,0XF1,0X17,0XF0,0X17,0XF0,
0X17,0XEF,0X17,0XEF,0X17,0XEF,0X17,0XEE,0X17,0XEE,0X1F,0XED,0X1F,0XED,0X1F,0XED,
0X1F,0XEC,0X1F,0XEC,0X1F,0XEB,0X1F,0XEB,0X1F,0XEA,0X1F,0XEA,0X1F,0XEA,0X1F,0XE9,
0X27,0XE9,0X27,0XE9,0X27,0XE8,0X27,0XE7,0X27,0XE7,0X27,0XE7,0X27,0XE7,0X27,0XE6,
0X27,0XE6,0X2F,0XE5,0X2F,0XE4,0X2F,0XE4,0X2F,0XE4,0X2F,0XE4,0X2F,0XE3,0X2F,0XE3,
0X2F,0XE2,0X2F,0XE2,0X2F,0XE2,0X2F,0XE1,0X37,0XE1,0X37,0XE0,0X37,0XE0,0X37,0XE0,
0X3F,0XE0,0X3F,0XE0,0X3F,0XE0,0X47,0XE0,0X47,0XE0,0X4F,0XE0,0X57,0XE0,0X57,0XE0,
0X57,0XE0,0X5F,0XE0,0X5F,0XE0,0X67,0XE0,0X67,0XE0,0X6F,0XE0,0X6F,0XE0,0X6F,0XE0,
0X77,0XE0,0X77,0XE0,0X7F,0XE0,0X87,0XE0,0X87,0XE0,0X8F,0XE0,0X8F,0XE0,0X8F,0XE0,
0X97,0XE0,0X97,0XE0,0X9F,0XE0,0XA7,0XE0,0XA7,0XE0,0XA7,0XE0,0XAF,0XE0,0XAF,0XE0,
0XAF,0XE0,0XB7,0XE0,0XBF,0XE0,0XBF,0XE0,0XBF,0XE0,0XC7,0XE0,0XC7,0XE0,0XCF,0XE0,
0XD7,0XE0,0XD7,0XE0,0XDF,0XE0,0XDF,0XE0,0XDF,0XE0,0XE7,0XE0,0XEF,0XE0,0XEF,0XE0,
0XEF,0XE0,0XEF,0XE0,0XF7,0XE0,0XF7,0XE0,0XFF,0XE0,0XFF,0XE0,0XFF,0XC0,0XFF,0XA0,
0XFF,0XA0,0XFF,0XA0,0XFF,0X80,0XFF,0X60,0XFF,0X40,0XFF,0X20,0XFF,0X00,0XFF,0X00,
0XFF,0X00,0XFE,0XE0,0XFE,0XC0,0XFE,0XA0,0XFE,0X80,0XFE,0X60,0XFE,0X60,0XFE,0X60,
0XFE,0X40,0XFE,0X20,0XFE,0X00,0XFD,0XE0,0XFD,0XC0,0XFD,0XC0,0XFD,0XC0,0XFD,0XA0,
0XFD,0X80,0XFD,0X60,0XFD,0X40,0XFD,0X40,0XFD,0X40,0XFD,0X20,0XFD,0X00,0XFC,0XE0,
0XFC,0XC0,0XFC,0XA0,0XFC,0XA0,0XFC,0XA0,0XFC,0X80,0XFC,0X60,0XFC,0X40,0XFC,0X20,
0XFC,0X00,0XFC,0X00,0XFC,0X00,0XFB,0XE0,0XFB,0XC0,0XFB,0XA0,0XFB,0X80,0XFB,0X60,
0XFB,0X60,0XFB,0X60,0XFB,0X40,0XFB,0X20,0XFB,0X00,0XFB,0X00,0XFA,0XE0,0XFA,0XE0,
0XFA,0XE0,0XFA,0XE0,0XFA,0XC0,0XFA,0XA1,0XFA,0XA1,0XFA,0X81,0XFA,0X81,0XFA,0X81,
0XFA,0X61,0XFA,0X61,0XFA,0X41,0XFA,0X41,0XFA,0X22,0XFA,0X22,0XFA,0X22,0XFA,0X02,
0XF9,0XE2,0XF9,0XE2,0XF9,0XE2,0XF9,0XC2,0XF9,0XC2,0XF9,0XC2,0XF9,0XA3,0XF9,0X83,
0XF9,0X83,0XF9,0X63,0XF9,0X63,0XF9,0X43,0XF9,0X43,0XF9,0X43,0XF9,0X24,0XF9,0X24,
0XF9,0X04,0XF9,0X04,0XF8,0XE4,0XF8,0XE4,0XF8,0XE4,0XF8,0XC4,0XF8,0XC4,0XF8,0XA5,
0XF8,0XA5,0XF8,0X85,0XF8,0X85,0XF8,0X85,0XF8,0X65,0XF8,0X45,0XF8,0X45,0XF8,0X45,
0XF8,0X26,0XF8,0X26,0XF8,0X06,0XF8,0X06,0XF8,0X06,0XF8,0X07,0XF8,0X07,0XF8,0X07,
0XF8,0X07,0XF8,0X08,0XF8,0X09,0XF8,0X09,0XF8,0X09,0XF8,0X0A,0XF8,0X0A,0XF8,0X0A,
0XF8,0X0B,0XF8,0X0B,0XF8,0X0C,0XF8,0X0C,0XF8,0X0C,0XF8,0X0D,0XF8,0X0D,0XF8,0X0E,
0XF8,0X0E,0XF8,0X0E,0XF8,0X0F,0XF8,0X0F,0XF8,0X10,0XF8,0X10,0XF8,0X10,0XF8,0X11,
0XF8,0X11,0XF8,0X11,0XF8,0X13,0X00,0X00,};
*/

const unsigned short camColors[] = {0x480F,
                              0x400F, 0x400F, 0x400F, 0x4010, 0x3810, 0x3810, 0x3810, 0x3810, 0x3010, 0x3010,
                              0x3010, 0x2810, 0x2810, 0x2810, 0x2810, 0x2010, 0x2010, 0x2010, 0x1810, 0x1810,
                              0x1811, 0x1811, 0x1011, 0x1011, 0x1011, 0x0811, 0x0811, 0x0811, 0x0011, 0x0011,
                              0x0011, 0x0011, 0x0011, 0x0031, 0x0031, 0x0051, 0x0072, 0x0072, 0x0092, 0x00B2,
                              0x00B2, 0x00D2, 0x00F2, 0x00F2, 0x0112, 0x0132, 0x0152, 0x0152, 0x0172, 0x0192,
                              0x0192, 0x01B2, 0x01D2, 0x01F3, 0x01F3, 0x0213, 0x0233, 0x0253, 0x0253, 0x0273,
                              0x0293, 0x02B3, 0x02D3, 0x02D3, 0x02F3, 0x0313, 0x0333, 0x0333, 0x0353, 0x0373,
                              0x0394, 0x03B4, 0x03D4, 0x03D4, 0x03F4, 0x0414, 0x0434, 0x0454, 0x0474, 0x0474,
                              0x0494, 0x04B4, 0x04D4, 0x04F4, 0x0514, 0x0534, 0x0534, 0x0554, 0x0554, 0x0574,
                              0x0574, 0x0573, 0x0573, 0x0573, 0x0572, 0x0572, 0x0572, 0x0571, 0x0591, 0x0591,
                              0x0590, 0x0590, 0x058F, 0x058F, 0x058F, 0x058E, 0x05AE, 0x05AE, 0x05AD, 0x05AD,
                              0x05AD, 0x05AC, 0x05AC, 0x05AB, 0x05CB, 0x05CB, 0x05CA, 0x05CA, 0x05CA, 0x05C9,
                              0x05C9, 0x05C8, 0x05E8, 0x05E8, 0x05E7, 0x05E7, 0x05E6, 0x05E6, 0x05E6, 0x05E5,
                              0x05E5, 0x0604, 0x0604, 0x0604, 0x0603, 0x0603, 0x0602, 0x0602, 0x0601, 0x0621,
                              0x0621, 0x0620, 0x0620, 0x0620, 0x0620, 0x0E20, 0x0E20, 0x0E40, 0x1640, 0x1640,
                              0x1E40, 0x1E40, 0x2640, 0x2640, 0x2E40, 0x2E60, 0x3660, 0x3660, 0x3E60, 0x3E60,
                              0x3E60, 0x4660, 0x4660, 0x4E60, 0x4E80, 0x5680, 0x5680, 0x5E80, 0x5E80, 0x6680,
                              0x6680, 0x6E80, 0x6EA0, 0x76A0, 0x76A0, 0x7EA0, 0x7EA0, 0x86A0, 0x86A0, 0x8EA0,
                              0x8EC0, 0x96C0, 0x96C0, 0x9EC0, 0x9EC0, 0xA6C0, 0xAEC0, 0xAEC0, 0xB6E0, 0xB6E0,
                              0xBEE0, 0xBEE0, 0xC6E0, 0xC6E0, 0xCEE0, 0xCEE0, 0xD6E0, 0xD700, 0xDF00, 0xDEE0,
                              0xDEC0, 0xDEA0, 0xDE80, 0xDE80, 0xE660, 0xE640, 0xE620, 0xE600, 0xE5E0, 0xE5C0,
                              0xE5A0, 0xE580, 0xE560, 0xE540, 0xE520, 0xE500, 0xE4E0, 0xE4C0, 0xE4A0, 0xE480,
                              0xE460, 0xEC40, 0xEC20, 0xEC00, 0xEBE0, 0xEBC0, 0xEBA0, 0xEB80, 0xEB60, 0xEB40,
                              0xEB20, 0xEB00, 0xEAE0, 0xEAC0, 0xEAA0, 0xEA80, 0xEA60, 0xEA40, 0xF220, 0xF200,
                              0xF1E0, 0xF1C0, 0xF1A0, 0xF180, 0xF160, 0xF140, 0xF100, 0xF0E0, 0xF0C0, 0xF0A0,
                              0xF080, 0xF060, 0xF040, 0xF020, 0xF800,
                             };
                             