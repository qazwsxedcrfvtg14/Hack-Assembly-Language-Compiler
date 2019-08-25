class Output {
public:
  static const int Gray_Mask = 50;
  static Var Init() { return Call(Output_Init); }
  static Var Clean() { return Call(Output_Clean); }
  static Var Update() { return Call(Output_Update); }
  template <typename T0, typename T1, typename T2, typename T3, typename T4>
  static Var DrawRect(T0 x1, T1 x2, T2 y1, T3 y2, T4 col) {
    return Call(Output_DrawRect, x1, x2, y1, y2, col);
  }
  template <typename T0, typename T1, typename T2, typename T3>
  static Var DrawVerticalLine(T0 x, T1 y1, T2 y2, T3 col) {
    return Call(Output_DrawVerticalLine, x, y1, y2, col);
  }
  template <typename T0, typename T1, typename T2>
  static Var DrawNumber(T0 n, T1 x, T2 y) {
    return Call(Output_DrawNumber, n, x, y);
  }
  static const int charset = 0x240;
  static const int charset_mem = 0xA00;
  static int charset_mem_top;
  static void Create(int index, int a, int b, int c, int d, int e, int f, int g,
                     int h, int i, int j) {
    mem[charset + index] = charset_mem_top;
    mem[charset_mem_top + 0] = a;
    mem[charset_mem_top + 1] = b;
    mem[charset_mem_top + 2] = c;
    mem[charset_mem_top + 3] = d;
    mem[charset_mem_top + 4] = e;
    mem[charset_mem_top + 5] = f;
    mem[charset_mem_top + 6] = g;
    mem[charset_mem_top + 7] = h;
    mem[charset_mem_top + 8] = i;
    mem[charset_mem_top + 9] = j;
    charset_mem_top += 10;
  }
  template <typename T0> static Var GetMap(T0 c) {
    return Call(Output_GetMap, c);
  }
  template <typename T0, typename T1> static Var MoveCursor(T0 x, T1 y) {
    return Call(Output_MoveCursor, x, y);
  }
  template <typename T0> static Var PrintChar(T0 c) {
    return Call(Output_PrintChar, c);
  }
  static Var PrintLine() { return Call(Output_PrintLine); }
  template <typename T0> static Var Scroll(T0 d) {
    return Call(Output_Scroll, d);
  }
  template <typename T0> static Var PrintString(T0 s) {
    return Call(Output_PrintString, s);
  }
};
class String {
public:
  template <typename T0> static void Set(T0 p, string s) {
    Global["String_set"] = p;
    for (char c : s) {
      put("@" + ToStr((int)c));
      put("D=A");
      Global["String_set"].toA();
      put("M=D");
      Global["String_set"]++;
    }
    Global["String_set"].toA();
    put("M=0");
  }
};
int Output::charset_mem_top;
class Input {
public:
  static void Update() {
    Global["KBD2"] = mem[KBD];
    If(Global["KBD2"], !=, 0) Global["KBD"] = Global["KBD2"];
    End
  }
  static void Clean() { Global["KBD"] = 0; }
  static obj Key() { return Global["KBD"]; }
  static Var WaitEnter() { return Call(Input_WaitEnter); }
};
class Math {
public:
  static void Init() {
    mem[0] = 1;
    mem[1] = 2;
    mem[2] = 4;
    mem[3] = 8;
    mem[4] = 16;
    mem[5] = 32;
    mem[6] = 64;
    mem[7] = 128;
    mem[8] = 256;
    mem[9] = 512;
    mem[10] = 1024;
    mem[11] = 2048;
    mem[12] = 4096;
    mem[13] = 8192;
    mem[14] = 16384;
    mem[15] = -32768;
  }
  template <typename T0, typename T1> static Var Bit(T0 x, T1 n) {
    return Call(Math_Bit, x, n);
  }
  template <typename T0> static Var TwoToThe(T0 n) {
    return Call(Math_TwoToThe, n);
  }
  template <typename T0> static Var Abs(T0 x) { return Call(Math_Abs, x); }
  template <typename T0, typename T1> static Var LeftShift(T0 x, T1 y) {
    return Call(Math_LeftShift, x, y);
  }
  template <typename T0> static Var Lowbit(T0 x) {
    return Call(Math_Lowbit, x);
  }
  template <typename T0, typename T1> static Var Xor(T0 x, T1 y) {
    return Call(Math_Xor, x, y);
  }
  template <typename T0, typename T1> static Var Multiply(T0 x, T1 y) {
    return Call(Math_Multiply, x, y);
  }
  template <typename T0, typename T1> static Var Divide(T0 x, T1 y) {
    return Call(Math_Divide, x, y);
  }
  template <typename T0, typename T1> static Var Mod(T0 x, T1 y) {
    return Call(Math_Mod, x, y);
  }
  template <typename T0, typename T1> static Var Max(T0 x, T1 y) {
    return Call(Math_Max, x, y);
  }
  template <typename T0, typename T1> static Var Min(T0 x, T1 y) {
    return Call(Math_Min, x, y);
  }
};

void obj::operator*=(int x) {
  if (x == 1)
    return;
  else if (x == -1) {
    put("@" + ToStr(id));
    run_star();
    put("M=-M");
  } else if (x == 0) {
    put("@" + ToStr(id));
    run_star();
    put("M=0");
  } else if (__builtin_popcount(x) == 1) {
    put("@" + ToStr(id));
    run_star();
    int y = __lg(x);
    for (int i = 0; i < y; i++)
      put("D=M"), put("M=M+D");
  } else {
    Math::Divide(*this, x).toD();
    put("@" + ToStr(id));
    run_star();
    put("M=D");
    return;
    // throw "error: *= only support -1,0,1";
  }
}
void obj::operator*=(const obj &x) {
  Math::Multiply(*this, x).toD();
  put("@" + ToStr(id));
  run_star();
  put("M=D");
  return;
}
void obj::operator/=(int x) {
  Math::Divide(*this, x).toD();
  put("@" + ToStr(id));
  run_star();
  put("M=D");
}
void obj::operator/=(const obj &x) {
  Math::Divide(*this, x).toD();
  put("@" + ToStr(id));
  run_star();
  put("M=D");
}
void obj::operator%=(int x) {
  Math::Mod(*this, x).toD();
  put("@" + ToStr(id));
  run_star();
  put("M=D");
}
void obj::operator%=(const obj &x) {
  Math::Mod(*this, x).toD();
  put("@" + ToStr(id));
  run_star();
  put("M=D");
}
void obj::operator^=(int x) {
  Math::Xor(*this, x).toD();
  put("@" + ToStr(id));
  run_star();
  put("M=D");
}
void obj::operator^=(const obj &x) {
  Math::Xor(*this, x).toD();
  put("@" + ToStr(id));
  run_star();
  put("M=D");
}
class Array {
private:
  static const int mask11 = -16384;
  static const int umask11 = 16383;
  static const int mask10 = -32768;
  static const int umask10 = 32767;
  static const int mask01 = 16384;
  static const int umask01 = -16385;

public:
  static void Init() {
    mem[Array_Pool_Begin] = (Array_Pool_Size - 2) | mask01;
    mem[Array_Pool_Begin + Array_Pool_Size - 1] = 0x3fff | mask10;
  }
  template <typename T0> static Var Alloc(T0 size) {
    return Call(Array_Alloc, size);
  }
  template <typename T0> static void Free(T0 p) { Call(Array_Free, p); }
};
