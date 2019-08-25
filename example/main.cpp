#include"compiler.h"
#include"builtin_class.h"
Begin
    #include"builtin_lib.h"
    Output::Init();
    Math::Init();
    Array::Init();
    Var result;
    result=Array::Alloc(20);
    String::Set(result,"Hello, World!\n");
    Output::PrintString(result);
    Func(Fib,x)
        If(x,<=,1)
            Return(1);
        End
        Var sum;
        x--;
        sum=Call(Fib,x);
        x--;
        sum+=Call(Fib,x);
        Return(sum);
    End
    Func(Print_Num,x)
        Var p,q,p1,q1;
        p=Array::Alloc(30);
        q=Array::Alloc(30);
        p1=p;
        q1=q;
        If(x,==,0)
            mem[p1]='0';
            p1++;
            mem[p1]=0;
        End
        If(x,!=,0)
            Var i;
            Loop
                If(x,==,0)
                    Break
                End
                i=x;
                i%=10;
                i+='0';
                mem[q1]=i;
                q1++;
                x/=10;
            End
            Loop
                If(q1,==,q)
                    Break;
                End
                q1--;
                mem[p1]=mem[q1];
                p1++;
            End
            mem[p1]=0;
        End
        Output::PrintString(p);
        Array::Free(p);
        Array::Free(q);
    End
    Var i;
    i=1;
    Loop
        Var tmp;
        tmp=Call(Fib,i);
        Call(Print_Num,tmp);
        If(i,==,20)
            Break
        End
        Output::PrintChar('\n');
        i++;
    End
Finish
