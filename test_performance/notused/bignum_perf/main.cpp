#include <iostream>
#include <cmath>
#include <map>
#include <vector>
#include <string>

#include "chron.h"
#include "bigun.h"

#include "../ckksrns/study/lifter.h"

using std::string;
using std::cout;
using std::vector;


using u128 = lifter<uint64_t>;
using u256 = lifter<u128>;
using u512 = lifter<u256>;
using u1024 = lifter<u512>;
using u2048 = lifter<u1024>;
using u4096 = lifter<u2048>;

int main()
try
{
    if (0)
    {
        cout << "powers of to up to 4096\n";
        u4096 ux {2};
        Integer bx {2};
        for ( int i = 0; i < 12; i++ )
        {
            auto ua = ux * ux;
            auto ub = ua - u4096 {3};
            auto uc = ua % ub;
            cout << i << "\t" << ua << '\t' << uc << "\n";
            ux = ua;

            auto ba = bx * bx;
            auto bb = ba - Integer {3};
            auto bc = ba % bb;
            cout << i << "\t" << ba << '\t' << bc << "\n";
            bx = ba;
        }
    }


    //int order = 10; // 10 - 2000 bits;
    //int shift0 = 263;
    //int shift1 = 999;
    //using utyp = u4096;
    //int delay = 1;

    //int order = 9;
    //int shift0 = 132;
    //int shift1 = 499;
    //using utyp = u2048;
    //int delay = 1;

    //int order = 8;
    //int shift0 = 66;
    //int shift1 = 249;
    //using utyp = u1024;
    //int delay = 4;

    //int order = 7;
    //int shift0 = 33;
    //int shift1 = 124;
    //using utyp = u512;
    //int delay = 16;

    int order = 6;
    int shift0 = 15;
    int shift1 = 31;
    using utyp = u256;
    int delay = 64;

    if (1)
    {
        cout << "\nspeed test Lifter\n";
        // select 2000 bit number
        utyp ux {2};
        auto start = chron::now();
        for ( int i = 0; i < order; i++ ) ux = ux * ux + utyp {17};
        ux >>= shift0;
        {
            int cntr = 0;
            {
                auto x = ux;
                while ( !!x ) { x >>= 1; cntr++ ; }
            }
            //cout << "x and size: " << ux << " 0x" << str(ux, 16) << " [" << cntr << "]\n";
            cout << "size: " << " [" << cntr << "]\n";
        }

        //cout << "time = " << (chron::now() - start) << "ms\n";

        cout << "\nstarting speed test, Lifter\n";
        start = chron::now();
        const int IT = 500 * delay;
        for ( int i = 0; i < IT; i++)
        {
            utyp ui {uint64_t(i)};
            auto y = (ux >> shift1) + ui;
            y = ux * ux % y;
        }
        cout << "lifter MOD time = " << (chron::now() - start) * 1.0 / IT << "ms\n";
    }

    if (1)
    {
        cout << "\nspeed test GMP\n";
        // select 2000 bit number
        Integer ux {2};
        auto start = chron::now();
        for ( int i = 0; i < order; i++ ) ux = ux * ux + Integer {17};
        ux >>= shift0;
        {
            int cntr = 0;
            {
                auto x = ux;
                while ( !!x ) { x >>= 1; cntr++ ; }
            }
            cout << "x and size: " << ux << " [" << cntr << "]\n";
            cout << "size: " << " [" << cntr << "]\n";
        }

        //cout << "time = " << (chron::now() - start) << "ms\n";

        cout << "\nstarting speed test, GMPbig\n";
        start = chron::now();
        const int IT = 10000 * delay;
        for ( int i = 0; i < IT; i++)
        {
            Integer ui {uint64_t(i)};
            auto y = ux;
            y >>= shift1;
            y = y + ui;
            y = ux * (ux + i) % y;
        }
        cout << "GMPbig MOD time = " << (chron::now() - start) * 1.0 / IT << "ms\n";
    }

}

catch (int e)
{
    cout << "error (int): " << e << "\n";
    return 1;
}
catch (string e)
{
    cout << "error (str): " << e << "\n";
    return 1;
}
catch (const char * e)
{
    cout << "error (cc): " << e << "\n";
    return 1;
}
catch (std::exception & e)
{
    cout << "error (std): " << e.what() << "\n";
    return 1;
}
catch (...)
{
    cout << "Unknown exception\n";
    return 1;
}



