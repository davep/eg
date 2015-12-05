/*

     Expert Guide - A Text Mode Norton Guide Reader
     Copyright (C) 1997-2015 David A Pearson
   
     This program is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation; either version 2 of the license, or 
     (at your option) any later version.
     
     This program is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
     GNU General Public License for more details.
     
     You should have received a copy of the GNU General Public License
     along with this program; if not, write to the Free Software
     Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

*/

#ifndef __EGCOLOUR_H

#define __EGCOLOUR_H

/* Available colour combinations */

#define BLACK_BLACK             0x00
#define BLUE_BLACK              0x01
#define GREEN_BLACK             0x02
#define CYAN_BLACK              0x03
#define RED_BLACK               0x04
#define MAGENTA_BLACK           0x05
#define YELLOW_BLACK            0x06
#define WHITE_BLACK             0x07
#define HI_BLACK_BLACK          0x08
#define HI_BLUE_BLACK           0x09
#define HI_GREEN_BLACK          0x0A
#define HI_CYAN_BLACK           0x0B
#define HI_RED_BLACK            0x0C
#define HI_MAGENTA_BLACK        0x0D
#define HI_YELLOW_BLACK         0x0E
#define HI_WHITE_BLACK          0x0F
#define BLACK_BLUE              0x10
#define BLUE_BLUE               0x11
#define GREEN_BLUE              0x12
#define CYAN_BLUE               0x13
#define RED_BLUE                0x14
#define MAGENTA_BLUE            0x15
#define YELLOW_BLUE             0x16
#define WHITE_BLUE              0x17
#define HI_BLACK_BLUE           0x18
#define HI_BLUE_BLUE            0x19
#define HI_GREEN_BLUE           0x1A
#define HI_CYAN_BLUE            0x1B
#define HI_RED_BLUE             0x1C
#define HI_MAGENTA_BLUE         0x1D
#define HI_YELLOW_BLUE          0x1E
#define HI_WHITE_BLUE           0x1F
#define BLACK_GREEN             0x20
#define BLUE_GREEN              0x21
#define GREEN_GREEN             0x22
#define CYAN_GREEN              0x23
#define RED_GREEN               0x24
#define MAGENTA_GREEN           0x25
#define YELLOW_GREEN            0x26
#define WHITE_GREEN             0x27
#define HI_BLACK_GREEN          0x28
#define HI_BLUE_GREEN           0x29
#define HI_GREEN_GREEN          0x2A
#define HI_CYAN_GREEN           0x2B
#define HI_RED_GREEN            0x2C
#define HI_MAGENTA_GREEN        0x2D
#define HI_YELLOW_GREEN         0x2E
#define HI_WHITE_GREEN          0x2F
#define BLACK_CYAN              0x30
#define BLUE_CYAN               0x31
#define GREEN_CYAN              0x32
#define CYAN_CYAN               0x33
#define RED_CYAN                0x34
#define MAGENTA_CYAN            0x35
#define YELLOW_CYAN             0x36
#define WHITE_CYAN              0x37
#define HI_BLACK_CYAN           0x38
#define HI_BLUE_CYAN            0x39
#define HI_GREEN_CYAN           0x3A
#define HI_CYAN_CYAN            0x3B
#define HI_RED_CYAN             0x3C
#define HI_MAGENTA_CYAN         0x3D
#define HI_YELLOW_CYAN          0x3E
#define HI_WHITE_CYAN           0x3F
#define BLACK_RED               0x40
#define BLUE_RED                0x41
#define GREEN_RED               0x42
#define CYAN_RED                0x43
#define RED_RED                 0x44
#define MAGENTA_RED             0x45
#define YELLOW_RED              0x46
#define WHITE_RED               0x47
#define HI_BLACK_RED            0x48
#define HI_BLUE_RED             0x49
#define HI_GREEN_RED            0x4A
#define HI_CYAN_RED             0x4B
#define HI_RED_RED              0x4C
#define HI_MAGENTA_RED          0x4D
#define HI_YELLOW_RED           0x4E
#define HI_WHITE_RED            0x4F
#define BLACK_MAGENTA           0x50
#define BLUE_MAGENTA            0x51
#define GREEN_MAGENTA           0x52
#define CYAN_MAGENTA            0x53
#define RED_MAGENTA             0x54
#define MAGENTA_MAGENTA         0x55
#define YELLOW_MAGENTA          0x56
#define WHITE_MAGENTA           0x57
#define HI_BLACK_MAGENTA        0x58
#define HI_BLUE_MAGENTA         0x59
#define HI_GREEN_MAGENTA        0x5A
#define HI_CYAN_MAGENTA         0x5B
#define HI_RED_MAGENTA          0x5C
#define HI_MAGENTA_MAGENTA      0x5D
#define HI_YELLOW_MAGENTA       0x5E
#define HI_WHITE_MAGENTA        0x5F
#define BLACK_YELLOW            0x60
#define BLUE_YELLOW             0x61
#define GREEN_YELLOW            0x62
#define CYAN_YELLOW             0x63
#define RED_YELLOW              0x64
#define MAGENTA_YELLOW          0x65
#define YELLOW_YELLOW           0x66
#define WHITE_YELLOW            0x67
#define HI_BLACK_YELLOW         0x68
#define HI_BLUE_YELLOW          0x69
#define HI_GREEN_YELLOW         0x6A
#define HI_CYAN_YELLOW          0x6B
#define HI_RED_YELLOW           0x6C
#define HI_MAGENTA_YELLOW       0x6D
#define HI_YELLOW_YELLOW        0x6E
#define HI_WHITE_YELLOW         0x6F
#define BLACK_WHITE             0x70
#define BLUE_WHITE              0x71
#define GREEN_WHITE             0x72
#define CYAN_WHITE              0x73
#define RED_WHITE               0x74
#define MAGENTA_WHITE           0x75
#define YELLOW_WHITE            0x76
#define WHITE_WHITE             0x77
#define HI_BLACK_WHITE          0x78
#define HI_BLUE_WHITE           0x79
#define HI_GREEN_WHITE          0x7A
#define HI_CYAN_WHITE           0x7B
#define HI_RED_WHITE            0x7C
#define HI_MAGENTA_WHITE        0x7D
#define HI_YELLOW_WHITE         0x7E
#define HI_WHITE_WHITE          0x7F
#define BLACK_BLACK_BLNK        0x80
#define BLUE_BLACK_BLNK         0x81
#define GREEN_BLACK_BLNK        0x82
#define CYAN_BLACK_BLNK         0x83
#define RED_BLACK_BLNK          0x84
#define MAGENTA_BLACK_BLNK      0x85
#define YELLOW_BLACK_BLNK       0x86
#define WHITE_BLACK_BLNK        0x87
#define HI_BLACK_BLACK_BLNK     0x88
#define HI_BLUE_BLACK_BLNK      0x89
#define HI_GREEN_BLACK_BLNK     0x8A
#define HI_CYAN_BLACK_BLNK      0x8B
#define HI_RED_BLACK_BLNK       0x8C
#define HI_MAGENTA_BLACK_BLNK   0x8D
#define HI_YELLOW_BLACK_BLNK    0x8E
#define HI_WHITE_BLACK_BLNK     0x8F
#define BLACK_BLUE_BLNK         0x90
#define BLUE_BLUE_BLNK          0x91
#define GREEN_BLUE_BLNK         0x92
#define CYAN_BLUE_BLNK          0x93
#define RED_BLUE_BLNK           0x94
#define MAGENTA_BLUE_BLNK       0x95
#define YELLOW_BLUE_BLNK        0x96
#define WHITE_BLUE_BLNK         0x97
#define HI_BLACK_BLUE_BLNK      0x98
#define HI_BLUE_BLUE_BLNK       0x99
#define HI_GREEN_BLUE_BLNK      0x9A
#define HI_CYAN_BLUE_BLNK       0x9B
#define HI_RED_BLUE_BLNK        0x9C
#define HI_MAGENTA_BLUE_BLNK    0x9D
#define HI_YELLOW_BLUE_BLNK     0x9E
#define HI_WHITE_BLUE_BLNK      0x9F
#define BLACK_GREEN_BLNK        0xA0
#define BLUE_GREEN_BLNK         0xA1
#define GREEN_GREEN_BLNK        0xA2
#define CYAN_GREEN_BLNK         0xA3
#define RED_GREEN_BLNK          0xA4
#define MAGENTA_GREEN_BLNK      0xA5
#define YELLOW_GREEN_BLNK       0xA6
#define WHITE_GREEN_BLNK        0xA7
#define HI_BLACK_GREEN_BLNK     0xA8
#define HI_BLUE_GREEN_BLNK      0xA9
#define HI_GREEN_GREEN_BLNK     0xAA
#define HI_CYAN_GREEN_BLNK      0xAB
#define HI_RED_GREEN_BLNK       0xAC
#define HI_MAGENTA_GREEN_BLNK   0xAD
#define HI_YELLOW_GREEN_BLNK    0xAE
#define HI_WHITE_GREEN_BLNK     0xAF
#define BLACK_CYAN_BLNK         0xB0
#define BLUE_CYAN_BLNK          0xB1
#define GREEN_CYAN_BLNK         0xB2
#define CYAN_CYAN_BLNK          0xB3
#define RED_CYAN_BLNK           0xB4
#define MAGENTA_CYAN_BLNK       0xB5
#define YELLOW_CYAN_BLNK        0xB6
#define WHITE_CYAN_BLNK         0xB7
#define HI_BLACK_CYAN_BLNK      0xB8
#define HI_BLUE_CYAN_BLNK       0xB9
#define HI_GREEN_CYAN_BLNK      0xBA
#define HI_CYAN_CYAN_BLNK       0xBB
#define HI_RED_CYAN_BLNK        0xBC
#define HI_MAGENTA_CYAN_BLNK    0xBD
#define HI_YELLOW_CYAN_BLNK     0xBE
#define HI_WHITE_CYAN_BLNK      0xBF
#define BLACK_RED_BLNK          0xC0
#define BLUE_RED_BLNK           0xC1
#define GREEN_RED_BLNK          0xC2
#define CYAN_RED_BLNK           0xC3
#define RED_RED_BLNK            0xC4
#define MAGENTA_RED_BLNK        0xC5
#define YELLOW_RED_BLNK         0xC6
#define WHITE_RED_BLNK          0xC7
#define HI_BLACK_RED_BLNK       0xC8
#define HI_BLUE_RED_BLNK        0xC9
#define HI_GREEN_RED_BLNK       0xCA
#define HI_CYAN_RED_BLNK        0xCB
#define HI_RED_RED_BLNK         0xCC
#define HI_MAGENTA_RED_BLNK     0xCD
#define HI_YELLOW_RED_BLNK      0xCE
#define HI_WHITE_RED_BLNK       0xCF
#define BLACK_MAGENTA_BLNK      0xD0
#define BLUE_MAGENTA_BLNK       0xD1
#define GREEN_MAGENTA_BLNK      0xD2
#define CYAN_MAGENTA_BLNK       0xD3
#define RED_MAGENTA_BLNK        0xD4
#define MAGENTA_MAGENTA_BLNK    0xD5
#define YELLOW_MAGENTA_BLNK     0xD6
#define WHITE_MAGENTA_BLNK      0xD7
#define HI_BLACK_MAGENTA_BLNK   0xD8
#define HI_BLUE_MAGENTA_BLNK    0xD9
#define HI_GREEN_MAGENTA_BLNK   0xDA
#define HI_CYAN_MAGENTA_BLNK    0xDB
#define HI_RED_MAGENTA_BLNK     0xDC
#define HI_MAGENTA_MAGENTA_BLNK 0xDD
#define HI_YELLOW_MAGENTA_BLNK  0xDE
#define HI_WHITE_MAGENTA_BLNK   0xDF
#define BLACK_YELLOW_BLNK       0xE0
#define BLUE_YELLOW_BLNK        0xE1
#define GREEN_YELLOW_BLNK       0xE2
#define CYAN_YELLOW_BLNK        0xE3
#define RED_YELLOW_BLNK         0xE4
#define MAGENTA_YELLOW_BLNK     0xE5
#define YELLOW_YELLOW_BLNK      0xE6
#define WHITE_YELLOW_BLNK       0xE7
#define HI_BLACK_YELLOW_BLNK    0xE8
#define HI_BLUE_YELLOW_BLNK     0xE9
#define HI_GREEN_YELLOW_BLNK    0xEA
#define HI_CYAN_YELLOW_BLNK     0xEB
#define HI_RED_YELLOW_BLNK      0xEC
#define HI_MAGENTA_YELLOW_BLNK  0xED
#define HI_YELLOW_YELLOW_BLNK   0xEE
#define HI_WHITE_YELLOW_BLNK    0xEF
#define BLACK_WHITE_BLNK        0xF0
#define BLUE_WHITE_BLNK         0xF1
#define GREEN_WHITE_BLNK        0xF2
#define CYAN_WHITE_BLNK         0xF3
#define RED_WHITE_BLNK          0xF4
#define MAGENTA_WHITE_BLNK      0xF5
#define YELLOW_WHITE_BLNK       0xF6
#define WHITE_WHITE_BLNK        0xF7
#define HI_BLACK_WHITE_BLNK     0xF8
#define HI_BLUE_WHITE_BLNK      0xF9
#define HI_GREEN_WHITE_BLNK     0xFA
#define HI_CYAN_WHITE_BLNK      0xFB
#define HI_RED_WHITE_BLNK       0xFC
#define HI_MAGENTA_WHITE_BLNK   0xFD
#define HI_YELLOW_WHITE_BLNK    0xFE
#define HI_WHITE_WHITE_BLNK     0xFF

#endif
