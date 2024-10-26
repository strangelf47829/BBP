#include "../include/ELSA/Numerical.h":

// Static operator stuff
BBP::elsa::Numerical_t BBP::elsa::Numerical_t::operator+(BBP::elsa::Numerical_t)
{
	// Not implemented
	throw 0;
	return 0;	
}

BBP::elsa::Numerical_t BBP::elsa::Numerical_t::operator-(BBP::elsa::Numerical_t)
{
	throw 0;
}

// Static operator stuff
BBP::elsa::Numerical_t BBP::elsa::Numerical_t::operator*(BBP::elsa::Numerical_t)
{
	throw 0;
}

BBP::elsa::Numerical_t BBP::elsa::Numerical_t::operator/(BBP::elsa::Numerical_t)
{
	throw 0;
}

BBP::elsa::Numerical_t BBP::elsa::Numerical_t::operator%(BBP::elsa::Numerical_t)
{
	throw 0;
}


// Equality comparators
bool BBP::elsa::Numerical_t::operator==(Numerical_t)
{
	throw 0;
}

bool BBP::elsa::Numerical_t::operator!=(Numerical_t)
{
	throw 0;
}


// Greater than, less than, comparators
bool BBP::elsa::Numerical_t::operator<(Numerical_t){throw 0;}
bool BBP::elsa::Numerical_t::operator>(Numerical_t){throw 0;}

// Compound comparators
bool BBP::elsa::Numerical_t::operator<=(Numerical_t){throw 0;}
bool BBP::elsa::Numerical_t::operator>=(Numerical_t) {throw 0;}

// Compound operators
void BBP::elsa::Numerical_t::operator+=(Numerical_t){throw 0;}
void BBP::elsa::Numerical_t::operator-=(Numerical_t){throw 0;}
void BBP::elsa::Numerical_t::operator/=(Numerical_t){throw 0;}
void BBP::elsa::Numerical_t::operator*=(Numerical_t){throw 0;}
void BBP::elsa::Numerical_t::operator%=(Numerical_t){throw 0;}

// Compound unary operators
BBP::elsa::Numerical_t BBP::elsa::Numerical_t::operator++(){throw 0;}
BBP::elsa::Numerical_t BBP::elsa::Numerical_t::operator--(){throw 0;}

// Negate 
BBP::elsa::Numerical_t BBP::elsa::Numerical_t::operator-() {throw 0;}

// Not
bool BBP::elsa::Numerical_t::operator!() {throw 0;}

// Bitwise operations
BBP::std::word BBP::elsa::Numerical_t::operator|(Numerical_t){throw 0;}
BBP::std::word BBP::elsa::Numerical_t::operator&(Numerical_t){throw 0;}
BBP::std::word BBP::elsa::Numerical_t::operator^(Numerical_t){throw 0;}
BBP::std::word BBP::elsa::Numerical_t::operator~() {throw 0;}
