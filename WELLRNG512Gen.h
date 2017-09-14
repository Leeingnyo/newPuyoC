
class WELLRNG512Generator {
    unsigned long state[16];
    int index = 0;
    public:
        WELLRNG512Generator(){
            srand(time(NULL));
            for (int i = 0; i < 16; i++)
                state[i] = rand();
        }
        WELLRNG512Generator(const int &seed){
            srand(seed);
            for (int i = 0; i < 16; i++)
                state[i] = rand();
        }
        unsigned long Generate(){
            // http://www.gamedevforever.com/114
            unsigned long a, b, c, d;
            a = state[index];
            c = state[(index+13)&15];
            b = a^c^(a<<16)^(c<<15);
            c = state[(index+9)&15];
            c ^= (c>>11);
            a = state[index] = b^c;
            d = a^((a<<5)&0xDA442D20UL);
            index = (index + 15)&15;
            a = state[index];
            state[index] = a^b^d^(a<<2)^(b<<18)^(c<<28);
            return state[index];
        }
};

