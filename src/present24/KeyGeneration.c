#include "KeyGeneration.h"

/*
For this function, we will use the following representation :
| k79 ....................... k16 | k15 ................... k0 |
|----------------------------------|----------------------------|
        64bits wide first_copy         16bits wide second_copy

For the following code, first_copy is the biggest part of our 80bits, being 64 bits large.
                        second_copy is the smallest one, being 16 bits.
Total : 80 bits
A 64 bit temporary variable is needed, to store 16 bits or 64 depending on the operations below.

Optimization : As we use the same Sbox in the sub keys generation, declaring a static one is optimized, like we did in the Substitution function.
*/
void subKeyGeneration(uint32_t master_key, uint32_t *sub_keys)
{
    uint16_t static sbox[16] = {12, 5, 6, 11, 9, 0, 10, 13, 3, 14, 15, 8, 4, 7, 1, 2};
    uint64_t first_copy = master_key, tmp;
    uint16_t second_copy = 0;
    
    //we put the Key given in parameters to the most significant bits by shifting (64 - 24 = 40)
    first_copy = (first_copy << 40);
    for (uint32_t i = 1; i <= 11; i++)
    {
        //We add a new sub-key using the 24 least significant bits of our 64 most significant bits.
        sub_keys[i - 1] = first_copy & 0xffffff;
        //Temporary contains the 16 least significant bits of the 80bits.
        tmp = second_copy;

        //We have to shift left by 61, in order to have our second_copy contains the [k35 ... k19] bits of our total word, which are on the 64 bits one.
        //So we shift right by 3 our first part ([k35 ... k19] -> [k32 ... k16]), then we can assign this result to our second_copy.        
        second_copy = first_copy << 45 >> 48; // corresponds at ">> 3" in a more optimized way as observed during compilation, because it directly puts every other bits to 0.
        
        /*
        This biggest part needs to be assigned too, and contains a part of its previous content, as well as the second_copy (before change, so we'll use tmp).
        To do so, we OR | these value :
        1) we take our least significant bits (of first_copy), shift them left by 61, which gives us the 3 most significant bits.
        2) we use our 16 bits from second_copy, contained in a 64 wide one, and shift them left by 61 - 16 (as the second_copy needs to be withdrawn from the shift value)
        3) 3 + 16 = 19 is the shift to the right we need to make to put the values that were more significant than second_copy in our 80 bits.
            These were shifted from most significant to least, and then moved again on the first_copy, so instead of doing this, we directly shift in the opposite direction at the right position.
        */
        first_copy = (first_copy << 61) | (tmp << 45) | (first_copy >> 19);

        //We shift the 4 most significant bits and shift them to the least significant, then put this value (a 4 bits number) in the Sbox. The associated value is put in temporary (still a 4 bits number).
        tmp = sbox[first_copy >> 60];
        //We shift our temporary by 60 to the left, keeping the 4 least significant bits and putting them in the most significant ones.
        //We use OR with our first_copy on which we took away the 4 most signficant bits ; and our temporary. This result in 4 modified bits at most significant position and the rest remains unchanged.
        first_copy = (tmp << 60) | (first_copy << 4) >> 4;

        //using a XOR between the 4 least significant bits or (first_copy and the most significant bit of second_copy) and the value i, which depend on the loop.
        //These value are not in the same word, and we better XOR respectively each the 4bits of i with first_copy and the least significant with second_copy.
        //Note : the operation is simple because the rest of the bits from i are at 0, and using xor with a 0 does not change the other bit.
        first_copy = (first_copy ^ (i >> 1));
        second_copy = (second_copy ^ (i << 15));
    }
}