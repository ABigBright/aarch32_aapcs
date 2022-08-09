#include <stdio.h>

int fb(int a);

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

void _exit(int e)
{}

int main(void)
{
    fa(0x11, 0x22);
    fc(0xbb, 0xaa, 0x99, 0x88, 0x77, 0x66);
}
