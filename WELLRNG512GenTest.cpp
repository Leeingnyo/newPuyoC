#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>

#include "WELLRNG512Gen.h"

// WELLRNG512GeneratorTest
int main(){
    printf("test start\n");
    { // same seed
        WELLRNG512Generator a(1);
        WELLRNG512Generator b(1);
        assert(a.Generate() == b.Generate());
        assert(a.Generate() == b.Generate());
        assert(a.Generate() == b.Generate());
    }
    printf("same seed test end\n");
    { // different seed
        WELLRNG512Generator c(1);
        WELLRNG512Generator d(2);
        assert(c.Generate() != d.Generate());
        assert(c.Generate() != d.Generate());
        assert(c.Generate() != d.Generate());
    }
    printf("different seed test end\n");
    { // same seed
        WELLRNG512Generator a(1);
        WELLRNG512Generator b(1);
        assert(a.Generate() % 10 == b.Generate() % 10);
        assert(a.Generate() % 10 == b.Generate() % 10);
        assert(a.Generate() % 10 == b.Generate() % 10);
    }
    printf("same seed modular test end\n");
    printf("all test end\n");
}

