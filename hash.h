#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>
//for r[i] values
#include <ctime>
#include <cstdlib>

typedef std::size_t HASH_INDEX_T;

struct MyStringHash {
    HASH_INDEX_T rValues[5] { 983132572, 1468777056, 552714139, 984953261, 261934300 };
    MyStringHash(bool debug = true)
    {
        if(false == debug){
            generateRValues();
        }
    }
    // hash function entry point (i.e. this is h(k))
    HASH_INDEX_T operator()(const std::string& k) const
    {
        // Add your code here
        //initialize w, with size 5 and all value defaul to be 0
        unsigned long long w[5] = {0};
        
        int strIdx = 0;
        int sizek = k.size();
        int sizea = 0;
        int sizew = 4;
        
        HASH_INDEX_T a[6] = {0};
        while (strIdx < sizek)
        {
            if(sizea == 6)
            {
                w[sizew] = ((((a[5] * 36 + a[4]) * 36 + a[3]) * 36 + a[2]) * 36 + a[1]) * 36 + a[0];
                sizew--;
                sizea=0;
                for(int i=0; i<6;i++)
                {
                    a[i] = 0;
                }
            }
            a[sizea] = letterDigitToNumber(k[sizek - strIdx - 1]);
            sizea++;
            strIdx++;
        }
        
        w[sizew] = ((((a[5] * 36 + a[4]) * 36 + a[3]) * 36 + a[2]) * 36 + a[1]) * 36 + a[0];
        
        
        unsigned long long ans=0;
        
        for (int i=0; i<5; i++)
        {
            ans += (rValues[i] * w[i]);
        }
        
        return ans;

    }

    // A likely helper function is to convert a-z,0-9 to an integral value 0-35
    HASH_INDEX_T letterDigitToNumber(char letter) const
    {
        HASH_INDEX_T ans;
        HASH_INDEX_T let = tolower(letter);
        if(let>=97 && let<=122)
        {
            ans = let-97;
        }
        else
        {
            ans = let-22;
        }
        return ans;
    }

    // Code to generate the random R values
    void generateRValues()
    {
        // obtain a seed from the system clock:
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator (seed);  // mt19937 is a standard random number generator

        // Simply call generator() [it has an operator()] to get another random number
        for(int i{ 0 }; i < 5; ++i)
        {
            rValues[i] = generator();
        }
    }
};

#endif
