#include <sstream>
#include <iomanip>

char hex_characters[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

static std::string byte_to_hex_helper(char byte){
	if(byte == 0){
		return std::string();	
	}

	return byte_to_hex_helper(byte / 16) + std::string(1,hex_characters[byte % 16]);	
}

std::string byte_to_hex(char byte){
	if(byte == 0){
		return std::string("false (0)");
	}	
	if(byte == 1){
		return std::string("true (1)");
	}
	return byte_to_hex_helper(byte);
}

