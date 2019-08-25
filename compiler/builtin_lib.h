{obj i=glo["tmp_obj0"],j=glo["tmp_obj1"];
Func(Output_Clean)
    j=ScreenBuffer;
    Loop
        mem[j]=0;
        j++;
        If(j,==,ScreenBuffer+0x2000)
            Break;
        End
    End
End
Func(Output_Update)
    Global["gray"]++;
/*
    i=0x4000;
    j=ScreenBuffer;
    Loop
        mem[i]=mem[j];
        j++;
        i++;
        If(i,==,0x6000)
            Break;
        End
    End
*/
End
{obj x1_high=glo["tmp_obj2"],x1_low=glo["tmp_obj3"],x2_high=glo["tmp_obj4"],x2_low=glo["tmp_obj5"],pos=glo["tmp_obj6"],line_begin=glo["tmp_obj7"],bi=glo["tmp_obj8"];
obj y11=glo["tmp_obj9"],y22=glo["tmp_obj10"],gry=glo["tmp_obj11"],y32=glo["tmp_obj12"];
Func(Output_DrawRect,x1,x2,y1,y2,col)
    y11=y1;
    y22=y2;
    If(x1,>,x2)
        Return(0);
    End
    If(y11,>,y22)
        Return(0);
    End
    x1_low=x1;
    x1_low&=15;
    x1_low+=16;
    x1_low=Mem[x1_low];
    x1_high=x1;
    x1_high&=-16;
    x1_high+=48;
    x1_high=Mem[x1_high];
    x2_low=x2;
    x2_low&=15;
    x2_low+=32;
    x2_low=Mem[x2_low];
    x2_high=x2;
    x2_high&=-16;
    x2_high+=48;
    x2_high=Mem[x2_high];
    If(x1_high,==,x2_high)
        x1_low&=x2_low;
        x2_low=x1_low;
    End
    If(col,<=,0)
        x1_low.nt();
        x2_low.nt();
    End
    line_begin=ScreenBuffer;
    y32=y1;
    y32*=32;
    line_begin+=y32;
    Loop
        bi=y11;
        bi+=Global["gray"];
        bi&=1;
        gry=Output::Gray_Mask;
        gry+=bi;
        If(col,==,2)
            pos=line_begin;
            pos+=x2_high;
            mem[pos]|=x2_low;
            mem[pos]&=mem[gry];
            pos=line_begin;
            pos+=x1_high;
            mem[pos]|=x1_low;
            mem[pos]&=mem[gry];
            i=x1_high;
            Loop
                pos++;
                i++;
                If(i,>=,x2_high)
                    Break
                End
                mem[pos]=mem[gry];
            End
        End
        If(col,==,1)
            pos=line_begin;
            pos+=x2_high;
            mem[pos]|=x2_low;
            pos=line_begin;
            pos+=x1_high;
            mem[pos]|=x1_low;
            i=x1_high;
            Loop
                pos++;
                i++;
                If(i,>=,x2_high)
                    Break
                End
                mem[pos]=-1;
            End
        End
        If(col,==,0)
            pos=line_begin;
            pos+=x2_high;
            mem[pos]&=x2_low;
            pos=line_begin;
            pos+=x1_high;
            mem[pos]&=x1_low;
            i=x1_high;
            Loop
                pos++;
                i++;
                If(i,>=,x2_high)
                    Break
                End
                mem[pos]=0;
            End
        End
        If(col,==,-1)
            pos=line_begin;
            pos+=x2_high;
            mem[pos]&=x2_low;
            mem[pos]|=mem[gry];
            pos=line_begin;
            pos+=x1_high;
            mem[pos]&=x1_low;
            mem[pos]|=mem[gry];
            i=x1_high;
            Loop
                pos++;
                i++;
                If(i,>=,x2_high)
                    Break
                End
                mem[pos]=mem[gry];
            End
        End
        line_begin+=32;
        y11++;
        If(y11,>,y22)
            Break
        End
    End
End
}}
Func(Output_DrawVerticalLine,x,y1,y2,col)
    Call(Output_DrawRect,x,x,y1,y2,col);
End
Func(Output_DrawNumber,n,x,y)
    Var pos;
    pos=0x0600;
    pos+=n;
    pos=mem[pos];
    Loop
        Var a,b,c;
        a=mem[pos];
        If(a,==,-1)
            Break;
        End
        pos++;
        b=mem[pos];
        pos++;
        c=mem[pos];
        pos++;
        a+=x;
        b+=y;
        c+=y;
        Call(Output_DrawVerticalLine,a,b,c,0);
    End
End
Func(Input_WaitEnter)
    Loop
        If(mem[KBD],==,128)
            Break
        End
    End
End
Func(Output_Init)
    for(int i=0;i<16;i++)
        Mem[i+16]=-(1<<i);
    //Mem[32]=-1;
    for(int i=0;i<15;i++)
        Mem[i+32]=(1<<i+1)-1;
    Mem[47]=-1;
    for(int i=0;i<32;i++)
        Mem[(i<<4)+48]=i;
    mem[Output::Gray_Mask]=21845;
    mem[Output::Gray_Mask+1]=-21846;
    Global["Output_cursor_x"]=0;
    Global["Output_cursor_y"]=0;
    Output::charset_mem_top=Output::charset_mem;

    Output::Create(32,0,0,0,0,0,0,0,0,0,0);          //
    Output::Create(33,12,30,30,30,12,12,0,12,12,0);  // !
    Output::Create(34,54,54,20,0,0,0,0,0,0,0);       // "
    Output::Create(35,0,18,18,63,18,18,63,18,18,0);  // #
    Output::Create(36,12,30,51,3,30,48,51,30,12,12); // $
    Output::Create(37,0,0,35,51,24,12,6,51,49,0);    // %
    Output::Create(38,12,30,30,12,54,27,27,27,54,0); // &
    Output::Create(39,12,12,6,0,0,0,0,0,0,0);        // '
    Output::Create(40,24,12,6,6,6,6,6,12,24,0);      // (
    Output::Create(41,6,12,24,24,24,24,24,12,6,0);   // )
    Output::Create(42,0,0,0,51,30,63,30,51,0,0);     // *
    Output::Create(43,0,0,0,12,12,63,12,12,0,0);     // +
    Output::Create(44,0,0,0,0,0,0,0,12,12,6);        // ,
    Output::Create(45,0,0,0,0,0,63,0,0,0,0);         // -
    Output::Create(46,0,0,0,0,0,0,0,12,12,0);        // .
    Output::Create(47,0,0,32,48,24,12,6,3,1,0);      // /

    Output::Create(48,12,30,51,51,51,51,51,30,12,0); // 0
    Output::Create(49,12,14,15,12,12,12,12,12,63,0); // 1
    Output::Create(50,30,51,48,24,12,6,3,51,63,0);   // 2
    Output::Create(51,30,51,48,48,28,48,48,51,30,0); // 3
    Output::Create(52,16,24,28,26,25,63,24,24,60,0); // 4
    Output::Create(53,63,3,3,31,48,48,48,51,30,0);   // 5
    Output::Create(54,28,6,3,3,31,51,51,51,30,0);    // 6
    Output::Create(55,63,49,48,48,24,12,12,12,12,0); // 7
    Output::Create(56,30,51,51,51,30,51,51,51,30,0); // 8
    Output::Create(57,30,51,51,51,62,48,48,24,14,0); // 9

    Output::Create(58,0,0,12,12,0,0,12,12,0,0);      // :
    Output::Create(59,0,0,12,12,0,0,12,12,6,0);      // ;
    Output::Create(60,0,0,24,12,6,3,6,12,24,0);      // <
    Output::Create(61,0,0,0,63,0,0,63,0,0,0);        // =
    Output::Create(62,0,0,3,6,12,24,12,6,3,0);       // >
    Output::Create(64,30,51,51,59,59,59,27,3,30,0);  // @
    Output::Create(63,30,51,51,24,12,12,0,12,12,0);  // ?

    Output::Create(65,12,30,51,51,63,51,51,51,51,0); // A
    Output::Create(66,31,51,51,51,31,51,51,51,31,0); // B
    Output::Create(67,28,54,35,3,3,3,35,54,28,0);    // C
    Output::Create(68,15,27,51,51,51,51,51,27,15,0); // D
    Output::Create(69,63,51,35,11,15,11,35,51,63,0); // E
    Output::Create(70,63,51,35,11,15,11,3,3,3,0);    // F
    Output::Create(71,28,54,35,3,59,51,51,54,44,0);  // G
    Output::Create(72,51,51,51,51,63,51,51,51,51,0); // H
    Output::Create(73,30,12,12,12,12,12,12,12,30,0); // I
    Output::Create(74,60,24,24,24,24,24,27,27,14,0); // J
    Output::Create(75,51,51,51,27,15,27,51,51,51,0); // K
    Output::Create(76,3,3,3,3,3,3,35,51,63,0);       // L
    Output::Create(77,33,51,63,63,51,51,51,51,51,0); // M
    Output::Create(78,51,51,55,55,63,59,59,51,51,0); // N
    Output::Create(79,30,51,51,51,51,51,51,51,30,0); // O
    Output::Create(80,31,51,51,51,31,3,3,3,3,0);     // P
    Output::Create(81,30,51,51,51,51,51,63,59,30,48);// Q
    Output::Create(82,31,51,51,51,31,27,51,51,51,0); // R
    Output::Create(83,30,51,51,6,28,48,51,51,30,0);  // S
    Output::Create(84,63,63,45,12,12,12,12,12,30,0); // T
    Output::Create(85,51,51,51,51,51,51,51,51,30,0); // U
    Output::Create(86,51,51,51,51,51,30,30,12,12,0); // V
    Output::Create(87,51,51,51,51,51,63,63,63,18,0); // W
    Output::Create(88,51,51,30,30,12,30,30,51,51,0); // X
    Output::Create(89,51,51,51,51,30,12,12,12,30,0); // Y
    Output::Create(90,63,51,49,24,12,6,35,51,63,0);  // Z

    Output::Create(91,30,6,6,6,6,6,6,6,30,0);          // [
    Output::Create(92,0,0,1,3,6,12,24,48,32,0);        //
    Output::Create(93,30,24,24,24,24,24,24,24,30,0);   // ]
    Output::Create(94,8,28,54,0,0,0,0,0,0,0);          // ^
    Output::Create(95,0,0,0,0,0,0,0,0,0,63);           // _
    Output::Create(96,6,12,24,0,0,0,0,0,0,0);          // `

    Output::Create(97,0,0,0,14,24,30,27,27,54,0);      // a
    Output::Create(98,3,3,3,15,27,51,51,51,30,0);      // b
    Output::Create(99,0,0,0,30,51,3,3,51,30,0);        // c
    Output::Create(100,48,48,48,60,54,51,51,51,30,0);  // d
    Output::Create(101,0,0,0,30,51,63,3,51,30,0);      // e
    Output::Create(102,28,54,38,6,15,6,6,6,15,0);      // f
    Output::Create(103,0,0,30,51,51,51,62,48,51,30);   // g
    Output::Create(104,3,3,3,27,55,51,51,51,51,0);     // h
    Output::Create(105,12,12,0,14,12,12,12,12,30,0);   // i
    Output::Create(106,48,48,0,56,48,48,48,48,51,30);  // j
    Output::Create(107,3,3,3,51,27,15,15,27,51,0);     // k
    Output::Create(108,14,12,12,12,12,12,12,12,30,0);  // l
    Output::Create(109,0,0,0,29,63,43,43,43,43,0);     // m
    Output::Create(110,0,0,0,29,51,51,51,51,51,0);     // n
    Output::Create(111,0,0,0,30,51,51,51,51,30,0);     // o
    Output::Create(112,0,0,0,30,51,51,51,31,3,3);      // p
    Output::Create(113,0,0,0,30,51,51,51,62,48,48);    // q
    Output::Create(114,0,0,0,29,55,51,3,3,7,0);        // r
    Output::Create(115,0,0,0,30,51,6,24,51,30,0);      // s
    Output::Create(116,4,6,6,15,6,6,6,54,28,0);        // t
    Output::Create(117,0,0,0,27,27,27,27,27,54,0);     // u
    Output::Create(118,0,0,0,51,51,51,51,30,12,0);     // v
    Output::Create(119,0,0,0,51,51,51,63,63,18,0);     // w
    Output::Create(120,0,0,0,51,30,12,12,30,51,0);     // x
    Output::Create(121,0,0,0,51,51,51,62,48,24,15);    // y
    Output::Create(122,0,0,0,63,27,12,6,51,63,0);      // z

    Output::Create(123,56,12,12,12,7,12,12,12,56,0);   // {
    Output::Create(124,12,12,12,12,12,12,12,12,12,0);  // |
    Output::Create(125,7,12,12,12,56,12,12,12,7,0);    // }
    Output::Create(126,38,45,25,0,0,0,0,0,0,0);        // ~

End

//512x256
Func(Output_GetMap,c)
    c+=Output::charset;
    Return(mem[c]);
End
Func(Output_MoveCursor,x,y)
    Global["Output_cursor_x"]=x;
    Global["Output_cursor_y"]=y;
End
{obj tmp=glo["tmp_obj0"],tmp2=glo["tmp_obj1"],i=glo["tmp_obj2"],msk=glo["tmp_obj3"],y32=glo["tmp_obj4"],line_begin=glo["tmp_obj5"],high=glo["tmp_obj6"],mk=glo["tmp_obj7"];
obj y1=glo["tmp_obj8"],y2=glo["tmp_obj9"],dis=glo["tmp_obj10"],pos=glo["tmp_obj11"];
Func(Output_PrintChar,c)
    If(Global["Output_cursor_x"],==,512)
        Output::PrintLine();
    End
    If(c,==,'\n')
        Output::PrintLine();
        Return(0);
    End
    If(c,==,128)
        Output::PrintLine();
        Return(0);
    End
    c+=Output::charset;
    tmp2=mem[c];
    tmp=Global["Output_cursor_x"];
    tmp&=8;
    msk=-256;
    high=0;
    If(tmp,!=,0)
        high=1;
        msk=255;
    End
    tmp=Global["Output_cursor_x"];
    tmp&=-16;
    tmp+=48;
    tmp=mem[tmp];

    line_begin=ScreenBuffer;
    y32=Global["Output_cursor_y"];
    y32*=32;
    line_begin+=y32;
    line_begin+=tmp;
    i=0;
    Loop
        If(i,==,10)
            Break;
        End
        mk=mem[tmp2];
        If(high,!=,0)
            mk*=1<<8;
        End
        mk*=2;
        mem[line_begin]&=msk;
        mem[line_begin]|=mk;
        line_begin+=32;
        tmp2++;
        i++;
    End
    Global["Output_cursor_x"]+=8;
End
Func(Output_PrintLine)
    Global["Output_cursor_x"]=0;
    Global["Output_cursor_y"]+=12;
    If(Global["Output_cursor_y"],>,256-12)
        Global["Output_cursor_y"]-=256-12;
        Output::Scroll(Global["Output_cursor_y"]);
        Global["Output_cursor_y"]=256-12;
    End
End

Func(Output_Scroll,d)
    dis=d;
    dis*=32;
    y1=ScreenBuffer;
    y2=ScreenBuffer;
    y2+=dis;
    Loop
        mem[y1]=mem[y2];
        y1++;
        y2++;
        If(y2,==,ScreenBuffer+0x2000)
            Break
        End
    End
    Loop
        mem[y1]=0;
        y1++;
        If(y1,==,ScreenBuffer+0x2000)
            Break
        End
    End
End

Func(Output_PrintString,s)
    pos=s;
    Loop
        If(mem[pos],==,0)
            Break
        End
        Output::PrintChar(mem[pos]);
        pos++;
    End
End
}

{obj pos=glo["tmp_obj0"],val=glo["tmp_obj1"],tmp=glo["tmp_obj2"],sz=glo["tmp_obj3"],pos2=glo["tmp_obj3"],tmp2=glo["tmp_obj4"];
//const int mask11=-16384;
const int umask11=16383;
const int mask10=-32768;
//const int umask10=32767;
const int mask01=16384;
const int umask01=-16385;
Func(Array_Alloc,size)
    sz=size;
    pos=Array_Pool_Begin;
    Loop
        val=mem[pos];
        tmp=val;
        tmp&=mask10;
        val&=umask11;
        If(tmp,==,0)
            If(val,>=,sz)
                Break
            End
        End
        pos+=val;
        pos++;
    End
    tmp=mem[pos];
    tmp&=mask01;
    tmp2=sz;
    tmp2|=tmp;
    tmp2|=mask10;
    mem[pos]=tmp2;
    pos+=sz;
    pos++;
    val-=sz;
    val--;
    If(val,<,0)
        val=mem[pos];
    End
    val|=mask01;
    mem[pos]=val;
    pos-=sz;
    Return(pos);
End
Func(Array_Free,p)
    p--;
    pos=p;
    val=mem[pos];
    tmp=val;
    tmp&=mask01;
    val&=umask11;
    If(tmp,==,0)
        pos--;
        pos=mem[pos];
    End
    pos2=p;
    Loop
        pos2+=val;
        pos2++;
        val=mem[pos2];
        tmp=val;
        tmp&=mask10;
        val&=umask11;
        If(tmp,!=,0)
            Break
        End
    End
    val=pos2;
    val-=pos;
    val--;
    tmp=mem[pos];
    tmp&=mask01;
    val|=tmp;
    mem[pos]=val;
    mem[pos2]&=umask01;
    pos2--;
    mem[pos2]=pos;
End
}
{obj tmp=glo["tmp_obj0"],tmp2=glo["tmp_obj1"],tmp3=glo["tmp_obj2"],tmp4=glo["tmp_obj3"],tmp5=glo["tmp_obj4"];
Func(Math_Bit,x,n)
    tmp=x;
    tmp&=mem[n];
    If(tmp,!=,0)
        Return(1);
    End
    Return(0);
End
Func(Math_TwoToThe,n)
    Return(mem[n]);
End
Func(Math_Abs,x)
    tmp=x;
    If(tmp,>=,0)
        Return(tmp);
    End
    tmp*=-1;
    Return(tmp);
End
Func(Math_LeftShift,x,y)
    tmp=0;
    tmp2=x;
    Loop
        If(tmp,>=,y)
            Break
        End
        tmp2+=tmp2;
    End
    Return(tmp2);
End
Func(Math_Lowbit,x)
    tmp=x;
    tmp2=x;
    tmp2*=-1;
    tmp&=tmp2;
    Return(tmp);
End
Func(Math_Xor,x,y)
    tmp=x;
    tmp2=y;
    tmp2.nt();
    tmp3=tmp;
    tmp3&=tmp2;
    tmp.nt();
    tmp2.nt();
    tmp&=tmp2;
    tmp|=tmp3;
    Return(tmp);
End
Func(Math_Multiply,x,y)
    tmp=0;
    tmp2=x;
    tmp3=y;
    tmp4=0;
    Loop
        If(tmp4,==,16)
            Break
        End
        tmp5=y;
        tmp5&=mem[tmp4];
        If(tmp5,!=,0)
            tmp+=tmp2;
        End
        tmp2+=tmp2;
        tmp4++;
    End
    Return(tmp);
End
Func(Math_Divide,x,y)
    Var z;
    z=0;
    If(x,<,0)
        z+=1;
        x*=-1;
    End
    If(y,<,0)
        z-=1;
        y*=-1;
    End
    If(y,>,x)
        Return(0);
    End
    Var w,res;
    w=y;
    w+=y;
    res=Math::Divide(x,w);
    w=Math::Multiply(w,res);
    w+=y;
    res+=res;
    If(x,>=,w)
        res++;
    End
    If(z,!=,0)
        res*=-1;
    End
    Return(res);
End
Func(Math_Mod,x,y)
    Var res;
    res=Math::Divide(x,y);
    res=Math::Multiply(res,y);
    res*=-1;
    res+=x;
    Return(res);
End
Func(Math_Max,x,y)
    If(x,>,y)
        Return(x);
    End
    Return(y);
End
Func(Math_Min,x,y)
    If(x,<,y)
        Return(x);
    End
    Return(y);
End
}
