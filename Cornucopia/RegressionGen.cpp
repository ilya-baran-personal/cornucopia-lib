/*--
    RegressionGen.cpp  

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

//Automatically generated file -- DO NOT EDIT

#include "Regression.h"

using namespace Eigen;
NAMESPACE_Cornu

DataModel::DataModel()
{
    _coefs.resize(10923);

    _coefs[0].resize(4);
    _coefs[0] << 0, 0, 0.233595, 5.23536;

    _coefs[34].resize(4);
    _coefs[34] << 0, 0, 0.233595, 5.23536;

    _coefs[85].resize(4);
    _coefs[85] << 0, 0, 0.233595, 5.23536;

    _coefs[136].resize(4);
    _coefs[136] << 0, 0, 0.233595, 5.23536;

    _coefs[170].resize(4);
    _coefs[170] << 0, 0, 0.233595, 5.23536;

    _coefs[256].resize(4);
    _coefs[256] << 0, 0, 0.0938049, 8.53518;

    _coefs[290].resize(4);
    _coefs[290] << 0, 0, 0.0655577, 6.41661;

    _coefs[341].resize(4);
    _coefs[341] << 0, 0, 0.0913202, 7.55731;

    _coefs[392].resize(4);
    _coefs[392] << 0, 0, 0.0938049, 8.53518;

    _coefs[426].resize(4);
    _coefs[426] << 0, 0, 0.0655577, 6.41661;

    _coefs[512].resize(4);
    _coefs[512] << 0, 0, 0.0675304, 6.94236;

    _coefs[514].resize(4);
    _coefs[514] << 0, 0, 0.0293391, 7.42931;

    _coefs[544].resize(4);
    _coefs[544] << 0, 0, 0.0157273, 10.4826;

    _coefs[546].resize(4);
    _coefs[546] << 0, 0, 0.0566565, 4.6675;

    _coefs[597].resize(4);
    _coefs[597] << 0, 0, 0.0420447, 7.82224;

    _coefs[648].resize(4);
    _coefs[648] << 0, 0, 0.0675304, 6.94236;

    _coefs[650].resize(4);
    _coefs[650] << 0, 0, 0.0293391, 7.42931;

    _coefs[680].resize(4);
    _coefs[680] << 0, 0, 0.0157273, 10.4826;

    _coefs[682].resize(4);
    _coefs[682] << 0, 0, 0.0566565, 4.6675;

    _coefs[1024].resize(4);
    _coefs[1024] << 0, 0, 0.187817, 8.85198;

    _coefs[1058].resize(4);
    _coefs[1058] << 0, 0, 0.187817, 8.85198;

    _coefs[1109].resize(4);
    _coefs[1109] << 0, 0, 0.237006, 8.93426;

    _coefs[1160].resize(4);
    _coefs[1160] << 0, 0, 0.322627, 9.37613;

    _coefs[1194].resize(4);
    _coefs[1194] << 0, 0, 0.322627, 9.37613;

    _coefs[1280].resize(4);
    _coefs[1280] << 0, 0, 0.0917071, 15.0398;

    _coefs[1314].resize(4);
    _coefs[1314] << 0, 0, 0.131673, 6.9369;

    _coefs[1365].resize(4);
    _coefs[1365] << 0, 0, 0.114716, 12.3257;

    _coefs[1416].resize(4);
    _coefs[1416] << 0, 0, 0.0964134, 17.1913;

    _coefs[1450].resize(4);
    _coefs[1450] << 0, 0, 0.0612493, 11.0557;

    _coefs[1536].resize(4);
    _coefs[1536] << 0, 0, 0.0544021, 11.9689;

    _coefs[1538].resize(4);
    _coefs[1538] << 0, 0, 0.0245737, 12.3513;

    _coefs[1568].resize(4);
    _coefs[1568] << 0, 0, 0.019523, 16.6008;

    _coefs[1570].resize(4);
    _coefs[1570] << 0, 0, 0.0798312, 7.51773;

    _coefs[1621].resize(4);
    _coefs[1621] << 0, 0, 0.0553511, 11.4897;

    _coefs[1672].resize(4);
    _coefs[1672] << 0, 0, 0.052132, 16.1657;

    _coefs[1674].resize(4);
    _coefs[1674] << 0, 0, 0.0252412, 13.0833;

    _coefs[1704].resize(4);
    _coefs[1704] << 0, 0, 0, 17.2563;

    _coefs[1706].resize(4);
    _coefs[1706] << 0, 0, 0.0501102, 8.72824;

    _coefs[2048].resize(4);
    _coefs[2048] << 0, 0, 0, 2.93347;

    _coefs[2056].resize(4);
    _coefs[2056] << 0, 0, 0.235226, 9.16556;

    _coefs[2082].resize(4);
    _coefs[2082] << 0, 0, 0, 2.93347;

    _coefs[2090].resize(4);
    _coefs[2090] << 0, 0, 0.235226, 9.16556;

    _coefs[2133].resize(4);
    _coefs[2133] << 0, 0, 0.233499, 10.5962;

    _coefs[2176].resize(4);
    _coefs[2176] << 0, 0, 0.21552, 12.0444;

    _coefs[2184].resize(4);
    _coefs[2184] << 0, 0, 0.224621, 5.90657;

    _coefs[2210].resize(4);
    _coefs[2210] << 0, 0, 0.21552, 12.0444;

    _coefs[2218].resize(4);
    _coefs[2218] << 0, 0, 0.224621, 5.90657;

    _coefs[2304].resize(4);
    _coefs[2304] << 0, 0, 0, 8.00404;

    _coefs[2312].resize(4);
    _coefs[2312] << 0, 0, 0.0923659, 12.4617;

    _coefs[2338].resize(4);
    _coefs[2338] << 0, 0, 0, 2.51139;

    _coefs[2346].resize(4);
    _coefs[2346] << 0, 0, 0.0473623, 11.172;

    _coefs[2389].resize(4);
    _coefs[2389] << 0, 0, 0.1014, 13.6053;

    _coefs[2432].resize(4);
    _coefs[2432] << 0, 0, 0.105547, 15.372;

    _coefs[2440].resize(4);
    _coefs[2440] << 0, 0, 0.0199605, 16.2823;

    _coefs[2466].resize(4);
    _coefs[2466] << 0, 0, 0.0954904, 11.1696;

    _coefs[2474].resize(4);
    _coefs[2474] << 0, 0, 0, 6.56597;

    _coefs[2560].resize(4);
    _coefs[2560] << 0, 0, 0, 4.40543;

    _coefs[2562].resize(4);
    _coefs[2562] << 0, 0, 0, 5.38924;

    _coefs[2568].resize(4);
    _coefs[2568] << 0, 0, 0.0607223, 11.865;

    _coefs[2570].resize(4);
    _coefs[2570] << 0, 0, 0.019902, 10.7573;

    _coefs[2592].resize(4);
    _coefs[2592] << 0, 0, 0, 12.9709;

    _coefs[2594].resize(4);
    _coefs[2594] << 0, 0, 0, 3.73489;

    _coefs[2600].resize(4);
    _coefs[2600] << 0, 0, 0, 22.2524;

    _coefs[2602].resize(4);
    _coefs[2602] << 0, 0, 0.0881276, 8.15615;

    _coefs[2645].resize(4);
    _coefs[2645] << 0, 0, 0.0516551, 13.4064;

    _coefs[2688].resize(4);
    _coefs[2688] << 0, 0, 0.0114301, 18.9998;

    _coefs[2690].resize(4);
    _coefs[2690] << 0, 0, 0.0452404, 13.5551;

    _coefs[2696].resize(4);
    _coefs[2696] << 0, 0, 0, 16.2;

    _coefs[2698].resize(4);
    _coefs[2698] << 0, 0, 0, 11.8531;

    _coefs[2720].resize(4);
    _coefs[2720] << 0, 0, 0.0518298, 8.96733;

    _coefs[2722].resize(4);
    _coefs[2722] << 0, 0, 0.0558111, 9.16592;

    _coefs[2728].resize(4);
    _coefs[2728] << 0, 0, 0.00785475, 10.1565;

    _coefs[2730].resize(4);
    _coefs[2730] << 0, 0, 0, 3.64945;

    _coefs[4352].resize(5);
    _coefs[4352] << 0, 0, 0, 0, 0.946916;

    _coefs[4386].resize(5);
    _coefs[4386] << 0, 0.0117837, 0.0245888, 0, 2.80797;

    _coefs[4437].resize(5);
    _coefs[4437] << 0, 0, 0, 0, 1.09015;

    _coefs[4488].resize(5);
    _coefs[4488] << 0, 0, 0, 0, 0.946916;

    _coefs[4522].resize(5);
    _coefs[4522] << 0, 0.0117837, 0.0245888, 0, 2.80797;

    _coefs[4608].resize(5);
    _coefs[4608] << 0, 0, 0, 0, 0.500953;

    _coefs[4610].resize(5);
    _coefs[4610] << 0, 0, 0, 0, 0.246847;

    _coefs[4640].resize(5);
    _coefs[4640] << 0, 0, 0, 0, 0.777361;

    _coefs[4642].resize(5);
    _coefs[4642] << 0, 0.199289, 0, 0, 0.711228;

    _coefs[4693].resize(5);
    _coefs[4693] << 0, 0, 0, 0, 0.50063;

    _coefs[4744].resize(5);
    _coefs[4744] << 0, 0, 0, 0, 0.500953;

    _coefs[4746].resize(5);
    _coefs[4746] << 0, 0, 0, 0, 0.246847;

    _coefs[4776].resize(5);
    _coefs[4776] << 0, 0, 0, 0, 0.777361;

    _coefs[4778].resize(5);
    _coefs[4778] << 0, 0.199289, 0, 0, 0.711228;

    _coefs[5120].resize(5);
    _coefs[5120] << 0, 0, 0.0179234, 28.1036, 1.71145;

    _coefs[5154].resize(5);
    _coefs[5154] << 0, 0, 0.0179234, 28.1036, 1.71145;

    _coefs[5205].resize(5);
    _coefs[5205] << 0, 0, 0.204465, 16.6723, 2.01103;

    _coefs[5256].resize(5);
    _coefs[5256] << 0, 0, 0.369875, 0, 2.88034;

    _coefs[5290].resize(5);
    _coefs[5290] << 0, 0, 0.369875, 0, 2.88034;

    _coefs[5376].resize(5);
    _coefs[5376] << 0, 0, 0.0818611, 7.21192, 1.17277;

    _coefs[5410].resize(5);
    _coefs[5410] << 0, 0, 0, 0, 1.53307;

    _coefs[5461].resize(5);
    _coefs[5461] << 0, 0, 0.0586138, 0, 1.504;

    _coefs[5512].resize(5);
    _coefs[5512] << 0, 0, 0, 0, 3.59826;

    _coefs[5546].resize(5);
    _coefs[5546] << 0, 0, 0.111443, 2.21503, 1.8597;

    _coefs[5632].resize(5);
    _coefs[5632] << 0, 0, 0.053014, 0, 1.21526;

    _coefs[5634].resize(5);
    _coefs[5634] << 0, 0, 0.0406892, 5.33189, 0.751862;

    _coefs[5664].resize(5);
    _coefs[5664] << 0, 0, 0.0573194, 0, 0.433321;

    _coefs[5666].resize(5);
    _coefs[5666] << 0, 0, 0, 0, 0.914861;

    _coefs[5717].resize(5);
    _coefs[5717] << 0, 0, 0.0394759, 0, 1.05835;

    _coefs[5768].resize(5);
    _coefs[5768] << 0, 0, 0, 0, 3.94263;

    _coefs[5770].resize(5);
    _coefs[5770] << 0, 0, 0.0841494, 0, 2.7281;

    _coefs[5800].resize(5);
    _coefs[5800] << 0, 0, 0.0743192, 0, 2.73285;

    _coefs[5802].resize(5);
    _coefs[5802] << 0, 0, 0.0545631, 0, 1.63939;

    _coefs[6144].resize(5);
    _coefs[6144] << 0, 0, 0, 15.0458, 1.80563;

    _coefs[6152].resize(5);
    _coefs[6152] << 0, 0, 0.345154, 3.52303, 1.97782;

    _coefs[6178].resize(5);
    _coefs[6178] << 0, 0, 0, 15.0458, 1.80563;

    _coefs[6186].resize(5);
    _coefs[6186] << 0, 0, 0.345154, 3.52303, 1.97782;

    _coefs[6229].resize(5);
    _coefs[6229] << 0, 0, 0.149903, 11.5502, 2.09998;

    _coefs[6272].resize(5);
    _coefs[6272] << 0, 0, 0, 0.388516, 2.26725;

    _coefs[6280].resize(5);
    _coefs[6280] << 0, 0, 0.316391, 0, 1.84308;

    _coefs[6306].resize(5);
    _coefs[6306] << 0, 0, 0, 0.388516, 2.26725;

    _coefs[6314].resize(5);
    _coefs[6314] << 0, 0, 0.316391, 0, 1.84308;

    _coefs[6400].resize(5);
    _coefs[6400] << 0, 0, 0, 0, 1.40436;

    _coefs[6408].resize(5);
    _coefs[6408] << 0, 0, 0.240248, 0, 1.39379;

    _coefs[6434].resize(5);
    _coefs[6434] << 0, 0, 0, 0, 1.01636;

    _coefs[6442].resize(5);
    _coefs[6442] << 0, 0, 0.250535, 0, 1.19794;

    _coefs[6485].resize(5);
    _coefs[6485] << 0, 0, 0.0655256, 0, 1.49331;

    _coefs[6528].resize(5);
    _coefs[6528] << 0, 0, 0, 0, 1.18899;

    _coefs[6536].resize(5);
    _coefs[6536] << 0, 0, 0, 0, 1.57534;

    _coefs[6562].resize(5);
    _coefs[6562] << 0, 0, 0.21238, 0, 1.70439;

    _coefs[6570].resize(5);
    _coefs[6570] << 0, 0, 0, 0, 1.8327;

    _coefs[6656].resize(5);
    _coefs[6656] << 0, 0, 0, 0, 0.792585;

    _coefs[6658].resize(5);
    _coefs[6658] << 0, 0, 0.0003541, 0, 0.927271;

    _coefs[6664].resize(5);
    _coefs[6664] << 0, 0, 0.120382, 0, 1.46941;

    _coefs[6666].resize(5);
    _coefs[6666] << 0, 0, 0, 0, 0.0735965;

    _coefs[6688].resize(5);
    _coefs[6688] << 0, 0, 0.0447118, 10.0376, 1.06383;

    _coefs[6690].resize(5);
    _coefs[6690] << 0, 0, 0, 0, 0.802853;

    _coefs[6696].resize(5);
    _coefs[6696] << 0, 0, 0.0545649, 24.1369, 1.91021;

    _coefs[6698].resize(5);
    _coefs[6698] << 0, 0, 0.0993695, 0, 0.559425;

    _coefs[6741].resize(5);
    _coefs[6741] << 0, 0, 0.068106, 0, 1.06;

    _coefs[6784].resize(5);
    _coefs[6784] << 0, 0, 0.0190845, 0, 1.02293;

    _coefs[6786].resize(5);
    _coefs[6786] << 0, 0, 0.077264, 0, 2.52151;

    _coefs[6792].resize(5);
    _coefs[6792] << 0, 0, 0, 0, 1.45738;

    _coefs[6794].resize(5);
    _coefs[6794] << 0, 0, 0, 0, 2.63133;

    _coefs[6816].resize(5);
    _coefs[6816] << 0, 0, 0, 0, 2.75934;

    _coefs[6818].resize(5);
    _coefs[6818] << 0, 0, 0.0915521, 0, 0.914192;

    _coefs[6824].resize(5);
    _coefs[6824] << 0, 0, 0.0310227, 0, 2.29717;

    _coefs[6826].resize(5);
    _coefs[6826] << 0, 0, 0, 0, 1.53407;

    _coefs[8704].resize(6);
    _coefs[8704] << 0, 0, 0, 0, 2.51561, 0.00675602;

    _coefs[8706].resize(6);
    _coefs[8706] << 0, 0.0133908, 0, 0, 13.7703, 6.2538;

    _coefs[8736].resize(6);
    _coefs[8736] << 0, 0, 0, 0, 0, 0.272973;

    _coefs[8738].resize(6);
    _coefs[8738] << 0, 0, 0, 0, 11.1978, 0.0193913;

    _coefs[8789].resize(6);
    _coefs[8789] << 0, 0, 0, 0, 0, 0.0306113;

    _coefs[8840].resize(6);
    _coefs[8840] << 0, 0, 0, 0, 2.51561, 0.00675602;

    _coefs[8842].resize(6);
    _coefs[8842] << 0, 0.0133908, 0, 0, 13.7703, 6.2538;

    _coefs[8872].resize(6);
    _coefs[8872] << 0, 0, 0, 0, 0, 0.272973;

    _coefs[8874].resize(6);
    _coefs[8874] << 0, 0, 0, 0, 11.1978, 0.0193913;

    _coefs[9728].resize(6);
    _coefs[9728] << 0, 0, 0.0427182, 0, 1.35517, 0.0754867;

    _coefs[9730].resize(6);
    _coefs[9730] << 0, 0, 0, 0, 0, 0.176705;

    _coefs[9760].resize(6);
    _coefs[9760] << 0, 0, 0.0839946, 0, 4.82725, 0;

    _coefs[9762].resize(6);
    _coefs[9762] << 0, 0, 0, 0, 0, 0;

    _coefs[9813].resize(6);
    _coefs[9813] << 0, 0, 0.0740334, 0, 2.54875, 0.0590062;

    _coefs[9864].resize(6);
    _coefs[9864] << 0, 0, 0, 0, 5.13605, 0;

    _coefs[9866].resize(6);
    _coefs[9866] << 0, 0, 0.107649, 0, 5.52227, 0.458625;

    _coefs[9896].resize(6);
    _coefs[9896] << 0, 0, 0.192201, 0, 3.50646, 0;

    _coefs[9898].resize(6);
    _coefs[9898] << 0, 0, 0.196901, 0, 2.18124, 0.160522;

    _coefs[10240].resize(6);
    _coefs[10240] << 0, 0, 0.147273, 0, 0.42204, 1.84509;

    _coefs[10248].resize(6);
    _coefs[10248] << 0, 0.263365, 0.462846, 0, 0, 2.8034;

    _coefs[10274].resize(6);
    _coefs[10274] << 0, 0, 0.147273, 0, 0.42204, 1.84509;

    _coefs[10282].resize(6);
    _coefs[10282] << 0, 0.263365, 0.462846, 0, 0, 2.8034;

    _coefs[10325].resize(6);
    _coefs[10325] << 0, 0.259137, 0.434013, 0, 0.376747, 1.45647;

    _coefs[10368].resize(6);
    _coefs[10368] << 0, 0, 0.37277, 0, 0.511424, 3.02791;

    _coefs[10376].resize(6);
    _coefs[10376] << 0, 0, 0.279494, 0, 1.66875, 1.22911;

    _coefs[10402].resize(6);
    _coefs[10402] << 0, 0, 0.37277, 0, 0.511424, 3.02791;

    _coefs[10410].resize(6);
    _coefs[10410] << 0, 0, 0.279494, 0, 1.66875, 1.22911;

    _coefs[10496].resize(6);
    _coefs[10496] << 0, 0, 0.0893151, 6.58665, 2.73021, 0.344412;

    _coefs[10504].resize(6);
    _coefs[10504] << 0, 0, 0.156211, 45.529, 0, 0.235551;

    _coefs[10530].resize(6);
    _coefs[10530] << 0, 0, 0, 0, 0, 0.062764;

    _coefs[10538].resize(6);
    _coefs[10538] << 0, 0, 0.556951, 0.379449, 2.6364, 0.132116;

    _coefs[10581].resize(6);
    _coefs[10581] << 0, 0, 0.297416, 34.8092, 2.32367, 0.137919;

    _coefs[10624].resize(6);
    _coefs[10624] << 0, 0, 0.362145, 0, 3.91639, 0.259047;

    _coefs[10632].resize(6);
    _coefs[10632] << 0, 0, 0, 0, 0, 0.137336;

    _coefs[10658].resize(6);
    _coefs[10658] << 0, 0, 0.674673, 9.77778, 0.104101, 0.175642;

    _coefs[10666].resize(6);
    _coefs[10666] << 0, 0, 0.20688, 0, 2.13689, 0.333789;

    _coefs[10752].resize(6);
    _coefs[10752] << 0, 0, 0, 0, 0.262177, 0.101711;

    _coefs[10754].resize(6);
    _coefs[10754] << 0, 0, 0, 0, 0, 0.230777;

    _coefs[10760].resize(6);
    _coefs[10760] << 0, 0, 0.0982344, 0, 0.717822, 0.0686759;

    _coefs[10762].resize(6);
    _coefs[10762] << 0, 0, 0.171429, 0, 0, 0;

    _coefs[10784].resize(6);
    _coefs[10784] << 0, 0, 0, 0, 0.466477, 0.425908;

    _coefs[10786].resize(6);
    _coefs[10786] << 0, 0, 0, 0, 1.10213, 0;

    _coefs[10792].resize(6);
    _coefs[10792] << 0, 0, 0.162526, 0, 1.74939, 0.426546;

    _coefs[10794].resize(6);
    _coefs[10794] << 0, 0, 0.257495, 0, 0.852739, 0.0492051;

    _coefs[10837].resize(6);
    _coefs[10837] << 0, 0, 0.108742, 0, 0.767025, 0.0609417;

    _coefs[10880].resize(6);
    _coefs[10880] << 0, 0, 0, 0, 1.18179, 0.0610022;

    _coefs[10882].resize(6);
    _coefs[10882] << 0, 0, 0.158146, 0, 1.22296, 0.203253;

    _coefs[10888].resize(6);
    _coefs[10888] << 0, 0, 0, 0, 0.70768, 0;

    _coefs[10890].resize(6);
    _coefs[10890] << 0, 0, 0, 0, 3.41278, 0.446127;

    _coefs[10912].resize(6);
    _coefs[10912] << 0, 0, 0.157592, 0, 5.19678, 0;

    _coefs[10914].resize(6);
    _coefs[10914] << 0, 0, 0.151019, 0, 0.613396, 0;

    _coefs[10920].resize(6);
    _coefs[10920] << 0, 0, 0.114263, 0, 1.91024, 0.195753;

    _coefs[10922].resize(6);
    _coefs[10922] << 0, 0, 0.0582818, 0, 2.16265, 0.117338;

}

END_NAMESPACE_Cornu
