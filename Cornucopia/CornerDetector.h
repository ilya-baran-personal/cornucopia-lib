/*--
    CornerDetector.h  

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

#ifndef CORNERDETECTOR_H_INCLUDED
#define CORNERDETECTOR_H_INCLUDED

#include "defs.h"
#include "Algorithm.h"

NAMESPACE_Cornu

class CornerDetector : public Algorithm<CornerDetector>
{
public:
    enum Algorithm
    {
        ALG1,
        ALG2
    };

    //virtual void run(const Parameters &parameters, const ) = 0;

    static void _initializePrivate();
};

END_NAMESPACE_Cornu

#endif //CORNERDETECTOR_H_INCLUDED
