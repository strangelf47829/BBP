#include "../include/Except.h"
#include "../include/Strings.h"


BBP::std::size_t BBP::std::strlen(BBP::std::conststring str)
{
	// Check if string is valid
	if (str == nullptr)
		throw BBP::std::exception("Could not calculate string length: string is null.", EINVAL);

	// Create length variable
	size_t length = 0;

	for (; str[length]; length++)
		if (length >= BBP::std::max_string_length)
			throw BBP::std::exception("Failed to calculate string length: string is calculated to be bigger than maximum permissible size.", ENOBUFS);

	return length;
}

BBP::std::size_t BBP::std::strlen(BBP::std::c_string str)
{
	// Check if string is valid
	if (str == nullptr)
		throw BBP::std::exception("Could not calculate string length: string is null.", EINVAL);

	// Create length variable
	size_t length = 0;

	for (; str[length]; length++)
		if (length >= BBP::std::max_string_length)
			throw BBP::std::exception("Failed to calculate string length: string is calculated to be bigger than maximum permissible size.", ENOBUFS);

	return length;
}

BBP::std::size_t BBP::std::strlen(BBP::std::string &str)
{
	// Create length variable
	size_t length = 0;

	for (; BBP::std::read(&str, length); length++)
		if (length >= BBP::std::max_string_length)
			throw BBP::std::exception("Failed to calculate string length: string is calculated to be bigger than maximum permissible size.", ENOBUFS);

	return length;
}

BBP::std::size_t BBP::std::strlen(BBP::std::string str, BBP::std::offset_t offset)
{
	// Create length variable
	size_t length = 0;

	for (; BBP::std::read(&str, length + offset); length++)
		if (length >= BBP::std::max_string_length)
			throw BBP::std::exception("Failed to calculate string length: string is calculated to be bigger than maximum permissible size.", ENOBUFS);

	return length;
}


BBP::std::size_t __UNSAFE__(BBP::std::strlen)(BBP::std::conststring str)
{
	// Create length variable
	size_t length = 0;

	for (; str[length]; length++);

	return length;
}

BBP::std::size_t __UNSAFE__(BBP::std::strlen)(BBP::std::c_string str)
{
	// Create length variable
	size_t length = 0;

	for (; str[length]; length++);

	return length;
}

BBP::std::size_t __UNSAFE__(BBP::std::strlen)(BBP::std::string str)
{
	// Create length variable
	size_t length = 0;

	for (; __UNSAFE__(BBP::std::read)(&str, length); length++);

	return length;
}

BBP::std::size_t __UNSAFE__(BBP::std::strlen)(BBP::std::string str, BBP::std::offset_t offset)
{
	// Create length variable
	size_t length = 0;

	for (; __UNSAFE__(BBP::std::read)(&str, length + offset); length++);

	return length;
}


/* Copy string literal into 'dst' */
void BBP::std::strcpy(string *dst, conststring src)
{
	BBP::std::c_string cstring = (BBP::std::c_string)src;
	BBP::std::strcpy(dst, cstring);
}

/* Copy conventional string into 'dst' */
void BBP::std::strcpy(string *dst, c_string src)
{
	size_t len = BBP::std::strlen(src);
	BBP::std::write_a(dst, src, 0, len + 1);
}

/* Copy string literal into 'dst' */
void BBP::std::strcpy(c_string dst, conststring src)
{
	size_t len = BBP::std::strlen(src) + 1;
	for (std::index_t idx = 0; idx < len; idx++)
		dst[idx] = src[idx];
}

bool BBP::std::strcmp(string *str1, conststring str2)
{
	// If lenghts are unequal, just return false
	size_t len1 = BBP::std::strlen(str1->data);
	size_t len2 = BBP::std::strlen(str2);

	// If lengths are unequal, just return
	if (len1 != len2)
		return false;

	// Loop over each character
	for (size_t index = 0; index < len1; index++)
	{
		// Fetch each character
		char c1 = std::read(str1, index);
		char c2 = str2[index];

		// If characters are unequal, return false
		if (c1 != c2)
			return false;
	}
	return true;
}

bool BBP::std::strcmp(c_string a, c_string b)
{
	// If both are equal to eachother, return true
	if (a == b)
		return true;

	// If either are nullptrs, return false
	if (a == nullptr || b == nullptr)
		return false;

	// Compare both
	for (BBP::std::index_t idx = 0; a[idx] || b[idx]; idx++)
	{
		// If string at this position is not the same, return false
		if (a[idx] != b[idx])
			return false;
	}

	// Return true
	return true;
}

bool BBP::std::strcmp(string &a, string &b)
{
	// Get sequence length for both
	std::size_t seqlen_a = seqlen(a);
	std::size_t seqlen_b = seqlen(b);

	// If they are both 0, return true
	if (seqlen_a == seqlen_b && seqlen_a == 0)
		return true;

	// If these don't match, return false
	if (seqlen_a != seqlen_b)
		return false;

	// Compare both
	for (BBP::std::index_t idx = 0; a[idx] || b[idx]; idx++)
	{
		// If string at this position is not the same, return false
		if (a[idx] != b[idx])
			return false;
	}

	// Return true
	return true;
}

void BBP::std::operator<=(string &dst, conststring src)
{
	BBP::std::strcpy(&dst, src);
}

void BBP::std::operator<=(string &dst, c_string src)
{
	BBP::std::strcpy(&dst, src);
}

BBP::std::Stack<BBP::std::string_element> &BBP::std::operator<<=(std::Stack<std::string_element> &stack, std::conststring str)
{
	stack << str;
	stack << '\0';

	// If writeTo is defined: execute
	if (stack.writeTo)
		stack.writeTo(&stack);

	return stack;
}

BBP::std::Stack<BBP::std::string_element> &BBP::std::operator<<=(std::Stack<std::string_element> &stack, std::c_string str)
{
	stack << str;
	stack << '\0';

	// If writeTo is defined: execute
	if (stack.writeTo)
		stack.writeTo(&stack);

	return stack;
}

BBP::std::Stack<BBP::std::string_element> &BBP::std::operator<<=(std::Stack<std::string_element> &stack, std::string &str)
{
	// Fetch the underlying c string
	std::c_string cstr = std::read_a(&str, 0);

	// Write that cstring
	stack <<= cstr;

	return stack;
}

BBP::std::Stack<BBP::std::string_element> &BBP::std::operator<<=(std::Stack<std::string_element> &stack, char c)
{
	stack << c;
	stack << (char)0;

	// If writeTo is defined: execute
	if (stack.writeTo)
		stack.writeTo(&stack);

	return stack;
}

BBP::std::Stack<BBP::std::string_element> &BBP::std::operator<=(std::Stack<std::string_element> &stack, int value)
{
	// Just write 'value' with the << operator
	stack << value;

	// Write '0'
	stack <<= "";

	return stack;
}

BBP::std::Stack<BBP::std::string_element> &BBP::std::operator<<(std::Stack<std::string_element> &stack, std::conststring str)
{
	// Check string exists
	if (str == nullptr)
		throw std::exception("Failed to stream string into buffer: string is null.", EINVAL);

	// Calculate string size
	size_t strLen = std::strlen(str);

	// Check if stack has enough space
	if (stack.atElement + strLen >= stack.max_elements)
		throw std::exception("Failed to stream string into buffer: operation would cause buffer overflow.", EPERM);

	// Write unsafely (since we already checked everything)
	for (index_t idx = 0; idx < strLen; idx++)
		stack <= str[idx];

	return stack;
}

BBP::std::Stack<BBP::std::string_element> &BBP::std::operator<<(std::Stack<std::string_element> &stack, std::c_string str)
{
	// Check string exists
	if (str == nullptr)
		throw std::exception("Failed to stream string into buffer: string is null.", EINVAL);

	// Calculate string size
	size_t strLen = std::strlen(str);

	// Check if stack has enough space
	if (stack.atElement + strLen >= stack.max_elements)
		throw std::exception("Failed to stream string into buffer: operation would cause buffer overflow.", EPERM);

	// Write unsafely (since we already checked everything)
	for (index_t idx = 0; idx < strLen; idx++)
		stack <= str[idx];

	return stack;
}

BBP::std::Stack<BBP::std::string_element> &BBP::std::operator<<(std::Stack<std::string_element> &stack, std::string &str)
{
	// Fetch the underlying c string
	std::c_string cstr = std::read_a(&str, 0);

	// Write that cstring
	stack <<= cstr;

	return stack;
}

BBP::std::Stack<BBP::std::string_element> &BBP::std::operator<(std::Stack<std::string_element> &stack, int value)
{
	// Print sign
	if (value < 0)
	{
		stack << '-';
		value *= -1;
	}

	// Get the amount of decimals
	uint32_t decimals = std::decimals(value);

	for (uint32_t decimal = 0; decimal < decimals; decimal++)
	{
		std::word dec = std::getDecimal(value, decimals - decimal - 1);
		stack << (char)(dec + 48);
	}

	return stack;
}

BBP::std::Stack<BBP::std::string_element> &BBP::std::operator>>=(std::Stack<std::string_element> &stack, std::c_string *str)
{
	// Check target exists
	if (str == nullptr)
		throw std::exception("Failed to stream string from buffer: string is null.", EINVAL);

	// If stack is already at 0, throw error (since this only occurs if there is 'nothing' in the buffer)
	if (stack.atElement == 0)
		throw std::exception("Could not stream string from buffer: buffer is empty.", EPERM);

	// Move the pointer back by one
	stack--;

	// Check if the current element is not a null character
	if ((stack %= 0) == 0)
		stack--; // If this is the case, move the pointer back one.

	// If this caused buffer overflow: return error.
	if (stack.atElement > stack.max_elements)
	{
		stack++; // Rectify the overflow
		throw std::exception("Failed to stream string from buffer: Caused overflow. (rectified).", EPERM);
	}

	char c = 0; // Allocate char already
	do
	{
		// Peek once character (unsafe since we know where we are)
		c = stack %= 0;

		// Check if character is null
		if (c == 0)
		{
			// If null, return here
			*str = std::read_a(stack.page, stack.atElement + 1);

			// If readFrom is defined: execute
			if (stack.readFrom)
				stack.readFrom(&stack);

			return stack;
		}

		if (stack.atElement == 0)
			break; // Must be done, logic allows it and WILL cause errors if not handled, even for unsafe functions.

		// Character not null, go back
		stack--;

	} while (stack.atElement);

	// Reached element 0, so return element 0.
	*str = __UNSAFE__(std::read_a)(stack.page, stack.atElement);

	// If readFrom is defined: execute
	if (stack.readFrom)
		stack.readFrom(&stack);

	return stack;
}

BBP::std::Stack<BBP::std::string_element> &BBP::std::operator>>=(std::Stack<std::string_element> &stack, std::string *str)
{
	// Fetch the underlying c string
	std::c_string cstr;
	stack >>= &cstr;

	// Get the size of the string
	size_t length = std::strlen(cstr);

	// Write that cstring into string
	std::write_a(str, cstr, 0, length);

	// If readFrom is defined: execute
	if (stack.readFrom)
		stack.readFrom(&stack);

	return stack;
}

BBP::std::Stack<BBP::std::string_element> &BBP::std::operator>(std::Stack<std::string_element> &stack, std::c_string *str)
{
	// Move pointer back by one.
	stack--;

	// Check if the current element is not a null character
	if (stack %= 0)
		stack--; // If this is the case, move the pointer back one.

	char c = 0; // Allocate char already
	do
	{
		// Peek once character (unsafe since we know where we are)
		c = stack %= 0;

		// Check if character is null
		if (c == 0)
		{
			// If null, return here
			*str = __UNSAFE__(std::read_a)(stack.page, stack.atElement + 1);

			// If readFrom is defined: execute
			if (stack.readFrom)
				stack.readFrom(&stack);

			return stack;
		}

		if (stack.atElement == 0)
			break; // Must be done, logic allows it and WILL cause errors if not handled, even for unsafe functions.

		// Character not null, go back
		stack--;

	} while (stack.atElement);

	// Reached element 0, so return element 0.
	*str = __UNSAFE__(std::read_a)(stack.page, stack.atElement);

	// If readFrom is defined: execute
	if (stack.readFrom)
		stack.readFrom(&stack);

	return stack;
}

BBP::std::Stack<BBP::std::string_element> &BBP::std::operator>(std::Stack<std::string_element> &stack, std::string *str)
{
	// Fetch the underlying c string
	std::c_string cstr;
	stack > &cstr;

	// Get the size of the string
	size_t length = __UNSAFE__(std::strlen)(cstr);

	// Write that cstring into string
	__UNSAFE__(std::write_a)(str, cstr, 0, length);

	// If readFrom is defined: execute
	if (stack.readFrom)
		stack.readFrom(&stack);

	return stack;
}

BBP::std::Stack<BBP::std::string_element> &BBP::std::operator>>(std::Stack<std::string_element> &lh, std::Stack<std::string_element> &rh)
{
	char c = 0;
	lh >> c;
	rh << c;
	return lh;
}

BBP::std::Stack<BBP::std::string_element> &BBP::std::operator<<(std::Stack<std::string_element> &lh, std::Stack<std::string_element> &rh)
{
	char c = 0;
	rh >> c;
	lh << c;
	return lh;
}

BBP::std::Stack<BBP::std::string_element> &BBP::std::operator>>=(std::Stack<std::string_element> &lh, std::Stack<std::string_element> &rh)
{
	char c = 0;
	lh >> c;
	rh <<= c;
	return lh;
}

BBP::std::Stack<BBP::std::string_element> &BBP::std::operator<<=(std::Stack<std::string_element> &lh, std::Stack<std::string_element> &rh)
{
	char c = 0;
	rh >> c;
	lh <<= c;
	return lh;
}

BBP::std::hash_t BBP::std::strhsh(std::string &str)
{
	return std::strhsh(std::read_a(&str, 0));
}

BBP::std::hash_t BBP::std::strhsh(const std::string &str)
{
	// Create new string for some reason?
	std::string str2(str.dataSize, str.data);

	return std::strhsh(str2);
}

BBP::std::hash_t BBP::std::strhsh(std::conststring str)
{
	return std::strhsh((std::c_string) str);
}

BBP::std::hash_t BBP::std::strhsh(std::c_string str)
{
	// Hashing constants
	const int p = 31;
	const int m = 1e9 + 9;

	// Working variables
	std::hash_t hash = 0;
	uint32_t Power = 1;

	// Get the size of the string
	std::size_t length = std::strlen(str);

	// Actual hashing algorithm. Taken from: 
	for (std::index_t index = 0; index < length; index++)
	{
		char c = str[index];
		hash = (hash + (c - 'a' + 1) * Power) % m;
		Power = (p * Power) % m;
	}
	
	return hash;
}

void BBP::std::strhsh(std::hash_t &hash, std::word &Power, std::string_element &c)
{
	// Hashing constants
	const int p = 31;
	const int m = 1e9 + 9;
	
	// Run the hashing equation once
	hash = (hash + (c - 'a' + 1) * Power) % m;
	Power = (p * Power) % m;
}

template<>
BBP::std::string::PAGE<BBP::std::string_element>(const char *arr)
	: PAGE<BBP::std::string_element>(BBP::std::strlen(arr), (std::c_string)arr)
{
	
}

template<>
BBP::std::string::operator BBP::std::hash_t() const
{
	return std::strhsh(*this);
}

template<>
bool BBP::std::PAGE<char>::operator ==(string &b)
{
	return std::strcmp(*this, b);
}

template<>
bool BBP::std::PAGE<char>::operator !=(string &b)
{
	return !(*this == b);
}