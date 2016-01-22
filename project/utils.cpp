#include "utils.h"
#include "CException.h"


void HashCrackerUtils::ParseCommandLine(const int p_argc, const char *p_argv[], std::string &p_hash, std::string &p_algo, std::string &p_alphabet, unsigned int &p_chunkSize, std::string &p_masterIpAddress, bool &p_slaveMode) {
	int i = 1;			// Because argv[0] contains the full path to program name --> real parameters start at position 1

	// Default values
	p_slaveMode = false;
	p_hash = "";
	p_algo = "";
	p_alphabet = "";
	p_chunkSize = 0;

	// Extract parameters using C style
	while( i < p_argc ) {
		if ( i + 1 < p_argc ) {										// Because all parameters are in the form (-${key}, ${value}), ensure the ${value} can be extracted
			if ( _strcmpi("-hash", p_argv[i] ) == 0 ) {
				p_hash = p_argv[i + 1];
				i++;												// skip associated value for next iteration
			}
			else if ( _strcmpi("-algo", p_argv[i] ) == 0 ) {
				p_algo = p_argv[i + 1];
				i++;												// skip associated value for next iteration
			}
			else if ( _strcmpi("-alphabet", p_argv[i] ) == 0 ) {
				p_alphabet = p_argv[i + 1];
				i++;												// skip associated value for next iteration
			}
			else if ( _strcmpi("-chunksize", p_argv[i] ) == 0 ) {
				p_chunkSize = std::atoi(p_argv[i + 1]);
				if( p_chunkSize < 0 || p_chunkSize > 10)
					p_chunkSize = 0;
				i++;												// skip associated value for next iteration
			}
			else if ( _strcmpi("-ip", p_argv[i] ) == 0 ) {
				p_masterIpAddress = p_argv[i + 1];
				i++;												// skip associated value for next iteration
			}
		}
		i++;
	}

	// Check for all mandatory parameters
	if ( p_masterIpAddress.length() > 0 ) {
		// Assume slave mode
		p_slaveMode = true;
		if (p_hash.length() > 0 || p_algo.length() > 0 || p_alphabet.length() > 0 || p_chunkSize > 0)
			throw _CException ( "Invaid command line.", 0 );
	}
	else {
		// Assume master mode
		p_slaveMode = false;
		if (p_hash.length() == 0 || p_algo.length() == 0 || p_alphabet.length() == 0 || p_chunkSize == 0)
			throw _CException ( "Invaid command line.", 0 );
	}
}


/// @brief compute next character according to aplhabet
/// @return next character or '\0' if the end of alphabet has been reached
static char NextAlphabetChar(const std::string &p_alphabet, const char p_currentChar) {
	char nextChar = '\0';

	size_t pos = p_alphabet.find(p_currentChar);
	if ( pos != std::string::npos  && pos < p_alphabet.length() - 1)
		nextChar = p_alphabet[pos + 1];

	return nextChar;
}


// For better speed directly handle memory instead of std::string
void HashCrackerUtils::IncreasePassword(char *p_password, const unsigned int p_bufferCapacity, const std::string &p_alphabet) {
	bool finished = false;
	char nc = '?';
	unsigned int i;


	i = static_cast<int>(strlen( p_password ));
	if( i < p_bufferCapacity ) {
		while ( !finished ) {
			nc = (i == 0 ? '\0' : NextAlphabetChar( p_alphabet, p_password[i - 1] ));
			if( nc != '\0' ) {
				// Not yet reached the end of alphabet, nc is the next character
				p_password[i - 1] = nc;
				finished = true;
			}
			else {
				// Reached the end of the alphabet
				if( i == 0 ) {
					// Shift 1 character to the right
					memmove_s(p_password + 1, p_bufferCapacity - 1, p_password, p_bufferCapacity - 1);
					p_password[0] = p_alphabet[0];
					finished = true;
				}
				else {
					p_password[i - 1] = p_alphabet[0];
				}
			}
			i--;
		}
	}
	else {
		throw _CException( "Buffer too small to store next password. 2 characters are required after the last password character.", 0 );
	}
}
