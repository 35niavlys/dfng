/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <game/generated/protocol.h>
#include <game/server/gamecontext.h>
#include "laserText.h"
#include <string.h>

static const bool asciiTable[256][5][3] = {
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 0
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 1
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 2
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 3
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 4
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 5
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 6
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 7
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 8
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 9
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 10
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 11
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 12
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 13
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 14
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 15
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 16
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 17
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 18
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 19
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 20
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 21
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 22
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 23
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 24
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 25
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 26
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 27
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 28
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 29
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 30
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 31
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 32
	{ {0,1,0}, {0,1,0}, {0,1,0}, {0,0,0}, {0,1,0} }, // ascii 33 !
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 34
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 35
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 36
	{ {1,0,1}, {1,0,0}, {0,1,0}, {0,0,1}, {1,0,1} }, // ascii 37 %
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 38
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 39
	{ {0,1,0}, {1,0,0}, {1,0,0}, {1,0,0}, {0,1,0} }, // ascii 40 (
	{ {0,1,0}, {0,0,1}, {0,0,1}, {0,0,1}, {0,1,0} }, // ascii 41 )
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 42
	{ {0,0,0}, {0,1,0}, {1,1,1}, {0,1,0}, {0,0,0} }, // ascii 43 +
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,1,0}, {1,0,0} }, // ascii 44 ,
	{ {0,0,0}, {0,0,0}, {1,1,1}, {0,0,0}, {0,0,0} }, // ascii 45 -
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,1,0} }, // ascii 46 .
	{ {0,0,0}, {0,0,1}, {0,1,0}, {1,0,0}, {0,0,0} }, // ascii 47 /
	{ {1,1,1}, {1,0,1}, {1,0,1}, {1,0,1}, {1,1,1} }, // 0
	{ {0,1,0}, {1,1,0}, {0,1,0}, {0,1,0}, {0,1,0} }, // 1
	{ {1,1,1}, {0,0,1}, {1,1,1}, {1,0,0}, {1,1,1} }, // 2
	{ {1,1,1}, {0,0,1}, {1,1,1}, {0,0,1}, {1,1,1} }, // 3
	{ {1,0,1}, {1,0,1}, {1,1,1}, {0,0,1}, {0,0,1} }, // 4
	{ {1,1,1}, {1,0,0}, {1,1,1}, {0,0,1}, {1,1,1} }, // 5
	{ {1,1,1}, {1,0,0}, {1,1,1}, {1,0,1}, {1,1,1} }, // 6
	{ {1,1,1}, {0,0,1}, {0,0,1}, {0,0,1}, {0,0,1} }, // 7
	{ {1,1,1}, {1,0,1}, {1,1,1}, {1,0,1}, {1,1,1} }, // 8
	{ {1,1,1}, {1,0,1}, {1,1,1}, {0,0,1}, {1,1,1} }, // 9
	{ {0,0,0}, {0,1,0}, {0,0,0}, {0,1,0}, {0,0,0} }, // :
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 59
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 60
	{ {0,0,0}, {1,1,1}, {0,0,0}, {1,1,1}, {0,0,0} }, // ascii 61 =
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 62
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 63
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 64
	{ {1,1,1}, {1,0,1}, {1,1,1}, {1,0,1}, {1,0,1} }, // A
	{ {1,0,0}, {1,0,0}, {1,1,1}, {1,0,1}, {1,1,1} }, // B
	{ {1,1,1}, {1,0,0}, {1,0,0}, {1,0,0}, {1,1,1} }, // C
	{ {0,0,1}, {0,0,1}, {1,1,1}, {1,0,1}, {1,1,1} }, // D
	{ {1,1,1}, {1,0,0}, {1,1,1}, {1,0,0}, {1,1,1} }, // E
	{ {1,1,1}, {1,0,0}, {1,1,1}, {1,0,0}, {1,0,0} }, // F
	{ {1,1,1}, {1,0,1}, {1,1,1}, {0,0,1}, {0,1,1} }, // G
	{ {1,0,1}, {1,0,1}, {1,1,1}, {1,0,1}, {1,0,1} }, // H
	{ {1,0,0}, {1,0,0}, {1,0,0}, {1,0,0}, {1,0,0} }, // I
	{ {0,0,1}, {0,0,1}, {1,0,1}, {1,0,1}, {1,1,1} }, // J
	{ {1,0,0}, {1,0,0}, {1,0,1}, {1,1,0}, {1,0,1} }, // K
	{ {1,0,0}, {1,0,0}, {1,0,0}, {1,0,0}, {1,1,1} }, // L
	{ {1,0,1}, {1,1,1}, {1,1,1}, {1,0,1}, {1,0,1} }, // M
	{ {0,0,0}, {0,0,0}, {1,1,1}, {1,0,1}, {1,0,1} }, // N
	{ {0,0,0}, {0,0,0}, {1,1,1}, {1,0,1}, {1,1,1} }, // O
	{ {1,1,1}, {1,0,1}, {1,1,1}, {1,0,0}, {1,0,0} }, // P
	{ {1,1,1}, {1,0,1}, {1,1,1}, {0,0,1}, {0,0,1} }, // Q
	{ {0,0,0}, {1,0,1}, {1,1,0}, {1,0,0}, {1,0,0} }, // R
	{ {0,1,1}, {1,0,0}, {0,1,0}, {0,0,1}, {1,1,0} }, // S
	{ {1,1,1}, {0,1,0}, {0,1,0}, {0,1,0}, {0,1,0} }, // T
	{ {0,0,0}, {0,0,0}, {1,0,1}, {1,0,1}, {1,1,1} }, // U
	{ {0,0,0}, {0,0,0}, {1,0,1}, {1,1,1}, {0,1,0} }, // V
	{ {1,0,1}, {1,0,1}, {1,1,1}, {1,1,1}, {1,1,1} }, // W
	{ {0,0,0}, {0,0,0}, {1,0,1}, {0,1,0}, {1,0,1} }, // X
	{ {1,0,1}, {1,0,1}, {1,1,1}, {0,1,0}, {0,1,0} }, // Y
	{ {1,1,1}, {0,0,1}, {0,1,0}, {1,0,0}, {1,1,1} }, // Z
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 91
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 92
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 93
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 94
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 95
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 96
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 97
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 98
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 99
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 100
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 101
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 102
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 103
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 104
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 105
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 106
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 107
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 108
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 109
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 110
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 111
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 112
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 113
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 114
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 115
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 116
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 117
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 118
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 119
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 120
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 121
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 122
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 123
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 124
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 125
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 126
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 127
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 128
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 129
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 130
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 131
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 132
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 133
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 134
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 135
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 136
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 137
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 138
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 139
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 140
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 141
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 142
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 143
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 144
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 145
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 146
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 147
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 148
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 149
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 150
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 151
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 152
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 153
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 154
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 155
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 156
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 157
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 158
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 159
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 160
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 161
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 162
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 163
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 164
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 165
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 166
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 167
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 168
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 169
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 170
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 171
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 172
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 173
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 174
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 175
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 176
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 177
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 178
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 179
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 180
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 181
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 182
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 183
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 184
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 185
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 186
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 187
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 188
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 189
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 190
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 191
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 192
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 193
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 194
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 195
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 196
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 197
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 198
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 199
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 200
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 201
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 202
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 203
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 204
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 205
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 206
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 207
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 208
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 209
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 210
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 211
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 212
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 213
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 214
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 215
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 216
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 217
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 218
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 219
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 220
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 221
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 222
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 223
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 224
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 225
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 226
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 227
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 228
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 229
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 230
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 231
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 232
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 233
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 234
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 235
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 236
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 237
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 238
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 239
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 240
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 241
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 242
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 243
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 244
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 245
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 246
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 247
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 248
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 249
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 250
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 251
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 252
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 253
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // ascii 254
	{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }  // ascii 255
};

CLaserText::CLaserText(CGameWorld *pGameWorld, vec2 Pos, int Owner, int pAliveTicks, char* pText, int pTextLen)
: CEntity(pGameWorld, CGameWorld::ENTTYPE_LASER)
{
	m_Pos = Pos;
	m_Owner = Owner;
	GameWorld()->InsertEntity(this);

	m_CurTicks = Server()->Tick();
	m_StartTick = Server()->Tick();
	m_AliveTicks = pAliveTicks;

	m_TextLen = pTextLen;
	m_Text = new char[pTextLen];
	memcpy(m_Text, pText, pTextLen);

	m_CharNum = 0;

	for(int i = 0; i < m_TextLen; ++i){
		for(int n = 0; n < 5; ++n){
			for(int j = 0; j < 3; ++j){
				if(asciiTable[(unsigned char)m_Text[i]][n][j]){
					++m_CharNum;
				}
			}
		}
	}

	m_Chars = new CLaserChar*[m_CharNum];

	m_PosOffsetCharPoints = 15.0;
	m_PosOffsetChars = m_PosOffsetCharPoints * 3.5;

	int charCount = 0;
	for(int i = 0; i < m_TextLen; ++i){
		makeLaser(m_Text[i], i, charCount);
	}
}

CLaserText::CLaserText(CGameWorld *pGameWorld, vec2 Pos, int Owner, int pAliveTicks, char* pText, int pTextLen, float pCharPointOffset, float pCharOffsetFactor)
: CEntity(pGameWorld, CGameWorld::ENTTYPE_LASER)
{
	m_Pos = Pos;
	m_Owner = Owner;
	GameWorld()->InsertEntity(this);

	m_CurTicks = Server()->Tick();
	m_StartTick = Server()->Tick();
	m_AliveTicks = pAliveTicks;

	m_TextLen = pTextLen;
	m_Text = new char[pTextLen];
	memcpy(m_Text, pText, pTextLen);

	m_CharNum = 0;

	for(int i = 0; i < m_TextLen; ++i){
		for(int n = 0; n < 5; ++n){
			for(int j = 0; j < 3; ++j){
				if(asciiTable[(unsigned char)m_Text[i]][n][j]){
					++m_CharNum;
				}
			}
		}
	}

	m_Chars = new CLaserChar*[m_CharNum];

	m_PosOffsetCharPoints = pCharPointOffset;
	m_PosOffsetChars = m_PosOffsetCharPoints * pCharOffsetFactor;

	int charCount = 0;
	for(int i = 0; i < m_TextLen; ++i){
		makeLaser(m_Text[i], i, charCount);
	}
}

void CLaserText::Reset()
{
//	GameServer()->m_World.DestroyEntity(this);
	this->Destroy();
}

void CLaserText::Tick()
{
//	if(++m_CurTicks - m_StartTick > m_AliveTicks) GameServer()->m_World.DestroyEntity(this);
	if(++m_CurTicks - m_StartTick > m_AliveTicks) Reset();
}

void CLaserText::TickPaused()
{
}

inline char NeighboursVert(const bool pCharVert[3], int pVertOff){
	char neighbours = 0;
	if(pVertOff > 0){
		if(pCharVert[pVertOff - 1]) ++neighbours;
	}
	if(pVertOff < 2){
		if(pCharVert[pVertOff + 1]) ++neighbours;
	}

	return neighbours;
}

inline char NeighboursHor(const bool pCharHor[5][3], int pHorOff, int pVertOff){
	char neighbours = 0;
	if(pHorOff > 0){
		if(pCharHor[pHorOff - 1][pVertOff]) ++neighbours;
	}
	if(pHorOff < 4){
		if(pCharHor[pHorOff + 1][pVertOff]) ++neighbours;
	}

	return neighbours;
}

void CLaserText::makeLaser(char pChar, int pCharOffset, int& charCount){
	unsigned short tail[5][3];
	char neighbourCount[5][3];

	for(int n = 0; n < 5; ++n){
		for(int j = 0; j < 3; ++j){
			if(asciiTable[(unsigned char)pChar][n][j]){
				neighbourCount[n][j] = 0;
				neighbourCount[n][j] += NeighboursVert(asciiTable[(unsigned char)pChar][n], j);
				neighbourCount[n][j] += NeighboursHor(asciiTable[(unsigned char)pChar], n, j);
				tail[n][j] = 0;
			} else tail[n][j] = (unsigned short)-1;
		}
	}

	for(int n = 0; n < 5; ++n){
		for(int j = 0; j < 3; ++j){
			if(asciiTable[(unsigned char)pChar][n][j]){
				//additional x, y offset to draw a line
				int x = j, y = n;
				int maxNeighbour = 0;
				//forced line draw
				bool forceLine = false;

				if(j > 0){
					if(asciiTable[(unsigned char)pChar][n][j - 1]){
						if(tail[n][j - 1] != 0){
							if(tail[n][j - 1] != (n << 8 | j)){
								forceLine = true;
								tail[n][j] = (n << 8 | (j - 1));
								x = j - 1;
								y = n;
							}
						} else if(neighbourCount[n][j - 1] > maxNeighbour){
							maxNeighbour = neighbourCount[n][j - 1];
							x = j - 1;
							y = n;
						}
					}
				}
				if(!forceLine && j < 2){
					if(asciiTable[(unsigned char)pChar][n][j + 1]){
						if(tail[n][j + 1] != 0){
							if(tail[n][j + 1] != (n << 8 | j)){
								forceLine = true;
								tail[n][j] = (n << 8 | (j + 1));
								x = j + 1;
								y = n;
							}
						} else if(neighbourCount[n][j + 1] > maxNeighbour){
							maxNeighbour = neighbourCount[n][j + 1];
							x = j + 1;
							y = n;
						}
					}
				}
				if(!forceLine && n > 0){
					if(asciiTable[(unsigned char)pChar][n - 1][j]){
						if(tail[n - 1][j] != 0){
							if(tail[n - 1][j] != (n << 8 | j)){
								forceLine = true;
								tail[n][j] = ((n-1) << 8 | (j));
								x = j;
								y = n - 1;
							}
						} else if(neighbourCount[n - 1][j] > maxNeighbour){
							maxNeighbour = neighbourCount[n - 1][j];
							x = j;
							y = n - 1;
						}
					}
				}
				if(!forceLine && n < 4){
					if(asciiTable[(unsigned char)pChar][n + 1][j]){
						if(tail[n + 1][j] != 0){
							if(tail[n + 1][j] != (n << 8 | j)){
								forceLine = true;
								tail[n][j] = ((n+1) << 8 | (j));
								x = j;
								y = n + 1;
							}
						} else if(neighbourCount[n + 1][j] > maxNeighbour){
							maxNeighbour = neighbourCount[n + 1][j];
							x = j;
							y = n + 1;
						}
					}
				}

				if(!forceLine){
					tail[n][j] = (y << 8 | x);
				}

				CLaserChar* pObj = (m_Chars[charCount] = new CLaserChar(GameWorld()));

				pObj->m_Pos.x = m_Pos.x + pCharOffset * m_PosOffsetChars + j * m_PosOffsetCharPoints;
				pObj->m_Pos.y = m_Pos.y + n * m_PosOffsetCharPoints;
				pObj->m_Frompos.x = m_Pos.x + pCharOffset * m_PosOffsetChars + x * m_PosOffsetCharPoints;
				pObj->m_Frompos.y = m_Pos.y + y * m_PosOffsetCharPoints;

				++charCount;
			}
		}
	}
}

void CLaserText::Snap(int SnappingClient)
{
	if(NetworkClipped(SnappingClient))
		return;

	for(int i = 0; i < m_CharNum; ++i){
		CNetObj_Laser *pObj = static_cast<CNetObj_Laser *>(Server()->SnapNewItem(NETOBJTYPE_LASER, m_Chars[i]->getID(), sizeof(CNetObj_Laser)));
		if(!pObj)
			return;

		pObj->m_X = m_Chars[i]->m_Pos.x;
		pObj->m_Y = m_Chars[i]->m_Pos.y;
		pObj->m_FromX = m_Chars[i]->m_Frompos.x;
		pObj->m_FromY = m_Chars[i]->m_Frompos.y;
		pObj->m_StartTick = Server()->Tick();
	}
}
