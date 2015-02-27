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

#include "KiwiPoint.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      POINT                                       //
    // ================================================================================ //
    
    Point::Point() noexcept :
    m_x(0.), m_y(0.)
    {
        ;
    }
    
    Point::Point(const double x, const double y) noexcept :
    m_x(x), m_y(y)
    {
        ;
    }
    
    Point::Point(Point const& pt) noexcept :
    m_x(pt.m_x), m_y(pt.m_y)
    {
        ;
    }
    
    Point::~Point() noexcept
    {
        ;
    }
    
    Point Point::fromLine(Point const& begin, Point const& end, double delta) noexcept
    {
        return Point(begin.x() + (end.x() - begin.x()) * delta, begin.y() + (end.y() - begin.y()) * delta);
    }
    
    Point Point::fromLine(Point const& begin, Point const& ctrl, Point const& end, const double delta) noexcept
    {
        const double mdelta = (1. - delta);
        const double fac1 = mdelta * mdelta;
        const double fac2 = 2. * delta * mdelta;
        const double fac3 = delta * delta;
        return Point(begin.x() * fac1 + ctrl.x() * fac2 + end.x() * fac3, begin.y() * fac1 + ctrl.y() * fac2 + end.y() * fac3);
    }
    
    Point Point::fromLine(Point const& begin, Point const& ctrl1, Point const& ctrl2, Point const& end, const double delta) noexcept
    {
        const double mdelta = (1. - delta);
        const double fac1 = mdelta * mdelta * mdelta;
        const double fac2 = 3. * delta * mdelta * mdelta;
        const double fac3 = 3. * delta * delta * mdelta;
        const double fac4 = delta * delta * mdelta;
        return Point(begin.x() * fac1 + ctrl1.x() * fac2 + ctrl2.x() * fac3 + end.x() * fac4, begin.y() * fac1 + ctrl1.y() * fac2 + ctrl2.y() * fac3 + end.y() * fac4);
    }
    
    double Point::distance(Point const& begin, Point const& end) const noexcept
    {
        const Point delta(end - begin);
        const double length = delta.x() * delta.x() + delta.y() * delta.y();
        
        if(length > 0.)
        {
            const double ratio = ((m_x - begin.x()) * delta.x() + (m_x - begin.x()) * delta.x()) / length;
            if(ratio < 0.)
            {
                return this->distance(begin);
            }
            else if(ratio > 1.)
            {
                return this->distance(end);
            }
            else
            {
                return this->distance((ratio * delta) + begin);
            }
        }
        else
        {
            const double distBegin  = this->distance(begin);
            const double distEnd    = this->distance(end);
            if(distBegin < distEnd)
            {
                return distBegin;
            }
            else
            {
                return distEnd;
            }
        }
    }
    
    double Point::distance(Point const& begin, Point const& ctrl, Point const& end) const noexcept
    {
        const Point A(ctrl.x() - begin.x(), ctrl.y() - begin.y());
        const Point B(begin.x() - ctrl.x() * 2. + end.x(), begin.y() - ctrl.y() * 2. + end.y());
        const Point C(begin.x() - m_x, begin.y() - m_y);
        
        double sol1, sol2, sol3;
        const ulong nresult = solve(B.x() * B.x() + B.y() * B.y(), 3 * (A.x() * B.x() + A.y() * B.y()), 2 * (A.x() * A.x() + A.y() * A.y()) + C.x() * B.x() + C.y() * B.y(), C.x() * A.x() + C.y() * A.y(), sol1, sol2, sol3);
        if(nresult)
        {
            double dist = this->distance(fromLine(begin, ctrl, end, sol1));
            if(nresult > 1)
            {
                const double dist2 = this->distance(fromLine(begin, ctrl, end, sol2));
                if(dist2 < dist)
                {
                    dist = dist2;
                }
            }
            if(nresult > 2)
            {
                const double dist2 = this->distance(fromLine(begin, ctrl, end, sol3));
                if(dist2 < dist)
                {
                    dist  = dist2;
                }
            }
            return dist;
        }
        else
        {
            return min(this->distance(begin), this->distance(end));
        }
    }
    
    double Point::distance(Point const& begin, Point const& ctrl1, Point const& ctrl2, Point const& end) const noexcept
    {
        array<Point, 6> W = {Point(0., 0.), Point(0.2, 0.), Point(0.4, 0.), Point(0.6, 0.), Point(0.8, 0.), Point(1., 0.)};
        array<Point, 4> C = {Point(begin - *this), Point(ctrl1 - *this), Point(ctrl2 - *this), Point(end - *this)};
        array<Point, 3> D = {Point((ctrl1 - begin) * 3.), Point((ctrl2 - ctrl1) * 3.), Point((end - ctrl2) * 3.)};
        static const double z[3][4] ={{1.0, 0.6, 0.3, 0.1}, {0.4, 0.6, 0.6, 0.4}, {0.1, 0.3, 0.6, 1.0}};
        double 	cd[3][4];
        for(int i = 0; i < 3; i++)
        {
            for(int j = 0; j < 4; j++)
            {
                cd[i][j] = D[i].dot(C[j]);
            }
        }
        for(int k = 0; k < 6; k++)
        {
            for(int i = max(0, k - 2); i <= min(k, 3); i++)
            {
                W[k].y(W[k].y() + cd[k - i][i] * z[k - i][i]);
            }
        }
        
        double 	t_candidate[5];
        ulong n_solutions = solve(W, t_candidate, 0ul);
        
        double dist = distance(end);
        for(int i = 0; i < n_solutions; i++)
        {
            const double new_dist = distance(Point::fromLine(begin, ctrl1, ctrl2, end, t_candidate[i]));
            if(new_dist < dist)
            {
                dist = new_dist;
            }
        }
        return dist;
    }
    
    bool Point::near(Point const& pt, double const dist) const noexcept
    {
        return this->distance(pt) <= dist;
    }
    
    bool Point::near(Point const& begin, Point const& end, double const dist) const noexcept
    {
        return this->distance(begin, end) <= dist;
    }
    
    bool Point::near(Point const& begin, Point const& ctrl, Point const& end, double const dist) const noexcept
    {
        return this->distance(begin, ctrl, end) <= dist;
    }
    
    bool Point::near(Point const& begin, Point const& ctrl1, Point const& ctrl2, Point const& end, double const dist) const noexcept
    {
        return this->distance(begin, ctrl1, ctrl2, end) <= dist;
    }
    
    ulong Point::solve(double a, double b, double c, double const d, double &solution1, double &solution2, double &solution3)
    {
        if(abs(a) > 0.)
        {
            double z = a;
            double a = b / z,  b = c / z; c = d / z;
            double p = b - a * a / 3.;
            double q = a * (2. * a * a - 9. * b) / 27. + c;
            double p3 = p * p * p;
            double D = q * q + 4. * p3 / 27.;
            double offset = -a / 3.;
            if(D == 0.)
            {
                double u;
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
                double u = ( -q + z) / 2.;
                double v = ( -q - z) / 2.;
                u = (u >= 0.) ? pow(u, 1. / 3.) : - pow( -u, 1. / 3.);
                v = (v >= 0.) ? pow(v, 1. / 3.) : - pow( -v, 1. / 3.);
                solution1 = u + v + offset;
                return 1;
            }
            else
            {
                double u = 2. * sqrt( -p / 3.);
                double v = acos(-sqrt( -27. / p3) * q / 2.) / 3.;
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
            
            double D = b*b - 4.*a*c;
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
    
    ulong Point::solve(array<Point, 6>& W, double *t, const ulong depth)
    {
        ulong count = 0;
        bool sign, old_sign;
        sign = old_sign = W[0].y() < 0. ? true : false;
        for(ulong i = 1; i < 6; i++)
        {
            sign = W[i].y() < 0. ? true : false;
            if(sign != old_sign)
            {
                count++;
            }
            old_sign = sign;
        }
        
        switch(count)
        {
            case 0 :
            {
                return 0;
                break;
            }
            case 1 :
            {
                if(depth >= 64)
                {
                    t[0] = (W[0].x() + W[5].x()) * 0.5;
                    return 1;
                }
                double 	distance[6];
                double a = W[0].y() - W[5].y(), b = W[5].x() - W[0].x(), c = W[0].x() * W[5].y() - W[5].x() * W[0].y();
                double abSquared = (a * a) + (b * b);
                for(ulong i = 1; i < 5; i++)
                {
                    distance[i] = a * W[i].x() + b * W[i].y() + c;
                    if (distance[i] > 0.0)
                    {
                        distance[i] = (distance[i] * distance[i]) / abSquared;
                    }
                    if (distance[i] < 0.0)
                    {
                        distance[i] = -((distance[i] * distance[i]) / abSquared);
                    }
                }
                
                double max_distance_above = 0., max_distance_below = 0.;
                for(ulong i = 1; i < 5; i++)
                {
                    if(distance[i] < 0.0)
                    {
                        max_distance_below = min(max_distance_below, distance[i]);
                    }
                    if(distance[i] > 0.0)
                    {
                        max_distance_above = max(max_distance_above, distance[i]);
                    }
                }
                
                double intercept_1 = (c + max_distance_above - b) / -a, intercept_2 = (c + max_distance_below - b ) / -a;
                if(0.5 * (max(intercept_1, intercept_2) - min(intercept_1, intercept_2)) < numeric_limits<double>::epsilon())
                {
                    const Point A = W[5] - W[0];
                    t[0] = (A.x() * W[0].y() - A.y() * W[0].y()) / -A.y();
                    return 1;
                }
                
                break;
            }
        }
        array<Point, 6> Left, Right;
        double 	left_t[6], right_t[6];
        
        Point Vtemp[6][6];
        for(ulong j = 0; j <= 5; j++)
        {
            Vtemp[0][j] = W[j];
        }
        for(ulong i = 1; i <= 5; i++)
        {
            for(ulong j =0 ; j <= 5 - i; j++)
            {
                Vtemp[i][j] = 0.5 * Vtemp[i-1][j] + 0.5 * Vtemp[i-1][j+1];
            }
        }
        
        for(ulong j = 0; j < 6; j++)
        {
            Left[j]  = Vtemp[j][0];
        }
        for(ulong j = 0; j < 6; j++)
        {
            Right[j] = Vtemp[6-j][j];
        }
        
        ulong left_count  = solve(Left, left_t, depth+1);
        ulong right_count = solve(Right, right_t, depth+1);
        
        for(ulong i = 0; i < left_count; i++)
        {
            t[i] = left_t[i];
        }
        for(ulong i = 0; i < right_count; i++)
        {
            t[i+left_count] = right_t[i];
        }
        
        return (left_count+right_count);
    }
}
