# Arm Architecture Process Call Standard

> Function: A, B

> Call: ->

A -> B: 这个过程就是一个过程调用，A作为caller，B作为callee

过程调用设计的寄存器保存策略，具体请参考[AAPCS]:(https://github.com/ABigBright/abi-aa.git)

# Example

```c
int fa(int a, int b)
{
    fb(0xee);
    return 0x11;
}

int fc(int a, int b, int c, int d, int e, int f);

int fb(int a)
{
    fc(0xff, 0x55, 0xaa, 0x44, 0x11, 0x22);
    return 0x22;
}

int fc(int a, int b, int c, int d, int e, int f)
{
    fa(0x55, 0x66);
    return 0x33;
}
```

+ fa -> fb
+ fb -> fc
+ fc -> fa

> 不考虑递归问题，单纯看过程调用对寄存器的处理

如下是反汇编相关的代码，

```c
00008244 <fa>:
fa():
/home/briq/work/tmp/aarch32_aapcs/main.c:6
#include <stdio.h>

int fb(int a);

int fa(int a, int b)
{
    8244:	e92d4800 	push	{fp, lr}
    8248:	e28db004 	add	fp, sp, #4
    824c:	e24dd008 	sub	sp, sp, #8
    8250:	e50b0008 	str	r0, [fp, #-8]
    8254:	e50b100c 	str	r1, [fp, #-12]
/home/briq/work/tmp/aarch32_aapcs/main.c:7
    fb(0xee);
    8258:	e3a000ee 	mov	r0, #238	; 0xee
    825c:	eb000004 	bl	8274 <fb>
/home/briq/work/tmp/aarch32_aapcs/main.c:8
    return 0x11;
    8260:	e3a03011 	mov	r3, #17
/home/briq/work/tmp/aarch32_aapcs/main.c:9
}
    8264:	e1a00003 	mov	r0, r3
    8268:	e24bd004 	sub	sp, fp, #4
    826c:	e8bd4800 	pop	{fp, lr}
    8270:	e12fff1e 	bx	lr

00008274 <fb>:
fb():
/home/briq/work/tmp/aarch32_aapcs/main.c:14

int fc(int a, int b, int c, int d, int e, int f);

int fb(int a)
{
    8274:	e92d4800 	push	{fp, lr}
    8278:	e28db004 	add	fp, sp, #4
    827c:	e24dd010 	sub	sp, sp, #16
    8280:	e50b0008 	str	r0, [fp, #-8]
/home/briq/work/tmp/aarch32_aapcs/main.c:15
    fc(0xff, 0x55, 0xaa, 0x44, 0x11, 0x22);
    8284:	e3a03022 	mov	r3, #34	; 0x22
    8288:	e58d3004 	str	r3, [sp, #4]
    828c:	e3a03011 	mov	r3, #17
    8290:	e58d3000 	str	r3, [sp]
    8294:	e3a03044 	mov	r3, #68	; 0x44
    8298:	e3a020aa 	mov	r2, #170	; 0xaa
    829c:	e3a01055 	mov	r1, #85	; 0x55
    82a0:	e3a000ff 	mov	r0, #255	; 0xff
    82a4:	eb000004 	bl	82bc <fc>
/home/briq/work/tmp/aarch32_aapcs/main.c:16
    return 0x22;
    82a8:	e3a03022 	mov	r3, #34	; 0x22
/home/briq/work/tmp/aarch32_aapcs/main.c:17
}
    82ac:	e1a00003 	mov	r0, r3
    82b0:	e24bd004 	sub	sp, fp, #4
    82b4:	e8bd4800 	pop	{fp, lr}
    82b8:	e12fff1e 	bx	lr

000082bc <fc>:
fc():
/home/briq/work/tmp/aarch32_aapcs/main.c:20

int fc(int a, int b, int c, int d, int e, int f)
{
    82bc:	e92d4800 	push	{fp, lr}
    82c0:	e28db004 	add	fp, sp, #4
    82c4:	e24dd010 	sub	sp, sp, #16
    82c8:	e50b0008 	str	r0, [fp, #-8]
    82cc:	e50b100c 	str	r1, [fp, #-12]
    82d0:	e50b2010 	str	r2, [fp, #-16]
    82d4:	e50b3014 	str	r3, [fp, #-20]	; 0xffffffec
/home/briq/work/tmp/aarch32_aapcs/main.c:21
    fa(0x55, 0x66);
    82d8:	e3a01066 	mov	r1, #102	; 0x66
    82dc:	e3a00055 	mov	r0, #85	; 0x55
    82e0:	ebffffd7 	bl	8244 <fa>
/home/briq/work/tmp/aarch32_aapcs/main.c:22
    return 0x33;
    82e4:	e3a03033 	mov	r3, #51	; 0x33
/home/briq/work/tmp/aarch32_aapcs/main.c:23
}
    82e8:	e1a00003 	mov	r0, r3
    82ec:	e24bd004 	sub	sp, fp, #4
    82f0:	e8bd4800 	pop	{fp, lr}
    82f4:	e12fff1e 	bx	lr

```

+ fa有2个参数(r0, r1承载)，所以在fa call fb，需要caller（fa）保存r0, r1
+ fb有1个参数(r0承载), 所以在fb call fc，需要caller(fb)保存r0
+ fc有6个参数(r0\~r3, 剩下的入栈)， 所以在fc call fa, 需要caller(fc)保存r0\~r3

# Sumary

根据AAPCS的标准描述，aarch32的参数使用r0\~r3,来作为过程调用传递参数，也就是说，作为caller，需要维护r0\~r3, 其余的寄存器，需要callee去维护（即callee-saved register）
