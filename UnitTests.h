/*
Copyright 2015 Laurent Claessens
contact : moky.math@gmail.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
//*/

#ifndef  __UNIT_TESTS__H
#define __UNIT_TESTS__H


// This is only because I did not RTFM [2]
// Thanks to [1] for the function pointer
// [1]  http://www.cprogramming.com/tutorial/function-pointers.html
// [2] http://cppunit.sourceforge.net/doc/lastest/cppunit_cookbook.html

class TestException : public std::exception
{
    private:
        string message;
    public:
        TestException(string);
        const char* what();
}

class GenericTestingFunction
{
    private:
        string name;
        string question;
        bool interactive=false;
        void (*test)();
    public :
        GenericTestingFunction (const string name ,const bool interactive, void (*test)(),string question="" );
        void run();
}

#endif //__UNIT_TESTS__H