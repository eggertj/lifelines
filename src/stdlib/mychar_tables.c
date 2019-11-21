#include "llstdlib.h"
#include "mychar.h"



/* a set of character properties for Latin1 (ISO-8859-1)
 fairly independent of language I believe -- 2002-11-05, Perry
 */
my_charset_info ISO_Latin1[] = {
  /*   0 0 */ {0, 0, 1, 0, 0},
  /*   1 1 */ {1, 1, 1, 0, 0},
  /*   2 2 */ {2, 2, 1, 0, 0},
  /*   3 3 */ {3, 3, 1, 0, 0},
  /*   4 4 */ {4, 4, 1, 0, 0},
  /*   5 5 */ {5, 5, 1, 0, 0},
  /*   6 6 */ {6, 6, 1, 0, 0},
  /*   7 7 */ {7, 7, 1, 0, 0},
  /*   8 8 */ {8, 8, 1, 0, 0},
  /*   9 9 */ {9, 9, 1, 0, 0},
  /*  10 10 */ {10, 10, 1, 0, 0},
  /*  11 11 */ {11, 11, 1, 0, 0},
  /*  12 12 */ {12, 12, 1, 0, 0},
  /*  13 13 */ {13, 13, 1, 0, 0},
  /*  14 14 */ {14, 14, 1, 0, 0},
  /*  15 15 */ {15, 15, 1, 0, 0},
  /*  16 16 */ {16, 16, 1, 0, 0},
  /*  17 17 */ {17, 17, 1, 0, 0},
  /*  18 18 */ {18, 18, 1, 0, 0},
  /*  19 19 */ {19, 19, 1, 0, 0},
  /*  20 20 */ {20, 20, 1, 0, 0},
  /*  21 21 */ {21, 21, 1, 0, 0},
  /*  22 22 */ {22, 22, 1, 0, 0},
  /*  23 23 */ {23, 23, 1, 0, 0},
  /*  24 24 */ {24, 24, 1, 0, 0},
  /*  25 25 */ {25, 25, 1, 0, 0},
  /*  26 26 */ {26, 26, 1, 0, 0},
  /*  27 27 */ {27, 27, 1, 0, 0},
  /*  28 28 */ {28, 28, 1, 0, 0},
  /*  29 29 */ {29, 29, 1, 0, 0},
  /*  30 30 */ {30, 30, 1, 0, 0},
  /*  31 31 */ {31, 31, 1, 0, 0},
  /*  32   */ {32, 32, 0, 0, 0},
  /*  33 ! */ {33, 33, 0, 0, 0},
  /*  34 " */ {34, 34, 0, 0, 0},
  /*  35 # */ {35, 35, 0, 0, 0},
  /*  36 $ */ {36, 36, 0, 0, 0},
  /*  37 % */ {37, 37, 0, 0, 0},
  /*  38 & */ {38, 38, 0, 0, 0},
  /*  39 ' */ {39, 39, 0, 0, 0},
  /*  40 ( */ {40, 40, 0, 0, 0},
  /*  41 ) */ {41, 41, 0, 0, 0},
  /*  42 * */ {42, 42, 0, 0, 0},
  /*  43 + */ {43, 43, 0, 0, 0},
  /*  44 , */ {44, 44, 0, 0, 0},
  /*  45 - */ {45, 45, 0, 0, 0},
  /*  46 . */ {46, 46, 0, 0, 0},
  /*  47 / */ {47, 47, 0, 0, 0},
  /*  48 0 */ {48, 48, 0, 0, 0},
  /*  49 1 */ {49, 49, 0, 0, 0},
  /*  50 2 */ {50, 50, 0, 0, 0},
  /*  51 3 */ {51, 51, 0, 0, 0},
  /*  52 4 */ {52, 52, 0, 0, 0},
  /*  53 5 */ {53, 53, 0, 0, 0},
  /*  54 6 */ {54, 54, 0, 0, 0},
  /*  55 7 */ {55, 55, 0, 0, 0},
  /*  56 8 */ {56, 56, 0, 0, 0},
  /*  57 9 */ {57, 57, 0, 0, 0},
  /*  58 : */ {58, 58, 0, 0, 0},
  /*  59 ; */ {59, 59, 0, 0, 0},
  /*  60 < */ {60, 60, 0, 0, 0},
  /*  61 = */ {61, 61, 0, 0, 0},
  /*  62 > */ {62, 62, 0, 0, 0},
  /*  63 ? */ {63, 63, 0, 0, 0},
  /*  64 @ */ {64, 64, 0, 0, 0},
  /*  65 A */ {65, 97, 0, 1, 0},
  /*  66 B */ {66, 98, 0, 1, 0},
  /*  67 C */ {67, 99, 0, 1, 0},
  /*  68 D */ {68, 100, 0, 1, 0},
  /*  69 E */ {69, 101, 0, 1, 0},
  /*  70 F */ {70, 102, 0, 1, 0},
  /*  71 G */ {71, 103, 0, 1, 0},
  /*  72 H */ {72, 104, 0, 1, 0},
  /*  73 I */ {73, 105, 0, 1, 0},
  /*  74 J */ {74, 106, 0, 1, 0},
  /*  75 K */ {75, 107, 0, 1, 0},
  /*  76 L */ {76, 108, 0, 1, 0},
  /*  77 M */ {77, 109, 0, 1, 0},
  /*  78 N */ {78, 110, 0, 1, 0},
  /*  79 O */ {79, 111, 0, 1, 0},
  /*  80 P */ {80, 112, 0, 1, 0},
  /*  81 Q */ {81, 113, 0, 1, 0},
  /*  82 R */ {82, 114, 0, 1, 0},
  /*  83 S */ {83, 115, 0, 1, 0},
  /*  84 T */ {84, 116, 0, 1, 0},
  /*  85 U */ {85, 117, 0, 1, 0},
  /*  86 V */ {86, 118, 0, 1, 0},
  /*  87 W */ {87, 119, 0, 1, 0},
  /*  88 X */ {88, 120, 0, 1, 0},
  /*  89 Y */ {89, 121, 0, 1, 0},
  /*  90 Z */ {90, 122, 0, 1, 0},
  /*  91 [ */ {91, 91, 0, 0, 0},
  /*  92 \ */ {92, 92, 0, 0, 0},
  /*  93 ] */ {93, 93, 0, 0, 0},
  /*  94 ^ */ {94, 94, 0, 0, 0},
  /*  95 _ */ {95, 95, 0, 0, 0},
  /*  96 ` */ {96, 96, 0, 0, 0},
  /*  97 a */ {65, 97, 0, 0, 1},
  /*  98 b */ {66, 98, 0, 0, 1},
  /*  99 c */ {67, 99, 0, 0, 1},
  /* 100 d */ {68, 100, 0, 0, 1},
  /* 101 e */ {69, 101, 0, 0, 1},
  /* 102 f */ {70, 102, 0, 0, 1},
  /* 103 g */ {71, 103, 0, 0, 1},
  /* 104 h */ {72, 104, 0, 0, 1},
  /* 105 i */ {73, 105, 0, 0, 1},
  /* 106 j */ {74, 106, 0, 0, 1},
  /* 107 k */ {75, 107, 0, 0, 1},
  /* 108 l */ {76, 108, 0, 0, 1},
  /* 109 m */ {77, 109, 0, 0, 1},
  /* 110 n */ {78, 110, 0, 0, 1},
  /* 111 o */ {79, 111, 0, 0, 1},
  /* 112 p */ {80, 112, 0, 0, 1},
  /* 113 q */ {81, 113, 0, 0, 1},
  /* 114 r */ {82, 114, 0, 0, 1},
  /* 115 s */ {83, 115, 0, 0, 1},
  /* 116 t */ {84, 116, 0, 0, 1},
  /* 117 u */ {85, 117, 0, 0, 1},
  /* 118 v */ {86, 118, 0, 0, 1},
  /* 119 w */ {87, 119, 0, 0, 1},
  /* 120 x */ {88, 120, 0, 0, 1},
  /* 121 y */ {89, 121, 0, 0, 1},
  /* 122 z */ {90, 122, 0, 0, 1},
  /* 123 { */ {123, 123, 0, 0, 0},
  /* 124 | */ {124, 124, 0, 0, 0},
  /* 125 } */ {125, 125, 0, 0, 0},
  /* 126 ~ */ {126, 126, 0, 0, 0},
  /* 127 127 */ {127, 127, 1, 0, 0},
  /* 128 128 */ {128, 128, 1, 0, 0},
  /* 129 129 */ {129, 129, 1, 0, 0},
  /* 130 130 */ {130, 130, 1, 0, 0},
  /* 131 131 */ {131, 131, 1, 0, 0},
  /* 132 132 */ {132, 132, 1, 0, 0},
  /* 133 133 */ {133, 133, 1, 0, 0},
  /* 134 134 */ {134, 134, 1, 0, 0},
  /* 135 135 */ {135, 135, 1, 0, 0},
  /* 136 136 */ {136, 136, 1, 0, 0},
  /* 137 137 */ {137, 137, 1, 0, 0},
  /* 138 138 */ {138, 138, 1, 0, 0},
  /* 139 139 */ {139, 139, 1, 0, 0},
  /* 140 140 */ {140, 140, 1, 0, 0},
  /* 141 141 */ {141, 141, 1, 0, 0},
  /* 142 142 */ {142, 142, 1, 0, 0},
  /* 143 143 */ {143, 143, 1, 0, 0},
  /* 144 144 */ {144, 144, 1, 0, 0},
  /* 145 145 */ {145, 145, 1, 0, 0},
  /* 146 146 */ {146, 146, 1, 0, 0},
  /* 147 147 */ {147, 147, 1, 0, 0},
  /* 148 148 */ {148, 148, 1, 0, 0},
  /* 149 149 */ {149, 149, 1, 0, 0},
  /* 150 150 */ {150, 150, 1, 0, 0},
  /* 151 151 */ {151, 151, 1, 0, 0},
  /* 152 152 */ {152, 152, 1, 0, 0},
  /* 153 153 */ {153, 153, 1, 0, 0},
  /* 154 154 */ {154, 154, 1, 0, 0},
  /* 155 155 */ {155, 155, 1, 0, 0},
  /* 156 156 */ {156, 156, 1, 0, 0},
  /* 157 157 */ {157, 157, 1, 0, 0},
  /* 158 158 */ {158, 158, 1, 0, 0},
  /* 159 159 */ {159, 159, 1, 0, 0},
  /* 160 160 */ {160, 160, 1, 0, 0}, /* u00A0: NO-BREAK SPACE */
  /* 161 � */ {161, 161, 0, 0, 0}, /* u00A1: INVERTED EXCLAMATION MARK */
  /* 162 � */ {162, 162, 0, 0, 0}, /* u00A2: CENT SIGN */
  /* 163 � */ {163, 163, 0, 0, 0}, /* u00A3: POUND SIGN */
  /* 164 � */ {164, 164, 0, 0, 0},
  /* 165 � */ {165, 165, 0, 0, 0},
  /* 166 � */ {166, 166, 0, 0, 0},
  /* 167 � */ {167, 167, 0, 0, 0},
  /* 168 � */ {168, 168, 0, 0, 0},
  /* 169 � */ {169, 169, 0, 0, 0},
  /* 170 � */ {170, 170, 0, 0, 0},
  /* 171 � */ {171, 171, 0, 0, 0},
  /* 172 � */ {172, 172, 0, 0, 0},
  /* 173 � */ {173, 173, 0, 0, 0},
  /* 174 � */ {174, 174, 0, 0, 0},
  /* 175 � */ {175, 175, 0, 0, 0},
  /* 176 � */ {176, 176, 0, 0, 0},
  /* 177 � */ {177, 177, 0, 0, 0},
  /* 178 � */ {178, 178, 0, 0, 0},
  /* 179 � */ {179, 179, 0, 0, 0},
  /* 180 � */ {180, 180, 0, 0, 0},
  /* 181 � */ {181, 181, 0, 0, 0},
  /* 182 � */ {182, 182, 0, 0, 0},
  /* 183 � */ {183, 183, 0, 0, 0},
  /* 184 � */ {184, 184, 0, 0, 0},
  /* 185 � */ {185, 185, 0, 0, 0},
  /* 186 � */ {186, 186, 0, 0, 0},
  /* 187 � */ {187, 187, 0, 0, 0},
  /* 188 � */ {188, 188, 0, 0, 0},
  /* 189 � */ {189, 189, 0, 0, 0},
  /* 190 � */ {190, 190, 0, 0, 0},
  /* 191 � */ {191, 191, 0, 0, 0},
  /* 192 � */ {192, 224, 0, 1, 0},
  /* 193 � */ {193, 225, 0, 1, 0},
  /* 194 � */ {194, 226, 0, 1, 0},
  /* 195 � */ {195, 227, 0, 1, 0},
  /* 196 � */ {196, 228, 0, 1, 0},
  /* 197 � */ {197, 229, 0, 1, 0},
  /* 198 � */ {198, 230, 0, 1, 0},
  /* 199 � */ {199, 231, 0, 1, 0},
  /* 200 � */ {200, 232, 0, 1, 0},
  /* 201 � */ {201, 233, 0, 1, 0},
  /* 202 � */ {202, 234, 0, 1, 0},
  /* 203 � */ {203, 235, 0, 1, 0},
  /* 204 � */ {204, 236, 0, 1, 0},
  /* 205 � */ {205, 237, 0, 1, 0},
  /* 206 � */ {206, 238, 0, 1, 0},
  /* 207 � */ {207, 239, 0, 1, 0},
  /* 208 � */ {208, 240, 0, 1, 0},
  /* 209 � */ {209, 241, 0, 1, 0},
  /* 210 � */ {210, 242, 0, 1, 0},
  /* 211 � */ {211, 243, 0, 1, 0},
  /* 212 � */ {212, 244, 0, 1, 0},
  /* 213 � */ {213, 245, 0, 1, 0},
  /* 214 � */ {214, 246, 0, 1, 0},
  /* 215 � */ {215, 215, 0, 0, 0},
  /* 216 � */ {216, 248, 0, 1, 0},
  /* 217 � */ {217, 249, 0, 1, 0},
  /* 218 � */ {218, 250, 0, 1, 0},
  /* 219 � */ {219, 251, 0, 1, 0},
  /* 220 � */ {220, 252, 0, 1, 0},
  /* 221 � */ {221, 253, 0, 1, 0},
  /* 222 � */ {222, 254, 0, 1, 0},
  /* 223 � */ {223, 223, 0, 0, 1},
  /* 224 � */ {192, 224, 0, 0, 1},
  /* 225 � */ {193, 225, 0, 0, 1},
  /* 226 � */ {194, 226, 0, 0, 1},
  /* 227 � */ {195, 227, 0, 0, 1},
  /* 228 � */ {196, 228, 0, 0, 1},
  /* 229 � */ {197, 229, 0, 0, 1},
  /* 230 � */ {198, 230, 0, 0, 1},
  /* 231 � */ {199, 231, 0, 0, 1},
  /* 232 � */ {200, 232, 0, 0, 1},
  /* 233 � */ {201, 233, 0, 0, 1},
  /* 234 � */ {202, 234, 0, 0, 1},
  /* 235 � */ {203, 235, 0, 0, 1},
  /* 236 � */ {204, 236, 0, 0, 1},
  /* 237 � */ {205, 237, 0, 0, 1},
  /* 238 � */ {206, 238, 0, 0, 1},
  /* 239 � */ {207, 239, 0, 0, 1},
  /* 240 � */ {208, 240, 0, 0, 1},
  /* 241 � */ {209, 241, 0, 0, 1},
  /* 242 � */ {210, 242, 0, 0, 1},
  /* 243 � */ {211, 243, 0, 0, 1},
  /* 244 � */ {212, 244, 0, 0, 1},
  /* 245 � */ {213, 245, 0, 0, 1},
  /* 246 � */ {214, 246, 0, 0, 1},
  /* 247 � */ {247, 247, 0, 0, 0},
  /* 248 � */ {216, 248, 0, 0, 1},
  /* 249 � */ {217, 249, 0, 0, 1},
  /* 250 � */ {218, 250, 0, 0, 1},
  /* 251 � */ {219, 251, 0, 0, 1},
  /* 252 � */ {220, 252, 0, 0, 1},
  /* 253 � */ {221, 253, 0, 0, 1},
  /* 254 � */ {222, 254, 0, 0, 1},
  /* 255 � */ {255, 255, 0, 0, 1},
};
