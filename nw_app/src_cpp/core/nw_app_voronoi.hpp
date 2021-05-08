#include "nw_app_pch.hpp"
/////////////////////////////////////////////////////////////////
// 
//          Copyright Vadim Stadnik 2015.
// Distributed under the Code Project Open License (CPOL) 1.02. 
// (See or copy at http://www.codeproject.com/info/cpol10.aspx) 
//
/////////////////////////////////////////////////////////////////


//  
//  the namespace VoronoiDemo provides a demonstration variant of 
//  the nearest neighbor search in an ordered set of two dimensional points; 
//  the algorithm has square root computational complexity, on average; 
//  
//  the algorithm is parameterized on types of containers, 
//  iterators and supporting algorithms; a user algorithm 
//  can take advantage of the interchangeable C++ standard 
//  containers and algorithms;
//
//  the performance test emulates the computation of 
//  the distance transform; the performance of the developed 
//  algorithm can be compared with the performance of 
//  the brute force algorithm; 
//  
//  the code of the namespace VoronoiDemo has been written  
//  to avoid complications associated with numeric errors of 
//  floating data types in comparison operations; 
//  the code uses integer type for X and Y coordinates of 
//  two dimensional points; in addition to this, instead of 
//  distance between two points, it calculates squared distance, 
//  which also takes advantage of the exact integer value; 
//  
namespace VoronoiDemo
{

    //  timer for performance measurements 
    class TimerChrono
    {
        typedef std::chrono::high_resolution_clock     _clock_impl;
        typedef _clock_impl::time_point                _time_point;
        typedef std::ratio< 1, 1000000 >               _micro;
        typedef std::chrono::duration<double, _micro>  _duration;

    public:

        TimerChrono() : start(), stop() {  }

        void Start() { start = _clock_impl::now(); }
        void Stop() { stop = _clock_impl::now(); }

        double DurationMicroSec() const
        {
            _duration   durn_mcsec = stop - start;
            double      time = durn_mcsec.count();
            return time;
        }

    private:

        _time_point     start;
        _time_point     stop;
    };


    void PrintTestResult(TimerChrono const& timer, std::string const& info)
    {
        double              time = timer.DurationMicroSec();
        std::ostringstream  ostr_out;
        ostr_out << " T = " << time << " mcs ; " << info << std::endl;
        std::cout << ostr_out.str();
    }


    //  class Point represents a two dimensional point 
    class Point
    {
    public:
        explicit
            Point(int _x = 0, int _y = 0) : x(_x), y(_y) {  }

        int	 X() const { return x; }
        int	 Y() const { return y; }

        bool operator <  (const Point& that) const
        {
            if (x < that.x)
                return true;
            else if (that.x < x)
                return false;
            else
                return (y < that.y);
        }

        bool operator == (const Point& that) const
        {
            return (x == that.x) && (y == that.y);
        }

        bool operator != (const Point& that) const
        {
            return !(*this == that);
        }

    protected:
        int     x;
        int     y;
    };


    //  several types of containers to store two dimensional points
    typedef std::vector<Point>      VectorPoints;
    typedef std::deque<Point>       DequePoints;
    typedef std::list<Point>        ListPoints;
    typedef std::set<Point>         SetPoints;


    //  type for non-negative values of DistanceSquared()  
    typedef unsigned int    uint;

    uint DistanceSquared(Point const& pnt_a, Point const& pnt_b)
    {
        int     x = pnt_b.X() - pnt_a.X();
        int     y = pnt_b.Y() - pnt_a.Y();
        uint    d = static_cast<uint>(x * x + y * y);

        return d;
    }


    //  the function MinDistanceBruteForce() 
    //  implements the brute force sequential search algorithm; 
    //  it can be used for any container that provides forward iterators, 
    //  a container can be either ordered or unordered; 
    //    
    //  computational complexity - O(N),  
    //  where N is the number of points in a container; 
    //
    template <typename _Container>
    uint MinDistanceBruteForce(Point const& pnt, _Container const& points)
    {
        typedef typename _Container::const_iterator  CtIterator;

        uint        dist_min = UINT_MAX;
        uint        dist_cur = 0;
        CtIterator  iter = points.begin();
        CtIterator  end = points.end();

        for (; iter != end; ++iter)
        {
            dist_cur = DistanceSquared(pnt, *iter);

            if (dist_cur < dist_min)
                dist_min = dist_cur;
        }

        return dist_min;
    }


    //  struct LowerBoundMemberFunction is a 
    //  function object for STL containers, such as std::set<T>, 
    //  that support member functions lower_bound(); 
    template < typename _OrderedSet >
    struct LowerBoundMemberFunction
    {
        typedef typename _OrderedSet::const_iterator  CtIterator;

        LowerBoundMemberFunction() { };

        CtIterator
            operator ( )
            (
                _OrderedSet const& set_points,
                Point const& pnt
                ) const
        {
            CtIterator  iter_res = set_points.lower_bound(pnt);
            return iter_res;
        }
    };


    //  struct LowerBoundSTDAlgorithm is a 
    //  function object for STL sequence containers 
    //  that store ordered elements; 
    //  for the best performance requires a container 
    //  with random access iterators;
    template < typename _OrderedContainer >
    struct LowerBoundSTDAlgorithm
    {
        typedef typename _OrderedContainer::const_iterator  CtIterator;

        LowerBoundSTDAlgorithm() { };

        CtIterator
            operator ( )
            (
                _OrderedContainer const& ordered_points,
                Point const& pnt
                ) const
        {
            CtIterator  iter_begin = ordered_points.begin();
            CtIterator  iter_end = ordered_points.end();
            CtIterator  iter_res = std::lower_bound(iter_begin, iter_end, pnt);
            return iter_res;
        }
    };


    //  the function FindForward() is a helper 
    //  for the function MinDistanceOrderedSet() 
    template < typename _CtIterator >
    void FindForward
    (
        Point const& pnt,
        _CtIterator     it_cur,
        _CtIterator     it_end,
        uint& dist_min
    )
    {
        uint        dist_cur = 0;
        uint        dist_x = 0;

        while (it_cur != it_end)
        {
            dist_cur = DistanceSquared(*it_cur, pnt);
            dist_x = (it_cur->X() - pnt.X()) * (it_cur->X() - pnt.X());

            if (dist_cur < dist_min)
                dist_min = dist_cur;

            if (dist_x > dist_min)
                break;

            ++it_cur;
        }
    }


    //  the function FindBackward() is a helper 
    //  for the function MinDistanceOrderedSet(), 
    //  generally, it is NOT safe if container is empty;
    template < typename _CtIterator >
    void FindBackward
    (
        Point const& pnt,
        _CtIterator     it_cur,
        _CtIterator     it_begin,
        uint& dist_min
    )
    {
        uint        dist_cur = 0;
        uint        dist_x = 0;

        do
        {
            //  it is safe if input ( it_cur == container.end() )  
            //  and container is NOT empty 
            --it_cur;

            dist_cur = DistanceSquared(*it_cur, pnt);
            dist_x = (it_cur->X() - pnt.X()) * (it_cur->X() - pnt.X());

            if (dist_cur < dist_min)
                dist_min = dist_cur;

            if (dist_x > dist_min)
                break;
        } while (it_cur != it_begin);
    }


    //  the function MinDistanceOrderedSet() implements  
    //  the nearest neighbor search in an ordered set of points, 
    //  its average computational complexity - O ( sqrt(N) ) ,
    //  where N is the number of points in the set; 
    //  
    //  the template parameter <_OrderedSet> represents either 
    //  an ordered set or an ordered sequence of two-dimensional points; 
    //  both std::vector<T> and std::set<T> can be used as template arguments; 
    //  
    //  the template parameter <_FuncLowBound> represents an algorithm 
    //  that finds for an input point its lower bound in 
    //  either an ordered set or an ordered sequence of two dimensional points; 
    //  the namespace VoronoiDemo provides two function objects of this algorithm: 
    //  LowerBoundMemberFunction and LowerBoundSTDAlgorithm; 
    //  to achieve the specified computational complexity 
    //  an object of LowerBoundSTDAlgorithm should be used 
    //  with a container that supports random access iterators; 
    //
    //  for examples how to use the function MinDistanceOrderedSet(), 
    //  see the code below in this file; 
    //  
    template < typename _OrderedSet, typename _FuncLowBound >
    uint  MinDistanceOrderedSet
    (
        _OrderedSet const& set_points,
        _FuncLowBound           find_LB,
        Point const& pnt
    )
    {
        typedef typename _OrderedSet::const_iterator  CtIterator;

        uint        dist_min = UINT_MAX;
        CtIterator  iter_begin = set_points.begin();
        CtIterator  iter_end = set_points.end();
        //  call lower boundary algorithm through a function object
        CtIterator  iter_forw = find_LB(set_points, pnt);
        CtIterator  iter_back = iter_forw;

        bool        move_forward = (iter_forw != iter_end);
        bool        move_backward = (iter_back != iter_begin);

        if (move_forward)
            FindForward(pnt, iter_forw, iter_end, dist_min);
        if (move_backward)
            FindBackward(pnt, iter_back, iter_begin, dist_min);

        return dist_min;
    }


    //  the function TestOrderedSet() tests the efficiency of 
    //  the nearest neighbor search in an ordered set of points;
    //
    //  this test emulates the computation of the distance transform; 
    //  it calculates the minimum distance from each point in 
    //  the given rectangle to a point in the input set; 
    //
    template < typename _OrderedSet, typename _FuncLowBound >
    uint TestOrderedSet
    (
        const int               rect_width,
        const int               rect_height,
        VectorPoints const& test_points
    )
    {
        //  this variable is used to avoid the effect of compiler optimization
        uint            dist_res = 0;
        _OrderedSet     set_points(test_points.begin(), test_points.end());
        _FuncLowBound   func_lower_bound;

        TimerChrono     timer;
        timer.Start();

        for (int x = 0; x < rect_width; ++x)
        {
            for (int y = 0; y < rect_height; ++y)
            {
                Point   point(x, y);
                uint    dist_min = MinDistanceOrderedSet(set_points, func_lower_bound, point);

                dist_res += dist_min;
            }
        }

        timer.Stop();
        PrintTestResult(timer, "ordered set algorithm;");

        return dist_res;
    }


    //  the TestBruteForce() function tests the efficiency of 
    //  the brute force algorithm 
    template <typename _Container>
    uint TestBruteForce (const int rect_width, const int rect_height, VectorPoints const& test_points)
    {
        //  this variable is used to avoid the effect of compiler optimization
        uint            dist_res = 0;
        _Container      points(test_points.begin(), test_points.end());

        TimerChrono     timer;
        timer.Start();

        for (int x = 0; x < rect_width; ++x)
        {
            for (int y = 0; y < rect_height; ++y)
            {
                Point   point(x, y);
                uint    dist_min = MinDistanceBruteForce(point, points);

                dist_res += dist_min;
            }
        }

        timer.Stop();
        PrintTestResult(timer, "brute force algorithm;");

        return dist_res;
    }


    //  the function random_value() generates 
    //  a random value in the range [ 0 , val_max ) ; 
    //  it assumes ( val_max > 1 ) ;
    int random_value(int  val_max)
    {
        double  rnd = double(rand()) / double(RAND_MAX);
        int     res = int(0.5 + rnd * (val_max - 1));
        return  res;
    }


    //  the function GenerateTestPoints() 
    //  generates random points within the specified 
    //  rectangle: [ 0 , width ) X [ 0 , height ) ; 
    //  the result is sorted and contains no duplicates,
    //  note that  points_res.size() <= n_points  ;
    void GenerateTestPoints
    (
        //  the width and height of a rectangle 
        //  that contains test points 
        const int       rect_width,
        const int       rect_height,
        const int       n_points,
        VectorPoints& points_res
    )
    {
        points_res.clear();
        points_res.reserve(n_points);

        //  generate random points 
        for (int i = 0; i < n_points; ++i)
        {
            int     x = random_value(rect_width);
            int     y = random_value(rect_height);
            points_res.push_back(Point(x, y));
        }

        std::sort(points_res.begin(), points_res.end());

        //  remove duplicates 
        VectorPoints::iterator  it_new_end;
        it_new_end = std::unique(points_res.begin(), points_res.end());
        points_res.erase(it_new_end, points_res.end());

        std::cout << "the number of unique test points N = " << points_res.size() << std::endl;
    }

}; //  namespace VoronoiDemo 


int main()
{
    using namespace VoronoiDemo;

    //  test performance of the nearest neighbor search algorithm 
    //  for various standard containers and algorithms; 

    //  use one data set to test all containers; 
    //  the number of generated unique test points <= n_points , 
    //  it depends on width and height of the test rectangle;
    const int       n_points = 1000;
    const int       width = 1000;
    const int       height = 1000;

    VectorPoints    test_points;
    GenerateTestPoints(width, height, n_points, test_points);

    //  std::vector<T> normally shows the best performance in this test
    TestBruteForce< VectorPoints >(width, height, test_points);
    TestOrderedSet< VectorPoints, LowerBoundSTDAlgorithm<VectorPoints > >(width, height, test_points);

    //  TestBruteForce< DequePoints > ( width , height , test_points ) ; 
    //  TestOrderedSet< DequePoints , LowerBoundSTDAlgorithm<DequePoints > > ( width , height , test_points ) ; 


        //  std::set<T> is less efficient than std::vector<T> in this test, 
        //  it offers the advantage of better performance in incremental algorithms,  
        //  but it does not support random access iterators;
    //  TestOrderedSet< SetPoints , LowerBoundMemberFunction<SetPoints > > ( width , height , test_points ) ; 


        //  examples of INEFFICIENT (!) algorithms using 
        //  bidirectional iterators instead of random access iterators;
    //  TestOrderedSet< SetPoints  , LowerBoundSTDAlgorithm<SetPoints  > > ( width , height , test_points ) ; 
    //  TestOrderedSet< ListPoints , LowerBoundSTDAlgorithm<ListPoints > > ( width , height , test_points ) ; 


        //  examples of containers that do not support member function lower_bound(), 
        //  thus, this code does not compile;
    //  TestOrderedSet< VectorPoints , LowerBoundMemberFunction<VectorPoints > > ( width , height , test_points ) ; 
    //  TestOrderedSet< DequePoints  , LowerBoundMemberFunction<DequePoints  > > ( width , height , test_points ) ; 

    return 0;
}