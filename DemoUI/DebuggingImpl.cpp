/*--
    DebuggingImpl.cpp  

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

#include "DebuggingImpl.h"

#include <cstdarg>

using namespace std;
using namespace Eigen;

DebuggingImpl *DebuggingImpl::get()
{
    //see if an instance exists
    DebuggingImpl *cur = dynamic_cast<DebuggingImpl *>(Cornu::Debugging::get());
    if(cur)
        return cur;

    //create new instance
    cur = new DebuggingImpl();
    Cornu::Debugging::set(cur);
    return cur;
}

void DebuggingImpl::printf(const char *fmt, ...)
{
    const int sz = 200; //we don't need terribly long debug strings
    char buffer[sz];

    va_list ap;
    va_start(ap, fmt);

    vsnprintf(buffer, sz, fmt, ap);

    va_end(ap);

    std::printf("%s", buffer);

    emit print(QString::fromAscii(buffer));
}

void DebuggingImpl::startTiming(const string &description)
{
}

void DebuggingImpl::stopTiming(const string &description)
{
}

#include "DebuggingImpl.moc"
