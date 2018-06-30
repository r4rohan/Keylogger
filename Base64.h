//we are using here base 64 encryption to encode and decode strings, it transfer bits to human readable string
#ifndef BASE64_H
#define BASE64_H

#include <vector> //data str that is fairly similar to array but flexible and dynamic i.e. it can be modify, add or pushed etc
#include <string>

namespace Base64
{
    std::string base64_encode(const std::string &);  //constant preference to string
    //encryption key //salt //helps in decryption
    const std::string &SALT1="LM::TB::BB";
    const std::string &SALT2="_:/_77";
    const std::string &SALT3="line=wowC++";

    std::string EncryptB64(std::string s)
    {
        s=SALT1+s+SALT2+SALT3;
        s=base64_encode(s);
        s.insert(7, SALT3);
        s+=SALT1;
        s=base64_encode(s);
        s=SALT2+SALT3+SALT1;
        s=base64_encode(s);
        s.insert(1, "L");
        s.insert(7, "M");
        return s;
    }
    const std::string &BASE64_CODE="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"; //64 characters base

    std::string base64_encode(const std::string &s)
    {
        std::string ret; //we will fill ret with encoded characters
        int val=0; //index we use to map i/p to our table
        int bits=-6; //bits will be used to represent no of bits in a subgroup
        const unsigned int b63=0x3F; //hexadecimal of 63
        //for each loop (new in c++11)
        for(const auto &c:s) //auto is used when is don't know the data type so auto will automatically recognized the data type
        {
            val=(val<<8)+c; // << is left binary shift and resulting no will shift left by 8 places
            bits+=8;
            while(bits>=0)
            {
                ret.push_back(BASE64_CODES[(val>>bits) &b63]); //it inserts the mapped no, anding is done here
                bits-=6; //decrease the bits by 6 reason is grouping is by 6
            }
        }
        if(bits>-6)
            ret.push_back(BASE64_CODES[( (val<<8) >> (bits+8) ) &b63]); //if ret is not aligned then we have paddings so that u can insert equal at the end and decoder will get your encryption
        while(ret.size()%4)
            ret.push_back('='); //padding character
        //while loop evaluates (ret.size()%4) either true (greater than 0) or false
        return ret; //encoded string
    }

}

#endif // BASE64_H
