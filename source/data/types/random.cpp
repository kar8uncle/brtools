#include <random>
#include <data/types/random.h>

using namespace brtools::data::types;
using namespace std;

namespace
{
    using random_engine = ranlux24;
    random_device rd;
    seed_seq seed{ rd(), rd(), rd() };
    random_engine rng(seed);

    template<typename NumberType>
    NumberType gen(NumberType lower, NumberType upper)
    {
        // TODO: assert lower <= upper
        return lower + rng() % (upper - lower);
    }
}

template<>
int16_t s_rand::generate(int16_t lower, int16_t upper)
{   return gen(lower, upper); }

template<>
uint16_t u_rand::generate(uint16_t lower, uint16_t upper)
{   return gen(lower, upper); }
