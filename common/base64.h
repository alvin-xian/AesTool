//
//  base64 encoding and decoding with C++.
//  Version: 1.01.00
//

#ifndef BASE64_H_C0CE2A47_D10E_42C9_A27C_C883944E704A
#define BASE64_H_C0CE2A47_D10E_42C9_A27C_C883944E704A

#define BASE64_ENCODE_OUT_SIZE(s) (((unsigned int)(s)%3>0)?((unsigned int)(s/3)*4+4):(((unsigned int)(s/3)*4)))
#define BASE64_DECODE_OUT_SIZE(s) ((unsigned int)((s/4)*3))

#include <string>

std::string base64_encode(unsigned char const* , unsigned int len);
std::string base64_decode(std::string const& s, unsigned int *out_length);

std::string base64_url_encode(unsigned char const* , unsigned int len);
std::string base64_url_decode(std::string const& s, unsigned int *out_length);

#endif /* BASE64_H_C0CE2A47_D10E_42C9_A27C_C883944E704A */
