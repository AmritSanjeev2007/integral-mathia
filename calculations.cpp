#include <iostream>
#include <iomanip>
#include <math.h>

#include <thread>

using real = long double;
using function = real(real);

constexpr real integral(function f, real start, real end) noexcept(true)
{
#define _INTERVAL_COUNT_ 100000000 // Number of rectangles

    real sum=0;

    real width = (end-start)/_INTERVAL_COUNT_;

    for(real follow=start; abs(follow)<abs(end); follow+=width)
    {
        sum+=f(follow)*width;
    }

    return sum;
}

constexpr real max(real a, real b) { return (a+b+abs(a-b))/2; }
constexpr real sqr(real x) { return x*x; }
constexpr real solar_panel_in_ocean(real x)
{
    real f = 0.002;
    real w = 8;
    real k = 0.4;
    real A = 0.333;
    real r = 6371;
    real pi = 3.1415926535897932384626433832795029L;

    real sv=(f*(w-(k*w*sin(w*x))));
    
    return 
    (sv/sqrt(sqr(k*w*cos(w*x))+sqr(sv))) * 
        (max(cos(0.4091*cos((pi/4380)*x)+atan(tan(A)/cos((pi/12)*x))),0))
            /(2*r*sqrt(sqr(sin(A))+sqr(cos(A)*cos((pi/12)*x))))
    ;
}

constexpr real solar_panel_on_land(real x)
{
    real A = 0.333;
    real r = 6371;
    real pi = 3.1415926535897932384626433832795029L;
    
    return  (max(cos(0.4091*cos((pi/4380)*x)+atan(tan(A)/cos((pi/12)*x))),0))
                /(2*r*sqrt(sqr(sin(A))+sqr(cos(A)*cos((pi/12)*x))))
    ;
}

real results[4] = {0};
void exec_ocean(real start, real end, int i)
{
    results[i] = integral(solar_panel_in_ocean, start, end);
}
void exec_land(real start, real end, int i)
{
    results[i] = integral(solar_panel_on_land, start, end);
}

int main()
{
// Ocean
    std::thread t1(exec_ocean, 0, 1095, 0), t2(exec_ocean, 1096, 2191, 1), t3(exec_ocean, 2192, 3286, 2), t4(exec_ocean, 3287, 4380, 3);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    std::cout << "Solar panel in the ocean: " << std::fixed << std::setprecision(12) << results[0]+results[1]+results[2]+results[3] << std::endl;

// Land
    std::thread t5(exec_land, 0, 1095, 0), t6(exec_land, 1096, 2191, 1), t7(exec_land, 2192, 3286, 2), t8(exec_land, 3287, 4380, 3);

    t5.join();
    t6.join();
    t7.join();
    t8.join();

    std::cout << "Solar panel on land: " << std::fixed << std::setprecision(12) << results[0]+results[1]+results[2]+results[3] << std::endl;
}
