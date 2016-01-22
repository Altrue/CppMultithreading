#include "PasswordChunk.h"


CPasswordChunk::CPasswordChunk() {
	this->Reset();
}


CPasswordChunk::CPasswordChunk( std::string p_passwordBegin, std::string p_passwordEnd ) {
	this->SetPasswordRange( p_passwordBegin, p_passwordEnd );
}


CPasswordChunk::~CPasswordChunk() {
	this->Reset();
}


void CPasswordChunk::Reset() {
	this->m_begin = "";
	this->m_end = "";
}


void CPasswordChunk::SetPasswordRange( std::string p_passwordBegin, std::string p_passwordEnd ) {
	this->m_begin = p_passwordBegin;
	this->m_end = p_passwordEnd;
}


std::string CPasswordChunk::GetPasswordBegin() const {
	return this->m_begin;
}


std::string CPasswordChunk::GetPasswordEnd() const {
	return this->m_end;
}


int CPasswordChunk::GetChunkSize() const {
	int i, result = 0;
	char c1 = '\0', c2 = '\0';

	// Try to get common prefix part
	i = 0;
	do {
		c1 = '\0';
		c2 = '\0';

		// Smartly get char value avoiding overflow
		if (i < static_cast<int>(this->m_begin.length())) {
			c1 = this->m_begin[ i ];
		}
		if (i < static_cast<int>(this->m_end.length())) {
			c2 = this->m_end[ i ];
		}

		// Check for char diff
		if (c1 != c2 && !(c1 == '\0' && c2 == '\0'))
			result++;

		i++;
	} while (!(c1 == '\0' && c2 == '\0'));
	return result;
}
