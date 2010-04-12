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
#include <iostream>

using namespace std;
using namespace Eigen;
NAMESPACE_Cornu

//Polynomial evaluation routines
static double polevl( double x, const VectorXd &coefVec ) //regular
{
    int i = coefVec.size() - 1;
    const double *coef = &(coefVec[0]);
    double ans = *coef++;

    do
    {
        ans = ans * x  +  *coef++;
    } while( --i );

    return ans;
}

static double p1evl( double x, const VectorXd &coefVec ) //leading coef is 1
{
    int i = coefVec.size() - 1;
    const double *coef = &(coefVec[0]);

    double ans = x + *coef++;

    do
    {
        ans = ans * x  + *coef++;
    } while( --i );

    return ans;
}

template<typename Packet>
static Packet polevl( const Packet &x, const Matrix<typename ei_unpacket_traits<Packet>::type, Dynamic, 1> &coefVec )
{
    int i = coefVec.size() - 1;
    const typename ei_unpacket_traits<Packet>::type *coef = &(coefVec[0]);
    Packet ans = ei_pset1(*coef++);

    do
    {
        ans = ei_padd(ei_pmul(ans, x), ei_pset1(*coef++));
    } while(--i);

    return ans;
}

template<typename Packet>
static Packet p1evl( const Packet &x, const Matrix<typename ei_unpacket_traits<Packet>::type, Dynamic, 1> &coefVec )
{
    int i = coefVec.size() - 1;
    const typename ei_unpacket_traits<Packet>::type *coef = &(coefVec[0]);
    Packet ans = ei_padd(x, ei_pset1(*coef++));

    do
    {
        ans = ei_padd(ei_pmul(ans, x), ei_pset1(*coef++));
    } while(--i);

    return ans;
}

EIGEN_STRONG_INLINE Packet4f packetTransferSign(const Packet4f& to, const Packet4f& from)
{
  const Packet4f mask = _mm_and_ps(from, _mm_castsi128_ps(_mm_setr_epi32(0x80000000,0x80000000,0x80000000,0x80000000)));
  return _mm_or_ps(to, mask);
}
EIGEN_STRONG_INLINE Packet2d packetTransferSign(const Packet2d& to, const Packet2d from)
{
  const Packet2d mask = _mm_and_pd(from, _mm_castsi128_pd(_mm_setr_epi32(0x00000000,0x80000000,0x00000000,0x80000000)));
  return _mm_or_pd(to, mask);
}

EIGEN_STRONG_INLINE Packet4f packetFmod(const Packet4f &a, const Packet4f &b)
{
    Packet4f div = ei_pdiv(a, b);
    div = _mm_cvtepi32_ps(_mm_cvtps_epi32(div));
    return ei_psub(a, ei_pmul(div, b));
}

//==================Coefficients===========================

template<typename Real> 
struct FresnelCoefs
{
    static Matrix<Real, Dynamic, 1> sn, sd, cn, cd, fn, fd, gn, gd;
};

VectorXd FresnelCoefs<double>::sn(6), FresnelCoefs<double>::sd(6);
VectorXd FresnelCoefs<double>::cn(6), FresnelCoefs<double>::cd(7);
VectorXd FresnelCoefs<double>::fn(10), FresnelCoefs<double>::fd(10);
VectorXd FresnelCoefs<double>::gn(11), FresnelCoefs<double>::gd(11);
VectorXf FresnelCoefs<float>::sn(7), FresnelCoefs<float>::sd(0);
VectorXf FresnelCoefs<float>::cn(7), FresnelCoefs<float>::cd(0);
VectorXf FresnelCoefs<float>::fn(8), FresnelCoefs<float>::fd(0);
VectorXf FresnelCoefs<float>::gn(8), FresnelCoefs<float>::gd(0);

struct InitCoefs
{
    InitCoefs()
    {
        initFloat();
        initDouble();
    }

    void initFloat()
    {
        FresnelCoefs<float>::sn <<
            1.647629463788700E-009,
            -1.522754752581096E-007,
            8.424748808502400E-006,
            -3.120693124703272E-004,
            7.244727626597022E-003,
            -9.228055941124598E-002,
            5.235987735681432E-001;
        FresnelCoefs<float>::cn <<
            1.416802502367354E-008,
            -1.157231412229871E-006,
            5.387223446683264E-005,
            -1.604381798862293E-003,
            2.818489036795073E-002,
            -2.467398198317899E-001,
            9.999999760004487E-001;
        FresnelCoefs<float>::fn <<
            -1.903009855649792E+012,
            1.355942388050252E+011,
            -4.158143148511033E+009,
            7.343848463587323E+007,
            -8.732356681548485E+005,
            8.560515466275470E+003,
            -1.032877601091159E+002,
            2.999401847870011E+000;
        FresnelCoefs<float>::gn <<
            -1.860843997624650E+011,
            1.278350673393208E+010,
            -3.779387713202229E+008,
            6.492611570598858E+006,
            -7.787789623358162E+004,
            8.602931494734327E+002,
            -1.493439396592284E+001,
            9.999841934744914E-001;
    }

    void initDouble()
    {
        FresnelCoefs<double>::sn <<
            -2.99181919401019853726E3,
            7.08840045257738576863E5,
            -6.29741486205862506537E7,
            2.54890880573376359104E9,
            -4.42979518059697779103E10,
            3.18016297876567817986E11;
        FresnelCoefs<double>::sd <<
            /* 1.00000000000000000000E0,*/
            2.81376268889994315696E2,
            4.55847810806532581675E4,
            5.17343888770096400730E6,
            4.19320245898111231129E8,
            2.24411795645340920940E10,
            6.07366389490084639049E11;
        FresnelCoefs<double>::cn <<
            -4.98843114573573548651E-8,
            9.50428062829859605134E-6,
            -6.45191435683965050962E-4,
            1.88843319396703850064E-2,
            -2.05525900955013891793E-1,
            9.99999999999999998822E-1;
        FresnelCoefs<double>::cd <<
            3.99982968972495980367E-12,
            9.15439215774657478799E-10,
            1.25001862479598821474E-7,
            1.22262789024179030997E-5,
            8.68029542941784300606E-4,
            4.12142090722199792936E-2,
            1.00000000000000000118E0;
        FresnelCoefs<double>::fn <<
            4.21543555043677546506E-1,
            1.43407919780758885261E-1,
            1.15220955073585758835E-2,
            3.45017939782574027900E-4,
            4.63613749287867322088E-6,
            3.05568983790257605827E-8,
            1.02304514164907233465E-10,
            1.72010743268161828879E-13,
            1.34283276233062758925E-16,
            3.76329711269987889006E-20;
        FresnelCoefs<double>::fd <<
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
            1.25443237090011264384E-20;
        FresnelCoefs<double>::gn <<
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
            1.86958710162783235106E-22;
        FresnelCoefs<double>::gd <<
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
            1.86958710162783236342E-22;
    }
} init;

void fresnel( double xxa, double *ssa, double *cca )
{
    typedef FresnelCoefs<double> FC;

    double f, g, cc, ss, c, s, t, u;
    double x, x2;

    x = fabs(xxa);
    x2 = x * x;
    if( x2 < 2.5625 )
    {
        t = x2 * x2;
        ss = x * x2 * polevl( t, FC::sn)/p1evl( t, FC::sd);
        cc = x * polevl( t, FC::cn)/polevl(t, FC::cd);
        goto done;
    }

    if( x > 36974.0 )
    {
        cc = 0.5;
        ss = 0.5;
        goto done;
    }

    /*        Asymptotic power series auxiliary functions
    *        for large argument
    */
    x2 = x * x;
    t = PI * x2;
    u = 1.0/(t * t);
    t = 1.0/t;
    f = 1.0 - u * polevl( u, FC::fn)/p1evl(u, FC::fd);
    g = t * polevl( u, FC::gn)/p1evl(u, FC::gd);

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

void fresnelLow( const Packet4f &xxa, Packet4f *ssa, Packet4f *cca )
{
    typedef FresnelCoefs<float> FC;

    Packet4f cc, ss, t;
    Packet4f x, x2;

    x = ei_pabs(xxa);
    x2 = ei_pmul(x, x);

    t = ei_pmul(x2, x2);
    ss = ei_pmul(x, ei_pmul(x2, polevl( t, FC::sn)));
    cc = ei_pmul(x, polevl( t, FC::cn));

    *ssa = packetTransferSign(ss, xxa);
    *cca = packetTransferSign(cc, xxa);
}

void fresnelMed( const Packet4f &xxa, Packet4f *ssa, Packet4f *cca )
{
    typedef FresnelCoefs<float> FC;

    Packet4f cc, ss, t, u, f, g, c, s;
    Packet4f x, x2;

    x = ei_pabs(xxa);
    x2 = ei_pmul(x, x);

    t = ei_pmul(ei_pset1(float(PI)), x2);
    t = ei_pdiv(ei_pset1(float(1.0)), t);
    u = ei_pmul(t, t);
    f = ei_psub(ei_pset1(float(1.0)), ei_pmul(u, polevl( u, FC::fn)));
    g = ei_pmul(t, polevl( u, FC::gn));

    t = ei_pmul(ei_pset1(float(HALFPI)), x2);

    t = packetFmod(t, ei_pset1(float(TWOPI)));

    c = ei_pcos(t);
    s = ei_psin(t);

    t = ei_pdiv(ei_pset1(float(1. / PI)), x);
    cc = ei_padd(ei_pset1(float(0.5)), ei_pmul(t, ei_psub(ei_pmul(f, s), ei_pmul(g, c))));
    ss = ei_psub(ei_pset1(float(0.5)), ei_pmul(t, ei_psub(ei_pmul(f, c), ei_pmul(g, s))));

    *ssa = packetTransferSign(ss, xxa);
    *cca = packetTransferSign(cc, xxa);
}

void fresnelNoVec(const VectorXd &t, VectorXd &s, VectorXd &c)
{
    for(int i = 0; i < t.size(); ++i)
        fresnel(t[i], &(s[i]), &(c[i]));
}

void fresnelVec(const VectorXd &t, VectorXd &s, VectorXd &c)
{
    const int packetSize = 4;
    typedef Packet4f Packet;
    typedef float Scalar;

    typedef Matrix<float, packetSize, 1> ValVec;
    typedef Matrix<int, packetSize, 1> IdxVec;

    s.resize(t.size());
    c.resize(t.size());

    ValVec lowVal, medVal;
    IdxVec lowIdx, medIdx;
    Packet pval, ps, pc;
    ValVec vs, vc;
    int lowNum = 0, medNum = 0;

    for(int i = 0; i < t.size(); ++i)
    {
        double vsq = t[i] * t[i];
        if(vsq > 1367076676)
        {
            if(t[i] < 0)
                s[i] = c[i] = -0.5;
            else
                s[i] = c[i] = 0.5;

            continue;
        }
        if(vsq < 2.5625) //low
        {
            lowVal[lowNum] = Scalar(t[i]);
            lowIdx[lowNum++] = i;

            if(lowNum == packetSize)
            {
                pval = lowVal.packet<Aligned>(0);
                fresnelLow(pval, &ps, &pc);
                vs.writePacket<Aligned>(0, ps);
                vc.writePacket<Aligned>(0, pc);
                for(int j = 0; j < packetSize; ++j)
                {
                    s[lowIdx[j]] = vs[j];
                    c[lowIdx[j]] = vc[j];
                }
                lowNum = 0;
            }
        }
        else //med
        {
            medVal[medNum] = Scalar(t[i]);
            medIdx[medNum++] = i;

            if(medNum == packetSize)
            {
                pval = medVal.packet<Aligned>(0);
                fresnelMed(pval, &ps, &pc);
                vs.writePacket<Aligned>(0, ps);
                vc.writePacket<Aligned>(0, pc);
                for(int j = 0; j < packetSize; ++j)
                {
                    s[medIdx[j]] = vs[j];
                    c[medIdx[j]] = vc[j];
                }
                medNum = 0;
            }
        }
    }

    //finish up
    for(int i = 0; i < lowNum; ++i)
        fresnel(lowVal[i], &(s[lowIdx[i]]), &(c[lowIdx[i]]));
    for(int i = 0; i < medNum; ++i)
        fresnel(medVal[i], &(s[medIdx[i]]), &(c[medIdx[i]]));
}

inline double f(int i, int num)
{
    //return double(rand() % 100) / 10. - 5.;
    return double(rand() % 1000) / 1000. - .5;
}

void runTest()
{
    const int num = 10000000;

    VectorXd t(num), s(num), c(num);

    for(int i = 0; i < num; ++i) {
        t[i] = f(i, num);
    }

    Debugging::get()->startTiming("Fresnel scalar");
    fresnelNoVec(t, s, c);
    Debugging::get()->elapsedTime("Fresnel scalar");

    Debugging::get()->printf("Done, sum = %lf\n", (s + c).sum());

    Debugging::get()->startTiming("Fresnel 4f");
    fresnelVec(t, s, c);
    Debugging::get()->elapsedTime("Fresnel 4f");

    Debugging::get()->printf("Done, sum = %lf\n", (s + c).sum());
}

END_NAMESPACE_Cornu

