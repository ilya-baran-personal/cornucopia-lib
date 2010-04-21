/*--
    Test.cpp  

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

#include "Test.h"
#include "Debugging.h"
#include <cstdio>
#include <ctime>
#include <cstdarg>
#include <map>

using namespace std;
using namespace Eigen;
using namespace Cornu;

class DebuggingTestImpl : public Cornu::Debugging
{
public:
    static DebuggingTestImpl *get()
    {
        //see if an instance exists
        DebuggingTestImpl *cur = dynamic_cast<DebuggingTestImpl *>(Cornu::Debugging::get());
        if(cur)
            return cur;

        //create new instance
        cur = new DebuggingTestImpl();
        Cornu::Debugging::set(cur);
        return cur;
    }

    //overrides
    void printf(const char *fmt, ...)
    {
        const int sz = 200; //we don't need terribly long debug strings
        char buffer[sz];

        va_list ap;
        va_start(ap, fmt);

        vsnprintf(buffer, sz, fmt, ap);

        va_end(ap);

        for(int i = 0; i < _indent * 10; ++i)
            std::printf(".");
        std::printf("%s\n", buffer);
    }

    void startTiming(const string &description)
    {
        _startTimes[description] = clock();
    }
    void elapsedTime(const string &description = "")
    {
        if(!_startTimes.count(description))
        {
            printf("Timing description not found: %s", description.c_str());

            return;
        }
        int diff = clock() - _startTimes[description];
        printf("Timing: %20s   ---   %.3lf", description.c_str(), double(diff) / CLOCKS_PER_SEC);
    }

    //not overrides
    void indent() { ++_indent; }
    void unindent() { --_indent; }

private:
    DebuggingTestImpl() : _indent(0) {}

    map<string, int> _startTimes; 
    int _indent;
};

std::vector<TestCase *> &TestCase::allTests()
{
    static std::vector<TestCase *> _allTests;
    return _allTests;
}

//To make sure tests run in a consistent order, we sort them by name
struct TestComparator
{
    bool operator()(TestCase *t1, TestCase *t2) const { return t1->name() < t2->name(); }
};

int main()
{
    std::sort(TestCase::allTests().begin(), TestCase::allTests().end(), TestComparator());

    DebuggingTestImpl::get()->printf("Starting tests\n");

    bool anyFailed = false;

    for(int i = 0; i < (int)TestCase::allTests().size(); ++i)
    {
        if(i > 0)
            DebuggingTestImpl::get()->printf("");

        TestCase *test = TestCase::allTests()[i];
        string name = test->name();
        DebuggingTestImpl::get()->printf("running %s", name.c_str());
        bool failed = true;
        try
        {
            DebuggingTestImpl::get()->startTiming(name);
            DebuggingTestImpl::get()->indent();
            test->run();
            failed = false;
            DebuggingTestImpl::get()->unindent();
        }
        catch(Assertion)
        {
            DebuggingTestImpl::get()->unindent();
        }
        catch(...)
        {
            DebuggingTestImpl::get()->unindent();
            DebuggingTestImpl::get()->printf("Exception thrown");
        }
        DebuggingTestImpl::get()->elapsedTime(name);
        if(failed)
            DebuggingTestImpl::get()->printf("FAILED! %s", name.c_str());
        else
            DebuggingTestImpl::get()->printf("passed  %s", name.c_str());
        anyFailed = anyFailed || failed;
    }

    return anyFailed ? 1 : 0;
}
