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


FUNCTION :
First, we put the Key given in parameters to the most significant bits by shifting (64 - 24 = 40)
Then, we will use the 80 given bits to obtain our sub key. These 80 bits will changed every loop.
second_copy value is now on temporary one, as we need it to be on a 64 bit wide variable and not 16.

1st part of key generation is a circular shift, BUT our 80 bits are split in 64 + 16 so the operation is less simple.
    We have to shift left by 61, so our second_copy contains the [k35 ... k19] bits of our total word, which are on the 64 bits one.
    So we shift right by 3 our first part ([k35 ... k19] -> [k32 ... k16]), then we can assign this result to our second_copy.

    This biggest part needs to be assigned too, and contains a part of its previous content, as well as the second_copy (before change, so we'll use tmp).
    To do so, we OR | these value :
    1) we take our least significant bits (of first_copy), shift them left by 61, which gives us the 3 most significant bits.
    2) we use our 16 bits from second_copy, contained in a 64 wide one, and shift them left by 61 - 16 (as the second_copy needs to be withdrawn from the shift value)
    3) 3 + 16 = 19 is the shift to the right we need to make to put the values that were more significant than second_copy in our 80 bits.
       These were shifted from most significant to least, and then moved again on the first_copy, so instead of doing this, we directly shift in the opposite direction at the right position.

2nd part of the key generation is putting our 4 most significant bits in the Sbox, and then replacing the 4 bits by the new ones.
    To do so, we shift our first_value by 60 to the left, thus putting 0 on every bits but not the 4 least significant (before shifting the 4 most significant ones).
    This result is put in the Sbox, and the associated one is assigned to temporary.
    Then, first_copy gets a OR | between the new 4 bits that are shifted back to the most significant ones ; and its own value with the 4 most significant bits set to 0.

Last part is using a XOR between the 4 least significant bits or (first_copy and the most significant bit of second_copy) and the value i, which depend on the loop.
    These value are not in the same word, and we better XOR respectively each the 4bits of i with first_copy and the least significant with second_copy.
    
    The trick here is that XOR 0 with A is always A, so first_copy XOR i once i is shifted right 1 time (to remove the least significant bit), 
    you have something like 1111010101    ^     0000000111 
                        example_first_copy       example i >> 1 = 7

    The result at the left of the bits we're interested in won't change, so there's only a XOR to do.

    The idea is the same for second_copy. We will shift left i 15 times to put the least significant bit in front of second_copy's most significant one and then XOR. 
    second_copy other bits won't be modified by the XOR, as other every other bits than the one we look for on i are at 0. 
*/ 
void subKeyGeneration(uint32_t master_key, uint32_t* sub_keys) {

    uint16_t static sbox[16] = {12, 5, 6, 11, 9, 0, 10, 13, 3, 14, 15, 8, 4, 7, 1, 2};
    uint64_t first_copy = master_key, tmp; 
    uint16_t second_copy = 0;

    first_copy = (first_copy << 40);

    for (uint32_t i = 1; i <= 11 ; i++) {
        sub_keys[i-1] = first_copy & 0xffffff;
        tmp = second_copy;

        second_copy = first_copy << 45 >> 48;   // corresponds at ">> 3" in a more optimized way as observed during compilation.
        first_copy = (first_copy << 61) | (tmp << 45) | (first_copy >> 19);
        
        tmp = sbox[first_copy >> 60];                    
        first_copy = (tmp << 60) | (first_copy << 4) >> 4;

        first_copy = (first_copy ^ (i >> 1));
        second_copy = (second_copy ^ (i << 15));
    }
}