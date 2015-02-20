/*
 ==============================================================================
 
 This file is part of the KIWI library.
 Copyright (c) 2014 Pierre Guillot & Eliott Paris.
 
 Permission is granted to use this software under the terms of either:
 a) the GPL v2 (or any later version)
 b) the Affero GPL v3
 
 Details of these licenses can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 To release a closed-source product which uses KIWI, contact : guillotpierre6@gmail.com
 
 ==============================================================================
*/

#ifndef __DEF_KIWI_GUI_TOOLS__
#define __DEF_KIWI_GUI_TOOLS__

#include <stdio.h>
#include <stdarg.h>
#include <cwchar>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <memory>
#include <cmath>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <deque>
#include <thread>
#include <typeinfo>
#include <typeindex>

#ifdef __APPLE__
#include <Accelerate/Accelerate.h>
#endif

using namespace std;

#define _USE_MATH_DEFINES

#ifndef M_PI
#define M_E         2.71828182845904523536028747135266250   /* e              */
#define M_LOG2E     1.44269504088896340735992468100189214   /* log2(e)        */
#define M_LOG10E    0.434294481903251827651128918916605082  /* log10(e)       */
#define M_LN2       0.693147180559945309417232121458176568  /* loge(2)        */
#define M_LN10      2.30258509299404568401799145468436421   /* loge(10)       */
#define M_PI        3.14159265358979323846264338327950288   /* pi             */
#define M_PI_2      1.57079632679489661923132169163975144   /* pi/2           */
#define M_PI_4      0.785398163397448309615660845819875721  /* pi/4           */
#define M_1_PI      0.318309886183790671537767526745028724  /* 1/pi           */
#define M_2_PI      0.636619772367581343075535053490057448  /* 2/pi           */
#define M_2_SQRTPI  1.12837916709551257389615890312154517   /* 2/sqrt(pi)     */
#define M_SQRT2     1.41421356237309504880168872420969808   /* sqrt(2)        */
#define M_SQRT1_2   0.707106781186547524400844362104849039  /* 1/sqrt(2)      */
#endif

namespace Kiwi
{
#ifndef ulong
    typedef unsigned long ulong;
#endif
    
#ifndef __KIWI_SHARED_FROM_THIS__
#define __KIWI_SHARED_FROM_THIS__
    class multiinheritable_enable_shared_from_this: public enable_shared_from_this<multiinheritable_enable_shared_from_this>
    {
    public:
        virtual ~multiinheritable_enable_shared_from_this(){}
    };
    
    template <class T> class inheritable_enable_shared_from_this : virtual public multiinheritable_enable_shared_from_this
    {
    public:
        shared_ptr<T> shared_from_this() noexcept
        {
            return dynamic_pointer_cast<T>(multiinheritable_enable_shared_from_this::shared_from_this());
        }
        
        shared_ptr<const T> shared_from_this() const noexcept
        {
            return dynamic_pointer_cast<const T>(multiinheritable_enable_shared_from_this::shared_from_this());
        }
    };
    
    typedef multiinheritable_enable_shared_from_this multi_shared_from_this;
#endif
    
    inline string trimDecimal(string& text)
    {
        string::size_type pos = text.find('.');
        if(pos != string::npos)
        {
            string::size_type size = text.size();
            while(size > pos && text[size - 1] == '0')
            {
                text.pop_back();
                size--;
            }
        }
        return text;
    }
    
    inline string toString(bool __val)
    {
        if(__val)
        {
            return "true";
        }
        else
        {
            return "false";
        }
    }
    
    inline string toString(int __val)
    {
        return to_string(__val);
    }
    
    inline string toString(unsigned __val)
    {
        return to_string(__val);
    }
    
    inline string toString(long __val)
    {
        return to_string(__val);
    }
    
    inline string toString(ulong __val)
    {
        return to_string(__val);
    }
    
    inline string toString(long long __val)
    {
        return to_string(__val);
    }
    
    inline string toString(unsigned long long __val)
    {
        return to_string(__val);
    }
    
    inline string toString(float __val, bool trim = true)
    {
        if(trim)
        {
            string text = to_string(__val);
            return trimDecimal(text);
        }
        else
        {
            return to_string(__val);
        }
    }
    
    inline string toString(double __val, bool trim = true)
    {
        if(trim)
        {
            string text = to_string(__val);
            return trimDecimal(text);
        }
        else
        {
            return to_string(__val);
        }
    }
    
    inline string toString(long double __val, bool trim = true)
    {
        if(trim)
        {
            string text = to_string(__val);
            return trimDecimal(text);
        }
        else
        {
            return to_string(__val);
        }
    }
    
    template<class T> inline T fromString(string const& __val)
    {
        return T();
    }
    
    template<> inline bool fromString(string const& __val)
    {
        string::size_type pos = __val.find_first_of("-0123456789");
        if(pos != string::npos)
        {
            return (bool)stol(__val.c_str()+pos);
        }
        else
        {
            return false;
        }
    }
    
    template<> inline int fromString(string const& __val)
    {
        string::size_type pos = __val.find_first_of("-0123456789");
        if(pos != string::npos)
        {
            return stoi(__val.c_str()+pos);
        }
        else
        {
            return 0;
        }
    }
    
    template<> inline long fromString(string const& __val)
    {
        string::size_type pos = __val.find_first_of("-0123456789");
        if(pos != string::npos)
        {
            return stol(__val.c_str()+pos);
        }
        else
        {
            return 0;
        }
    }
    
    template<> inline ulong fromString(string const& __val)
    {
        string::size_type pos = __val.find_first_of("0123456789");
        if(pos != string::npos)
        {
            return stoul(__val.c_str()+pos);
        }
        else
        {
            return 0;
        }
    }
    
    template<> inline long long fromString(string const& __val)
    {
        string::size_type pos = __val.find_first_of("-0123456789");
        if(pos != string::npos)
        {
            return stoll(__val.c_str()+pos);
        }
        else
        {
            return 0;
        }
    }
    
    template<> inline unsigned long long fromString(string const& __val)
    {
        string::size_type pos = __val.find_first_of("0123456789");
        if(pos != string::npos)
        {
            return stoull(__val.c_str()+pos);
        }
        else
        {
            return 0;
        }
    }
    
    template<> inline float fromString(string const& __val)
    {
        string::size_type pos = __val.find_first_of("-0123456789.");
        if(pos != string::npos)
        {
            return stof(__val.c_str()+pos);
        }
        else
        {
            return 0;
        }
    }
    
    template<> inline double fromString(string const& __val)
    {
        string::size_type pos = __val.find_first_of("-0123456789.");
        if(pos != string::npos)
        {
            return stod(__val.c_str()+pos);
        }
        else
        {
            return 0;
        }
    }
    
    template<> inline long double fromString(string const& __val)
    {
        string::size_type pos = __val.find_first_of("-0123456789.");
        if(pos != string::npos)
        {
            return stold(__val.c_str()+pos);
        }
        else
        {
            return 0;
        }
    }
    
    template <typename Type> Type clip(const Type& n, const Type& lower, const Type& upper)
    {
        return max(lower, min(n, upper));
    }
    
    // Solve a 3rd degree equation, return the number of solution and the solution are in solution1, solution2 and solution3.
    template <typename Type> ulong solve(Type a, Type b, Type c, Type const d, Type &solution1, Type &solution2,  Type &solution3)
    {
        if(abs(a) > 0.)
        {
            Type z = a;
            a = b / z;
            b = c / z;
            c = d / z;
            
            Type p = b - a * a / 3.;
            Type q = a * (2. * a * a - 9. * b) / 27. + c;
            Type p3 = p * p * p;
            Type D = q * q + 4. * p3 / 27.;
            Type offset = -a / 3.;
            if(D == 0.)
            {
                Type u;
                if(q < 0.)
                {
                    u = pow( -q / 2., 1. / 3.);
                }
                else
                {
                    u = -pow( q / 2., 1. / 3.);
                }
                solution1 = 2. * u + offset;
                solution2 = -u + offset;
                return 2;
            }
            else if(D > 0.)
            {
                z = sqrt(D);
                Type u = ( -q + z) / 2.;
                Type v = ( -q - z) / 2.;
                u = (u >= 0.) ? pow(u, 1. / 3.) : - pow( -u, 1. / 3.);
                v = (v >= 0.) ? pow(v, 1. / 3.) : - pow( -v, 1. / 3.);
                solution1 = u + v + offset;
                return 1;
            }
            else
            {
                Type u = 2. * sqrt( -p / 3.);
                Type v = acos(-sqrt( -27. / p3) * q / 2.) / 3.;
                solution1 = u * cos(v) + offset;
                solution2 = u * cos(v + 2. * M_PI / 3.) + offset;
                solution3 = u * cos(v + 4. * M_PI / 3.) + offset;
                return 3;
            }
        }
        else
        {
            a = b;
            b = c;
            c = d;
            if(abs(a) <= 0.)
            {
                if(abs(b) <= 0.)
                {
                    return 0;
                }
                else
                {
                    solution1 = -c / b;
                    return 1;
                }
            }
            
            Type D = b*b - 4.*a*c;
            if(D == 0)
            {
                solution1 = -b / (2. * a);
                return 1;
            }
            if(D > 0.)
            {
                D = sqrt(D);
                solution1 = ( -b - D) / (2. * a);
                solution2 = ( -b + D) / (2. * a);
                return 2;
            }
            else
            {
                return 0;
            }
        }
    }

    namespace Gui
    {
        class Sketcher;
        typedef shared_ptr<Sketcher>          sSketcher;
        typedef weak_ptr<Sketcher>            wSketcher;
        typedef shared_ptr<const Sketcher>    scSketcher;
        typedef weak_ptr<const Sketcher>      wcSketcher;
        
        class Mouser;
        typedef shared_ptr<Mouser>          sMouser;
        typedef weak_ptr<Mouser>            wMouser;
        typedef shared_ptr<const Mouser>    scMouser;
        typedef weak_ptr<const Mouser>      wcMouser;
        
        class Keyboarder;
        typedef shared_ptr<Keyboarder>          sKeyboarder;
        typedef weak_ptr<Keyboarder>            wKeyboarder;
        typedef shared_ptr<const Keyboarder>    scKeyboarder;
        typedef weak_ptr<const Keyboarder>      wcKeyboarder;
        
        
    }
    
};

#endif


