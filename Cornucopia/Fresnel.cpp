/*--
Fresnel.cpp  

This file is part of the Cornucopia curve sketching library.
Copyright (C) 2010 Ilya Baran (ibaran@mit.edu)

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

/*
The code in this file is adapted from:
Cephes Math Library Release 2.8:  June, 2000
Copyright 1984, 1987, 1989, 2000 by Stephen L. Moshier
*/

#include "Fresnel.h"
#include <vector>

using namespace std;
using namespace Eigen;
NAMESPACE_Cornu

//Polynomial evaluation routines
static double polevl( double x, double *coef, int N ) //regular
{
    int i = N;
    double ans = *coef++;

    do
    {
        ans = ans * x  +  *coef++;
    } while( --i );

    return ans;
}

static double p1evl( double x, double *coef, int N ) //leading coef is 1
{
    int i = N - 1;
    double ans = x + *coef++;

    do
    {
        ans = ans * x  + *coef++;
    } while( --i );

    return ans;
}

template<typename Packet>
static Packet polevl( const Packet &x, typename ei_unpacket_traits<Packet>::type *coef, int N )
{
    int i = N;
    Packet ans = ei_pset1(*coef++);

    do
    {
        ans = ei_padd(ei_pmul(ans, x), ei_pset1(*coef++));
    } while(--i);

    return ans;
}

template<typename Packet>
static Packet p1evl( const Packet &x, typename ei_unpacket_traits<Packet>::type *coef, int N )
{
    int i = N - 1;
    Packet ans = ei_padd(x, ei_pset1(*coef++));

    do
    {
        ans = ei_padd(ei_pmul(ans, x), ei_pset1(*coef++));
    } while(--i);

    return ans;
}

/* S(x) for small x */
static double sn[6] = {
    -2.99181919401019853726E3,
    7.08840045257738576863E5,
    -6.29741486205862506537E7,
    2.54890880573376359104E9,
    -4.42979518059697779103E10,
    3.18016297876567817986E11,
};
static double sd[6] = {
    /* 1.00000000000000000000E0,*/
    2.81376268889994315696E2,
    4.55847810806532581675E4,
    5.17343888770096400730E6,
    4.19320245898111231129E8,
    2.24411795645340920940E10,
    6.07366389490084639049E11,
};

/* C(x) for small x */
static double cn[6] = {
    -4.98843114573573548651E-8,
    9.50428062829859605134E-6,
    -6.45191435683965050962E-4,
    1.88843319396703850064E-2,
    -2.05525900955013891793E-1,
    9.99999999999999998822E-1,
};
static double cd[7] = {
    3.99982968972495980367E-12,
    9.15439215774657478799E-10,
    1.25001862479598821474E-7,
    1.22262789024179030997E-5,
    8.68029542941784300606E-4,
    4.12142090722199792936E-2,
    1.00000000000000000118E0,
};

/* Auxiliary function f(x) */
static double fn[10] = {
    4.21543555043677546506E-1,
    1.43407919780758885261E-1,
    1.15220955073585758835E-2,
    3.45017939782574027900E-4,
    4.63613749287867322088E-6,
    3.05568983790257605827E-8,
    1.02304514164907233465E-10,
    1.72010743268161828879E-13,
    1.34283276233062758925E-16,
    3.76329711269987889006E-20,
};
static double fd[10] = {
    /*  1.00000000000000000000E0,*/
    7.51586398353378947175E-1,
    1.16888925859191382142E-1,
    6.44051526508858611005E-3,
    1.55934409164153020873E-4,
    1.84627567348930545870E-6,
    1.12699224763999035261E-8,
    3.60140029589371370404E-11,
    5.88754533621578410010E-14,
    4.52001434074129701496E-17,
    1.25443237090011264384E-20,
};

/* Auxiliary function g(x) */
static double gn[11] = {
    5.04442073643383265887E-1,
    1.97102833525523411709E-1,
    1.87648584092575249293E-2,
    6.84079380915393090172E-4,
    1.15138826111884280931E-5,
    9.82852443688422223854E-8,
    4.45344415861750144738E-10,
    1.08268041139020870318E-12,
    1.37555460633261799868E-15,
    8.36354435630677421531E-19,
    1.86958710162783235106E-22,
};
static double gd[11] = {
    /*  1.00000000000000000000E0,*/
    1.47495759925128324529E0,
    3.37748989120019970451E-1,
    2.53603741420338795122E-2,
    8.14679107184306179049E-4,
    1.27545075667729118702E-5,
    1.04314589657571990585E-7,
    4.60680728146520428211E-10,
    1.10273215066240270757E-12,
    1.38796531259578871258E-15,
    8.39158816283118707363E-19,
    1.86958710162783236342E-22,
};

void fresnel( double xxa, double *ssa, double *cca )
{
    double f, g, cc, ss, c, s, t, u;
    double x, x2;

    x = fabs(xxa);
    x2 = x * x;
    if( x2 < 2.5625 )
    {
        t = x2 * x2;
        ss = x * x2 * polevl( t, sn, 5)/p1evl( t, sd, 6 );
        cc = x * polevl( t, cn, 5)/polevl(t, cd, 6 );
        goto done;
    }

    if( x > 36974.0 )
    {
        cc = 0.5;
        ss = 0.5;
        goto done;
    }

    /*		Asymptotic power series auxiliary functions
    *		for large argument
    */
    x2 = x * x;
    t = PI * x2;
    u = 1.0/(t * t);
    t = 1.0/t;
    f = 1.0 - u * polevl( u, fn, 9)/p1evl(u, fd, 10);
    g = t * polevl( u, gn, 10)/p1evl(u, gd, 11);

    t = HALFPI * x2;
    c = cos(t);
    s = sin(t);
    t = PI * x;
    cc = 0.5  +  (f * s  -  g * c)/t;
    ss = 0.5  -  (f * c  +  g * s)/t;

done:
    if( xxa < 0.0 )
    {
        cc = -cc;
        ss = -ss;
    }

    *cca = cc;
    *ssa = ss;
}

template<typename Packet>
void fresnelLow( const Packet &xxa, Packet *ssa, Packet *cca )
{
    Packet cc, ss, t;
    Packet x, x2;

    x = ei_pabs(xxa);
    x2 = ei_pmul(x, x);

    t = ei_pmul(x2, x2);
    ss = ei_pmul(x, ei_pmul(x2, ei_pdiv(polevl( t, sn, 5), p1evl( t, sd, 6 ))));
    cc = ei_pmul(x, ei_pdiv(polevl( t, cn, 5), polevl(t, cd, 6 )));

    /*
    if( xxa[0] < 0.0 )
    {
        cc = -cc;
        ss = -ss;
    }
    */

    *cca = cc;
    *ssa = ss;
}

#if 0
template<typename Packet>
void fresnl( const Packet &xxa, Packet *ssa, Packet *cca )
{
    Packet f, g, cc, ss, c, s, t, u;
    Packet x, x2;

    x = ei_pabs(xxa);
    x2 = x * x;
    if( x2[0] < 2.5625 )
    {
        t = x2 * x2;
        ss = x * x2 * polevl( t, sn, 5)/p1evl( t, sd, 6 );
        cc = x * polevl( t, cn, 5)/polevl(t, cd, 6 );
        goto done;
    }

    if( x[0] > 36974.0 )
    {
        cc = Array2d(0.5, 0.5);
        ss = Array2d(0.5, 0.5);
        goto done;
    }
    /*		Asymptotic power series auxiliary functions
    *		for large argument
    */
    x2 = x * x;
    t = PI * x2;
    t = Array2d(1.0, 1.0)/t;
    u = t * t;
    f = Array2d(1.0, 1.0) - u * polevl( u, fn, 9)/p1evl(u, fd, 10);
    g = t * polevl( u, gn, 10)/p1evl(u, gd, 11);

    t = HALFPI * x2;
    //c = cos(t);
    //s = sin(t);
    c[0] = cos(t[0]); c[1] = cos(t[1]);
    s[0] = sin(t[0]); s[1] = sin(t[1]);
    t = PI * x;
    cc = Array2d(0.5, 0.5)  +  (f * s  -  g * c)/t;
    ss = Array2d(0.5, 0.5)  -  (f * c  +  g * s)/t;

done:
    if( xxa[0] < 0.0 )
    {
        cc = -cc;
        ss = -ss;
    }

    *cca = cc;
    *ssa = ss;
}
#endif

void runTest()
{
    const int num = 10000000;

    double sum = 0;
    double inum = 1. / double(num);

    Debugging::get()->startTiming("Fresnel scalar");
    for(int i = 0; i < num; ++i) {
        double t = 1.5 * double(i) * inum;
        double s, c;
        fresnel(t, &s, &c);
        sum += s;
        sum += c;
    }
    Debugging::get()->elapsedTime("Fresnel scalar");

    Debugging::get()->printf("Done, sum = %lf\n", sum);
    sum = 0;

    Debugging::get()->startTiming("Fresnel 2d");
    for(int i = 0; i < num; i += 2) {
        Array2d t, s, c;
        t[0] = 1.5 * double(i) / double(num);
        t[1] = 1.5 * double(i + 1) / double(num);

        Packet2d pt, ps, pc;
        pt = t.packet<Aligned>(0);

        fresnelLow(pt, &ps, &pc);

        s.writePacket<Aligned>(0, ps);
        c.writePacket<Aligned>(0, pc);

        sum += (s + c).sum();
    }
    Debugging::get()->elapsedTime("Fresnel 2d");

    Debugging::get()->printf("Done, sum = %lf\n", sum);
    sum = 0;

#if 0
    for(int i = 0; i < num; i += 4) {
        Vector4f t, s, c;
        t[0] = 4. * float(i) / float(num);
        t[1] = 4. * float(i + 1) / float(num);
        t[2] = 4. * float(i + 2) / float(num);
        t[3] = 4. * float(i + 3) / float(num);

        Packet4f pt, ps, pc;

        pt = ei_pload(&(t[0]));
        ps = ei_psin(pt);
        pc = ei_pcos(pt);
        ei_pstore(&(s[0]), ps);
        ei_pstore(&(c[0]), pc);

        sum += (s + c).sum();
    }
#endif
}

END_NAMESPACE_Cornu


