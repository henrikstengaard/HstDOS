void MicroDelay (long MicroSecs)
{
  asm {
    mov cx, word ptr [MicroSecs+2]
    mov dx, word ptr [MicroSecs]
    mov ah, 0x86
    int 0x15
  }
}

void MilliDelay (long MilliSecs)
{
  MicroDelay (MilliSecs * 1000);
}