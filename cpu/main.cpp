#include <stdio.h>
#include <stdlib.h>

#include <iostream>

// http://www.obelisk.me.uk/6502

using Byte = unsigned char;
using Word = unsigned short;

using u32 = unsigned int;

struct Mem
{
   static constexpr u32 MAX_MEM = 1024 * 64;
   Byte Data[MAX_MEM];

   void Initialize()
   {
      for (u32 i = 0; i < MAX_MEM; i++) {
         Data[i] = 0;
      }
   }

   // read 1 byte
   Byte operator[](u32 Address) const {
      // assert Address < MAX_MEM
      return Data[Address];
   }

   // write 1 byte
   Byte& operator[](u32 Address) {
      // assert Address < MAX_MEM
      return Data[Address];
   }

};

struct CPU
{
   Word PC;  // program counter
   Word SP;  // stack pointer
   Byte A;   // accumulator
   Byte X;   // index register X
   Byte Y;   // index register Y

   // status flags
   Byte C : 1;  // carry
   Byte Z : 1;  // zero
   Byte I : 1;  // interrupt disable
   Byte D : 1;  // decimal mode
   Byte B : 1;  // break command
   Byte V : 1;  // overflow
   Byte N : 1;  // negative

   void Reset(Mem& memory)
   {
      PC = 0xFFFC;
      SP = 0x0100;
      C = Z = I = D = B = V = N = 0;
      A = X = Y = 0;
      memory.Initialize();
   }

   Byte FetchByte (u32& Cycles, Mem& memory) {
      Byte Data = memory[PC];
      PC++;
      Cycles--;
      return Data;
   }

   static constexpr Byte INS_LDA_IM = 0xA9;

   void Execute(u32 Cycles, Mem& memory)
   {
      while (Cycles > 0) {
        Byte Ins = FetchByte(Cycles, memory);
        switch (Ins) {
           case INS_LDA_IM: {
              Byte Value = FetchByte(Cycles, memory);
              A = Value;
              Z = (A == 0);
              N = (A &0b10000000) > 0;
           } break;
           default: {
              printf("Instruction not handled %d\n", Ins);
           }

        }
      }
   }

};

int main()
{
   Mem mem;
   CPU cpu;
   cpu.Reset(mem);

   mem[0xFFFC] = CPU::INS_LDA_IM;
   mem[0xFFFD] = 0x42;

   cpu.Execute(2, mem);

   std::cout << "PC:" << cpu.PC << std::endl;
   std::cout << "SP:" << cpu.SP << std::endl;
   std::cout << "A:" << cpu.A << std::endl;
   std::cout << "X:" << cpu.X << std::endl;
   std::cout << "Y:" << cpu.Y << std::endl;

   std::cout << "done\n";
   return 0;
}

