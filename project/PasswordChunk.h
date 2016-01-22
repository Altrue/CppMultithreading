#pragma once

#include <iostream>
#include <string>

class CPasswordChunk {
private:
	std::string m_begin;
	std::string m_end;

public:
	CPasswordChunk();
	CPasswordChunk( std::string p_passwordBegin, std::string p_passwordEnd );
	~CPasswordChunk();

	void Reset();
	void SetPasswordRange( std::string p_passwordBegin, std::string p_passwordEnd );

	std::string GetPasswordBegin() const;
	std::string GetPasswordEnd() const;
	int GetChunkSize() const;
};
