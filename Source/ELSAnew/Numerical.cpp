#include "../include/ELSA/Numerical.h"

// Static operator stuff
BBP::elsa::Numerical_t BBP::elsa::Numerical_t::operator+(BBP::elsa::Numerical_t)
{

}

BBP::elsa::Numerical_t BBP::elsa::Numerical_t::operator-(BBP::elsa::Numerical_t)
{

}

// Static operator stuff
BBP::elsa::Numerical_t BBP::elsa::Numerical_t::operator*(BBP::elsa::Numerical_t)
{

}

BBP::elsa::Numerical_t BBP::elsa::Numerical_t::operator/(BBP::elsa::Numerical_t)
{

}

BBP::elsa::Numerical_t BBP::elsa::Numerical_t::operator%(BBP::elsa::Numerical_t)
{

}


// Equality comparators
bool BBP::elsa::Numerical_t::operator==(Numerical_t)
{

}

bool BBP::elsa::Numerical_t::operator!=(Numerical_t)
{

}


// Greater than, less than, comparators
bool BBP::elsa::Numerical_t::operator<(Numerical_t){}
bool BBP::elsa::Numerical_t::operator>(Numerical_t){}

// Compound comparators
bool BBP::elsa::Numerical_t::operator<=(Numerical_t){}
bool BBP::elsa::Numerical_t::operator>=(Numerical_t) {}

// Compound operators
void BBP::elsa::Numerical_t::operator+=(Numerical_t){}
void BBP::elsa::Numerical_t::operator-=(Numerical_t){}
void BBP::elsa::Numerical_t::operator/=(Numerical_t){}
void BBP::elsa::Numerical_t::operator*=(Numerical_t){}
void BBP::elsa::Numerical_t::operator%=(Numerical_t){}

// Compound unary operators
BBP::elsa::Numerical_t BBP::elsa::Numerical_t::operator++(){}
BBP::elsa::Numerical_t BBP::elsa::Numerical_t::operator--(){}

// Negate 
BBP::elsa::Numerical_t BBP::elsa::Numerical_t::operator-() {}

// Not
bool BBP::elsa::Numerical_t::operator!() {}

// Bitwise operations
BBP::std::word BBP::elsa::Numerical_t::operator|(Numerical_t){}
BBP::std::word BBP::elsa::Numerical_t::operator&(Numerical_t){}
BBP::std::word BBP::elsa::Numerical_t::operator^(Numerical_t){}
BBP::std::word BBP::elsa::Numerical_t::operator~() {}