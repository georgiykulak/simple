#include <stdio.h>  // print
#include <stdlib.h> // malloc, calloc, realloc, free
#include <stdint.h> // uint16_t



// ignore first 8 zeroes
//  00000101                     11110101
//         5                          245               4               7
unsigned int rangeToggle(unsigned int num, unsigned int i, unsigned int j)
{
    //                          4 = 7 - 4 + 1
    unsigned int bitRangeFromZero = j - i + 1;

    //                      00010000
    unsigned int numberRangeFromZero = 1 << bitRangeFromZero;

    //         00010000
    //       - 00000001
    //         ________
    //         00001111
    numberRangeFromZero -= 1;

    //                   11110000              00001111    4
    unsigned int numberRangeFromI = numberRangeFromZero << i;

    //   11110101
    // ^ 11110000
    //   ________
    //   00000101
    return num ^ numberRangeFromI;
}



#define SWAP_16_BIT(number)   (((number & 0x00FF) << 8) \
                            |  ((number & 0xFF00) >> 8))



//                                10010000
//  128                                144
uint8_t remove_last_set_bit(uint8_t number)
{
    //           10010000
    //         - 00000001
    //           ________
    //           10001111
    uint8_t numberOneLess = number - 1;

    //   10010000
    // & 10001111
    //   ________
    //   10000000
    return number & numberOneLess;
}



//                  00010000
void powerOf2(uint8_t number)
{
    // 1) 00010000 --> 00001111
    // 2) 00010000 & 00001111 = 00000000 or just 0

    if (number & (number - 1))
    {
        printf("Number is NOT power of 2\n");
    }
    else
    {
        printf("Number is power of 2\n");
    }
}


unsigned int countSetBits(unsigned int number)
{
    unsigned int count = 0;

    // number == ...00011011
    // 1) 00011011
    //  & 00011010
    //    ________
    // 2) 00011010
    //  & 00011001
    //    ________
    // 3) 00011000
    //  & 00010111
    //    ________
    // 4) 00010000
    //  & 00001111
    //    ________
    //    00000000
    while (number != 0)
    {
        ++count;
        number &= (number - 1);
    }

    return count;
}

unsigned int myCountSetBits(unsigned int number)
{
    unsigned int count = 0;

    while (number)
    {
        count += (number & 1);
        number >>= 1;
    }

    return count;
}



int main()
{
    unsigned rangeRes = rangeToggle(245, 4, 7);
    printf("range res = %d\n", rangeRes); // = 5

    //                                        {  ->  }{  <-  }
    uint16_t swappedEightBits = SWAP_16_BIT(0b0000111100000000);
    //              '0000.0000.0000.1111' or '15'
    printf("swapped res = %d\n", swappedEightBits); // = 15

    unsigned lastBitReset = remove_last_set_bit(144);
    printf("bit less res = %d\n", lastBitReset); // = 128

    // 01000000
    powerOf2(64); // is power of 2
    // 00110000
    powerOf2(48); // is NOT power of 2

    //              4       ...00011011
    unsigned countRes = countSetBits(27);
    unsigned myCountRes = myCountSetBits(27);
    printf("count res = %d\n", countRes);      // = 4
    printf("my count res = %d\n", myCountRes); // = 4
}