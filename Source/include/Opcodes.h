#ifndef BBP_STDLIB_OPCODES_H
#define BBP_STDLIB_OPCODES_H

#include "stddef.h"

namespace BBP
{
	namespace userspace
	{

		enum OPCODES : std::byte
		{
			NIL = 0,
			STACK = 1,
			CTRL = 2,
			MEM = 3,
			INC = 4,
			DEC = 5,
			NEG = 6,
			ADD = 7,
			SUB = 8,
			MUL = 9,
			DIV = 10,
			MOD = 11,
			LBS = 12,
			RBS = 13,
			OR = 14,
			XOR = 15,
			AND = 16,
			NOT = 17,
			FLAG = 18,
			JMP = 19,
			JMPZ = 20,
			JMPC = 21,
			PAGE = 22,
			SIGNAL = 23,
			HALT = 24,
			SECR = 25
		};

		enum STACKSUFFIXES : std::byte
		{
			push = 1,
			pop = 2,
			argument = 4
		};

		enum CTRLSUFFIXES : std::byte
		{
			ret = 1,
			call = 2,
			callp = 4,
			syscall = 8,
			routine = 16,
			absolute = 32
		};

		enum MATHSUFFIXES : std::byte
		{
			ffloat = 1,
			sfloat = 2,
		};

		enum LOGICSUFFIXES : std::byte
		{
			bitwise = 1,
			inplace = 2
		};

		enum JMPSUFFIXES : std::byte
		{
			jmproutine = 1,
			negated = 2,
			lessthan = 4,
			greaterthan = 8,
			equalto = 16,
			absoluteJmp = 32
		};

		enum PAGESUFFIXES : std::byte
		{
			assign = 1,
			read = 2,
			write = 4,
			memoryOp = 8,
			sizeOp = 16,
			bytes = 32
		};

		enum MEMSUFFIXES : std::byte
		{
			fbyte = 1,
			fhalfword = 2,
			fword = 4,
			faddress = 8,
			sbyte = 16,
			shalfword = 32,
			sword = 64,
			saddress = 128
		};

		enum SECRSUFFIXES : std::byte
		{
			endbr = 1
		};

		constexpr std::word REGISTERPAGE = 0xf0000;

		enum REGISTERMODIFIERS : std::byte
		{
			HIHW = 16,
			LOHW = 32,
			HIBY = 64,
			LOBY = 128
		};

	}
}

/*
nil		(NIL)
push	(STACK)
pop		(STACK)
popa	(STACK)
ret		(CTRL)
call	(CTRL)
callp	(CTRL)
syscall	(CTRL)
mov		(MEM)
inc		(INC)
dec		(DEC)
neg		(NEG)
add		(ADD)
sub		(SUB)
mul		(MUL)
div		(DIV)
mod		(MOD)
incf	(INC)
decf	(DEC)
negf	(NEG)
addf	(ADD)
subf	(SUB)
mulf	(MUL)
divf	(DIV)
modf	(MOD)
addff	(ADD)
subff	(SUB)
mulff	(MUL)
divff	(DIV)
modff	(MOD)
addif	(ADD)
subif	(SUB)
mulif	(MUL)
divif	(DIV)
modif	(MOD)
lbs		(LBS)
rbs		(RBS)
or		(OR)
xor		(XOR)
and		(AND)
not		(NOT)
orb		(OR)
andb	(AND)
notb	(NOT)
lbsip	(LBS)
rbsip	(RBS)
orip	(OR)
xorip	(XOR)
andip	(AND)
notip	(NOT)
orbip	(OR)
andbip	(AND)
notbip	(NOT)
comp	(FLG)
flag	(FLG)
routine	(CTRL)
jmp		(JMP)
jlz		(JMPZ)
jgz		(JMPZ)
jez		(JMPZ)
jlt		(JMPC)
jgt		(JMPC)
jlzn	(JMPZ)
jgzn	(JMPZ)
jezn	(JMPZ)
jltn	(JMPC)
jgtn	(JMPC)
jmpr	(JMP)
jlzr	(JMPZ)
jgzr	(JMPZ)
jezr	(JMPZ)
jltr	(JMPC)
jgtr	(JMPC)
jlznr	(JMPZ)
jgznr	(JMPZ)
jeznr	(JMPZ)
jltnr	(JMPC)
jgtnr	(JMPC)
page	(PAGE)
pageaw	(PAGE)
pagear	(PAGE)
pagew	(PAGE)
pager	(PAGE)
palloc	(PAGE)
pfree	(PAGE)
pages	(PAGE)
pagebc	(PAGE)
passign (PAGE)
signal	(SIGNAL)
signalf	(SIGNAL)
hlt		(HALT)
hcf		(HALT)
hbu		(HALT)

*/

#endif