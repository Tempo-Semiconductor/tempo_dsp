
#include <iostream>
#include <cstdlib>
#include <cstdio>

#include "biquad.hpp"

using namespace tempo;

int main(int argc, char *argv[])
{
    bqd_t bq = biquad_filter(1000.0, 0.0, 48000.0,
                                BW, 1.8, 
                                PEAKING);

    printf("Test passed:\n%f\n%f\n%f\n%f\n%f\n%f\n",
            bq.a0, bq.a1, bq.a2, bq.b0, bq.b1, bq.b2);

    std::exit(EXIT_SUCCESS);
}
