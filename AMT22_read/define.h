
#include "Arduino.h"

#define PIN_CSB1 4
#define PIN_CSB2 29
// AMT203のCSBピンは，10,68,69,70番は使用できない．
//他にも使用できないピンがある可能性有，読めない場合はピンを変えてみること．

#define DSW_1 46
#define DSW_2 47

// 制御周期
#define INT_TIME (0.01) //( 0.001 )
