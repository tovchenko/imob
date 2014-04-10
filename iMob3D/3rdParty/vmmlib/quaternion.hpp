/*
* VMMLib - Vector & Matrix Math Lib
*
* @author Philip Schlegel
* @author Jonas Boesch
* @author Julius Natrup
* @additional functional by Taras Tovchenko
*/

#ifndef __VMML__QUATERNION__HPP__
#define __VMML__QUATERNION__HPP__

#include <vmmlib/vector.hpp>
#include <vmmlib/matrix.hpp>
#include <vmmlib/math.hpp>
#include <vmmlib/enable_if.hpp>

#include <vmmlib/exception.hpp>
#include <vmmlib/vmmlib_config.hpp>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <limits>


// - declaration - //

#define QUATERNION_TRACE_EPSILON 1e-5

namespace vmml
{

template < typename T >
class quaternion : private vector< 4, T >
{
public:
    typedef vector< 4, T >  super;

    using super::operator();
    using super::operator=;
    using super::at;
    using super::x;
    using super::y;
    using super::z;
    using super::w;
    using super::array;
    using super::find_min;
    using super::find_max;
    using super::find_min_index;
    using super::find_max_index;
    using super::iter_set;

	//constructors
	quaternion(); // warning: components NOT initialised (for performance)
	quaternion( T x, T y, T z, T w );
	quaternion( const vector< 3, T >& xyz , T w );
    // initializes the quaternion with xyz, sets w to zero
	quaternion( const vector< 3, T >& xyz );
	quaternion( T angle, const vector< 3, T >& rkAxis ); // from angle-axis
	quaternion( const vector< 3, T > akAxis[] ); // from axis
	quaternion( const vector< 3, T >& xaxis, const vector< 3, T >& yaxis, const vector< 3, T >& zaxis ); // from axis
    // uses the top-left 3x3 part of the supplied matrix as rotation matrix
    template< size_t M >
    quaternion( const matrix< M, M, T >& rotation_matrix_,
        typename enable_if< M >= 3 >::type* = 0 );
	
    void zero();
    void identity();

    template< size_t DIM >
    void set( const matrix< DIM, DIM, T >& rotation_matrix_ );
    
	void set( T ww, T xx, T yy, T zz);
    void set( vector< 3, T >& xyz, T w );

    template< typename input_iterator_t >
    void set( input_iterator_t begin_, input_iterator_t end_ );

	bool operator==( const T& a ) const;
	bool operator!=( const T& a ) const;
	
	bool operator==( const quaternion& a ) const;
	bool operator!=( const quaternion& a ) const;

	bool is_akin( const quaternion& a, 
				 const T& delta = std::numeric_limits< T >::epsilon() );
				 	
	void conjugate();
	quaternion get_conjugate() const;

	T abs() const;
	T squared_abs() const;
	
	T normalize();
	quaternion get_normalized() const;

	quaternion negate() const;
	quaternion operator-() const;

    //
	// quaternion/quaternion operations
	//
    quaternion operator+( const quaternion< T >& a ) const;
	quaternion operator-( const quaternion< T >& a ) const;
	// caution: a * q != q * a in general
	quaternion operator*( const quaternion< T >& a ) const;
	void operator+=( const quaternion< T >& a );
	void operator-=( const quaternion< T >& a );
	// caution: a *= q != q *= a in general
	void operator*=( const quaternion< T >& a );

    //
	// quaternion/scalar operations
    //
	quaternion operator*( T a ) const;
	quaternion operator/( T a ) const;
	
	void operator*=( T a );
	void operator/=( T a );
	
    //
	//quaternion/vector operations
	//
    quaternion operator+( const vector< 3, T >& a ) const;
	quaternion operator-( const vector< 3, T >& a ) const;
	quaternion operator*( const vector< 3, T >& a ) const;
	vector< 3, T > rotate_vector( const vector< 3, T >& a ) const;

	void operator+=( const vector< 3, T >& a );
	void operator-=( const vector< 3, T >& a );
	void operator*=( const vector< 3, T >& a );
	
    // vec3 = this x b
	vector< 3, T > cross( const quaternion< T >& b ) const;

	T dot( const quaternion< T >& a ) const;
	static T dot( const quaternion< T >& a, const quaternion< T >& b );
	
	// returns multiplicative inverse
	quaternion inverse() const;
	
	void normal( const quaternion& aa, const quaternion& bb, const quaternion& cc,  const quaternion& dd );
	quaternion normal( const quaternion& aa, const quaternion& bb, const quaternion& cc );
	
	// to combine two rotations, multiply the respective quaternions before using rotate 
	// instead of rotating twice for increased performance, but be aware of non-commutativity!
	void rotate( T theta, const vector< 3, T >& a );
	quaternion rotate( T theta, vector< 3, T >& axis, const vector< 3, T >& a );
	quaternion rotate_x( T theta, const vector< 3, T >& a );
	quaternion rotate_y( T theta, const vector< 3, T >& a );
	quaternion rotate_z( T theta, const vector< 3, T >& a );
	
	void from_angle_axis( T angle, const vector< 3, T >& rkAxis );
    void to_angle_axis( T& rfAngle, vector< 3, T >& rkAxis ) const;
	void from_axes( const vector< 3, T > akAxis[] );
    void from_axes( const vector< 3, T >& xaxis, const vector< 3, T >& yaxis, const vector< 3, T >& zaxis );
    void to_axes( vector< 3, T > akAxis[] ) const;
	void to_axes( vector< 3, T >& xaxis, vector< 3, T >& yaxis, vector< 3, T >& zaxis );
	void from_rotation_matrix( const matrix< 3, 3, T >& mtx );
	void to_rotation_matrix( matrix< 3, 3, T >& mtx ) const;

	vector< 3, T > xAxis() const;
    vector< 3, T > yAxis() const;
    vector< 3, T > zAxis() const;

	static quaternion slerp( T a, const quaternion< T >& p, const quaternion& q );
    static quaternion slerp_no_invert( T a, const quaternion< T >& p, const quaternion& q );
	
    matrix< 3, 3, T > get_rotation_matrix() const;

    template< size_t DIM >
    void get_rotation_matrix( matrix< DIM, DIM, T >& result ) const;
    
	friend std::ostream& operator<< ( std::ostream& os, const quaternion& q )
	{
        os << "(";
        size_t index = 0;
        for( ; index < 3; ++index )
        {
            os << q.at( index ) << ", ";
        }
        os << q.at( index ) << ") ";
        return os;
	};
    
	static const quaternion ZERO;
	static const quaternion IDENTITY;
	static const quaternion QUATERI;
	static const quaternion QUATERJ;
	static const quaternion QUATERK;

}; // class quaternion

#ifndef VMMLIB_NO_TYPEDEFS

typedef quaternion< float >  quaternionf;
typedef quaternion< double > quaterniond;

#endif

// - implementation - //

template < typename T >
const quaternion< T > quaternion< T >::ZERO( 0, 0, 0, 0 );

template < typename T >
const quaternion< T > quaternion< T >::IDENTITY( 0, 0, 0, 1 );

template < typename T >
const quaternion< T > quaternion< T >::QUATERI( 1, 0, 0, 0 );

template < typename T >
const quaternion< T > quaternion< T >::QUATERJ( 0, 1, 0, 0 );

template < typename T >
const quaternion< T > quaternion< T >::QUATERK( 0, 0, 1, 0 );


template < typename T >
quaternion< T >::quaternion()
{
    // intentionally left empty
}



template < typename T >
quaternion< T >::quaternion( T x_, T y_, T z_, T w_ )
{
    x() = x_;
    y() = y_;
    z() = z_;
    w() = w_;
}



template < typename T >
quaternion< T >::quaternion(
    const vector< 3, T >& xyz,
    T w_ 
    )
{
    super::set( xyz, w_ );
}




template < typename T >
quaternion< T >::quaternion( const vector< 3, T >& xyz )
{
    super::set( xyz, static_cast< T >( 0.0 ) );
}

	
	
template < typename T >
quaternion< T >::quaternion( T angle, const vector< 3, T >& rkAxis )
{
	from_angle_axis( angle, rkAxis );
}
	
	
	
template < typename T >
quaternion< T >::quaternion( const vector< 3, T > akAxis[] )
{
	from_axes( akAxis );
}
	
	
	
template < typename T >
quaternion< T >::quaternion( const vector< 3, T >& xaxis, const vector< 3, T >& yaxis, const vector< 3, T >& zaxis )
{
	from_axes( xaxis, yaxis, zaxis );
}
	
	

template< typename T >
template< size_t M >
quaternion< T >::quaternion( const matrix< M, M, T >& rotation_matrix_,
    typename enable_if< M >= 3 >::type* )
{
    this->template set< M >( rotation_matrix_ );
}



 // top-left 3x3 is interpreted as rot matrix.
template < typename T >
template< size_t DIM >
void
quaternion< T >::
set( const matrix< DIM, DIM, T >& M )
{
    T trace = M( 0, 0 ) + M( 1, 1 ) + M( 2,2 ) + 1.0;
    
    // very small traces may introduce a big numerical error
    if( trace > QUATERNION_TRACE_EPSILON )
    {
        T s = 0.5 / sqrt( trace );
        x() = M( 2, 1 ) - M( 1, 2 );
        x() *= s;

        y() = M( 0, 2 ) - M( 2, 0 );
        y() *= s;

        z() = M( 1, 0 ) - M( 0, 1 );
        z() *= s;

        w() = 0.25 / s;
    }
    else 
    {
        vector< 3, T > diag( M( 0, 0 ), M( 1, 1 ), M( 2, 2 ) );
        size_t largest = diag.find_max_index();
        
        // 0, 0 is largest
        if ( largest == 0 )
        {
            T s = 0.5 / sqrt( 1.0 + M( 0, 0 ) - M( 1, 1 ) - M( 2, 2 ) );
            x() = 0.25 / s;

            y() = M( 0,1 ) + M( 1,0 );
            y() *= s;

            z() = M( 0,2 ) + M( 2,0 );
            z() *= s;
            
            w() = M( 1,2 ) - M( 2,1 );
            w() *= s;
        }
        else if ( largest == 1 )
        {
            T s = 0.5 / sqrt( 1.0 + M( 1,1 ) - M( 0,0 ) - M( 2,2 ) );
            x() = M( 0,1 ) + M( 1,0 );
            x() *= s;

            y() = 0.25 / s;

            z() = M( 1,2 ) + M( 2,1 );
            z() *= s;

            w() = M( 0,2 ) - M( 2,0 );
            w() *= s;
        }
        // 2, 2 is largest
        else if ( largest == 2 )
        {
            T s = 0.5 / sqrt( 1.0 + M( 2,2 ) - M( 0,0 ) - M( 1,1 ) );
            x() = M( 0,2 ) + M( 2,0 );
            x() *= s;

            y() = M( 1,2 ) + M( 2,1 );
            y() *= s;

            z() = 0.25 / s;

            w() = M( 0,1 ) - M( 1,0 );
            w() *= s;
        }
        else
        {
            assert( 0 );
        }
    }
}



template < typename T >
void
quaternion< T >::zero()
{
    (*this) = ZERO;
}



template < typename T >
void
quaternion< T >::identity()
{
    (*this) = IDENTITY;
}



template < typename T >
void
quaternion< T >::set( T xx, T yy, T zz, T ww )
{
	x() = xx;
	y() = yy;
	z() = zz;
	w() = ww;
}



template< typename T >
void
quaternion< T >::set( vector< 3, T >& xyz, T _w )
{
    x() = xyz.x();
    y() = xyz.y();
    z() = xyz.z();
    w() = _w;
}



template < typename T >
template< typename input_iterator_t >
void
quaternion< T >::set( input_iterator_t begin_, input_iterator_t end_ )
{
    super::template set< input_iterator_t >( begin_, end_ );
}



template < typename T >
bool
quaternion< T >::operator==( const T& a ) const
{
	return ( w() == a && x() == 0 && y() == 0 && z() == 0 );
}



template < typename T >
bool quaternion< T >::operator!=( const T& a ) const
{
	return ( w() != a || x() != 0 || y() != 0 || z() != 0 );
}



template < typename T >
bool
quaternion< T >::operator==( const quaternion& a ) const
{
	return ( 
        w() == a.w() && 
        x() == a.x() &&
        y() == a.y() &&
        z() == a.z()
        );
}



template < typename T >
bool
quaternion< T >::operator!=( const quaternion& a ) const
{
	return ! this->operator==( a );
}



template < typename T >
bool
quaternion< T >::is_akin( const quaternion& a, const T& delta )
{
	if( fabsf( w() - a.w() ) > delta || 
        fabsf( x() - a.x() ) > delta || 
		fabsf( y() - a.y() ) > delta ||
        fabsf( z() - a.z() ) > delta
        )
        return false;
	return true;
}



template < typename T >
void quaternion< T >::conjugate() 
{
    x() = -x();
    y() = -y();
    z() = -z();
}



template < typename T >
quaternion< T > quaternion< T >::get_conjugate() const
{
	return quaternion< T > ( -x(), -y(), -z(), w() );
}



template < typename T >
T
quaternion< T >::abs() const
{
	return sqrt( squared_abs() );
}



template < typename T >
T quaternion< T >::squared_abs() const
{
	return x() * x() + y() * y() + z() * z() + w() * w();
}



template < typename T >
quaternion< T > quaternion< T >::inverse() const
{
	quaternion< T > q( *this );
    q.conjugate();
    
	T tmp = squared_abs();
	tmp = static_cast< T >( 1.0 ) / tmp;
	return q * tmp;
}



template < typename T >
T quaternion< T >::normalize()
{
	T length = abs();
	if( length == 0.0 )
		return 0.0;
	length = 1.0f / length;
    this->operator*=( length );
	return length;
}



template < typename T >
quaternion< T >
quaternion< T >::get_normalized() const
{
    quaternion< T > q( *this );
    q.normalize();
	return q;
}



//
// quaternion/quaternion operations
//

template < typename T >
quaternion< T >
quaternion< T >::operator+( const quaternion< T >& a ) const
{
	return quaternion( x() + a.x(), y() + a.y(), z() + a.z(), w() + a.w() );
}



template < typename T >
quaternion< T >
quaternion< T >::operator-( const quaternion< T >& a ) const
{
	return quaternion( x() - a.x(), y() - a.y(), z() - a.z(), w() - a.w() );
}



// returns Grasssmann product
template < typename T >
quaternion< T >
quaternion< T >::operator*( const quaternion< T >& a ) const
{
    quaternion< T > ret( *this );
    ret *= a;
    return ret;
}

	
	
template < typename T >
vector< 3, T >
quaternion< T >::rotate_vector( const vector< 3, T >& a ) const
{
	vector< 3, T > uv, uuv;
	vector< 3, T > qvec( x(), y(), z() );
	uv = qvec.cross( a );
	uuv = qvec.cross( uv );
	uv *= ( 2.0 * w() );
	uuv *= 2.0;
	
	return a + uv + uuv;	
}


	
// Grassmann product
template < typename T >
void
quaternion< T >::operator*=( const quaternion< T >& q )
{
    #if 0
    quaternion< T > orig( *this );
	x() = orig.w() * a.x() + orig.x() * a.w() + orig.y() * a.z() - orig.z() * a.y();
	y() = orig.w() * a.y() + orig.y() * a.w() + orig.z() * a.x() - orig.x() * a.z();
	z() = orig.w() * a.z() + orig.z() * a.w() + orig.x() * a.y() - orig.y() * a.x();
	w() = orig.w() * a.w() - orig.x() * a.x() - orig.y() * a.y() - orig.z() * a.z();
    #else

    // optimized version, 7 less mul, but 15 more add/subs
    // after Henrik Engstrom, from a gamedev.net article.

    T* _array = super::array;

    const T& a = _array[ 3 ];
    const T& b = _array[ 0 ];
    const T& c = _array[ 1 ];
    const T& d = _array[ 2 ];
    const T& _x = q.array[ 3 ];
    const T& _y = q.array[ 0 ];
    const T& _z = q.array[ 1 ];
    const T& _w = q.array[ 2 ];

    const T tmp_00 = (d - c) * (_z - _w);
    const T tmp_01 = (a + b) * (_x + _y);
    const T tmp_02 = (a - b) * (_z + _w);
    const T tmp_03 = (c + d) * (_x - _y);
    const T tmp_04 = (d - b) * (_y - _z);
    const T tmp_05 = (d + b) * (_y + _z);
    const T tmp_06 = (a + c) * (_x - _w);
    const T tmp_07 = (a - c) * (_x + _w);
    const T tmp_08 = tmp_05 + tmp_06 + tmp_07;
    const T tmp_09 = 0.5 * (tmp_04 + tmp_08);
    
    _array[ 3 ] = tmp_00 + tmp_09 - tmp_05;
    _array[ 0 ] = tmp_01 + tmp_09 - tmp_08;
    _array[ 1 ] = tmp_02 + tmp_09 - tmp_07;
    _array[ 2 ] = tmp_03 + tmp_09 - tmp_06;   
    
    #endif
}





template < typename T >
quaternion< T > 
quaternion< T >::operator-() const
{
	return quaternion( -x(), -y(), -z(), -w() );
}



template < typename T >
void
quaternion< T >::operator+=( const quaternion< T >& a )
{
    array[ 0 ] += a.array[ 0 ];
    array[ 1 ] += a.array[ 1 ];
    array[ 2 ] += a.array[ 2 ];
    array[ 3 ] += a.array[ 3 ];
}	



template < typename T >
void
quaternion< T >::operator-=( const quaternion< T >& a )
{
    array[ 0 ] -= a.array[ 0 ];
    array[ 1 ] -= a.array[ 1 ];
    array[ 2 ] -= a.array[ 2 ];
    array[ 3 ] -= a.array[ 3 ];
}



//
// quaternion/scalar operations		
//

template < typename T >
quaternion< T >
quaternion< T >::operator*( const T a ) const
{
	return quaternion( x() * a, y() * a, z() * a, w() * a );
}


	
template < typename T >
quaternion< T >
quaternion< T >::operator/( T a ) const
{  
    if ( a == 0.0 )
    {
        VMMLIB_ERROR( "Division by zero.", VMMLIB_HERE );
    }
	a = 1.0 / a;
	return quaternion( x() * a, y() * a, z() * a, w() * a );
}



template < typename T >
void
quaternion< T >::operator*=( T a ) 
{
	array[ 0 ] *= a;
	array[ 1 ] *= a;
	array[ 2 ] *= a;
	array[ 3 ] *= a;
}	
	


template < typename T >
void
quaternion< T >::operator/=( T a ) 
{
    if ( a == 0.0 )
    {
        VMMLIB_ERROR( "Division by zero", VMMLIB_HERE );
    }
	a = 1.0f / a;
    this->operator*=( a );
}


//quaternion/vector operations

template < typename T >
quaternion< T >
quaternion< T >::operator+( const vector< 3, T >& a ) const
{
	return quaternion( x() + a.x(), y() + a.y(), z() + a.z(), w() );
}



template < typename T >
quaternion< T >
quaternion< T >::operator-( const vector< 3, T >& a ) const
{
	return quaternion( w(), x() - a.x(), y() - a.y(), z() - a.z() );
}



template < typename T >
quaternion< T >
quaternion< T >::operator*( const vector< 3, T >& a ) const
{
	return quaternion( -x() * a.x() - y() * a.y() - z() * a.z(),
	 					w() * a.x() + y() * a.z() - z() * a.y(), 
						w() * a.y() + z() * a.x() - x() * a.z(),
						w() * a.z() + x() * a.y() - y() * a.x()  );
}



template < typename T >
void
quaternion< T >::operator+=( const vector< 3, T >& xyz )
{
	x() += xyz.x();
	y() += xyz.y();
	y() += xyz.z();
}



template < typename T >
void
quaternion< T >::operator-=( const vector< 3, T >& xyz )
{
	x() -= xyz.x();
	y() -= xyz.y();
	z() -= xyz.z();
	return *this;
}



template < typename T >
void
quaternion< T >::operator*=(const vector< 3, T >& a )
{
    T _x = x();
    T _y = y();
    T _z = z();
    T _w = w();
    
	x() = _w * a.x() + _y * a.z() - _z * a.y();
	y() = _w * a.y() + _z * a.x() - _x * a.z();
	z() = _w * a.z() + _x * a.y() - _y * a.x();
    w() = -_x * a.x() - _y * a.y() - _z * a.z();
}
	



template < typename T >
vector< 3, T > quaternion< T >::cross( const quaternion< T >& bb ) const
{
	vector< 3, T > result;

    result.array[ 0 ] = y() * bb.z() - z() * bb.y(); 
    result.array[ 1 ] = z() * bb.x() - x() * bb.z(); 
    result.array[ 2 ] = x() * bb.y() - y() * bb.x(); 

    return result;
}



template < typename T >
T quaternion< T >::dot( const quaternion< T >& a ) const
{
	return w() * a.w() + x() * a.x() + y() * a.y() + z() * a.z();
}



template < typename T >
T quaternion< T >::
dot( const quaternion< T >& a, const quaternion< T >& b )
{
	return a.w() * b.w() + a.x() * b.x() + a.y() * b.y() + a.z() * b.z();
}



template < typename T >
void quaternion< T >::normal( const quaternion< T >& aa,
							  const quaternion< T >& bb,
							  const quaternion< T >& cc,
							  const quaternion< T >& dd )
{
	//right hand system, CCW triangle
	const quaternion< T > quat_t = bb - aa;
	const quaternion< T > quat_u = cc - aa;
	const quaternion< T > quat_v = dd - aa;
	cross( quat_t, quat_u, quat_v );
	normalize();
}



template < typename T >
quaternion< T > quaternion< T >::normal( const quaternion< T >& aa,
										 const quaternion< T >& bb,
										 const quaternion< T >& cc )
{
	quaternion< T > tmp;
	tmp.normal( *this, aa, bb, cc );
	return tmp;
}


// to combine two rotations, multiply the respective quaternions before using rotate 
// instead of rotating twice for increased performance, but be aware of non-commutativity!
// (the first rotation quaternion has to be the first factor)
template< typename T >
quaternion< T >
quaternion< T >::rotate( T theta, vector< 3, T >& axis, const vector< 3, T >& a )
{
	quaternion< T > p = a;
	T alpha = theta / 2;
	quaternion< T > q( sin( alpha ) * axis.normalize(), cos( alpha ) );
	return q * p * q.inverse();
}



template< typename T >
quaternion< T > quaternion< T >::rotate_x( T theta, const vector< 3, T >& a )
{
	quaternion< T > p = a;
	T alpha = theta / 2;
	quaternion< T > q( QUATERI * sin( alpha ), cos( alpha ) );
	return q * p * q.inverse();
}



template< typename T >
quaternion< T > quaternion< T >::rotate_y( T theta, const vector< 3, T >& a )
{
	quaternion< T > p = a;
	T alpha = theta / 2;
	quaternion< T > q( QUATERJ * sin( alpha ), cos( alpha ) );
	return q * p * q.inverse();
}



template< typename T >
quaternion< T > quaternion< T >::rotate_z( T theta, const vector< 3, T >& a )
{
	quaternion< T > p = a;
	T alpha = theta / 2;
	quaternion< T > q( QUATERK * sin( alpha ), cos( alpha ) );
	return q * p * q.inverse();
}
	
	

template< typename T >
void
quaternion< T >::from_angle_axis( T angle, const vector< 3, T >& rkAxis )
{
	// assert:  axis[] is unit length
	//
	// The quaternion representing the rotation is
	//   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)
	
	T fHalfAngle = 0.5 * angle;
	T fSin = sin( fHalfAngle );
	vector< 3, T > xyz = rkAxis * fSin;
	set( xyz, cos( fHalfAngle ) );
}
	
	

template< typename T >
void
quaternion< T >::to_angle_axis( T& rfAngle, vector< 3, T >& rkAxis ) const
{
	// The quaternion representing the rotation is
	//   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)
	
	T fSqrLength = x() * x() + y() * y() + z() * z();
	if ( fSqrLength > 0.0 )
	{
		rfAngle = 2.0 * acos(w());
		T fInvLength = 1.0 / sqrt( fSqrLength );
		rkAxis.x() = x() * fInvLength;
		rkAxis.y() = y() * fInvLength;
		rkAxis.z() = z() * fInvLength;
	}
	else
	{
		// angle is 0 (mod 2*pi), so any axis will do
		rfAngle = 0.0;
		rkAxis.x() = 1.0;
		rkAxis.y() = 0.0;
		rkAxis.z() = 0.0;
	}
}

	
	
template < typename T >
void
quaternion< T >::from_axes( const vector< 3, T > akAxis[] )
{
	matrix< 3, 3, T > rot;
	
	for (size_t iCol = 0; iCol < 3; iCol++)
	{
		rot( 0, iCol ) = akAxis[ iCol ].x();
		rot( 1, iCol ) = akAxis[ iCol ].y();
		rot( 2, iCol ) = akAxis[ iCol ].z();
	}
	
	set( rot );
}

	
	
template < typename T >
void
quaternion< T >::from_axes( const vector< 3, T >& xaxis, const vector< 3, T >& yaxis, const vector< 3, T >& zaxis )
{
	matrix< 3, 3, T > rot;
	
	rot( 0, 0 ) = xaxis.x();
	rot( 1, 0 ) = xaxis.y();
	rot( 2, 0 ) = xaxis.z();
	
	rot( 0, 1 ) = yaxis.x();
	rot( 1, 1 ) = yaxis.y();
	rot( 2, 1 ) = yaxis.z();
	
	rot( 0, 2 ) = zaxis.x();
	rot( 1, 2 ) = zaxis.y();
	rot( 2, 2 ) = zaxis.z();
	
	set( rot );	
}

	
	
template < typename T >
void
quaternion< T >::to_axes( vector< 3, T > akAxis[] ) const
{
	matrix< 3, 3, T > rot;
	
	get_rotation_matrix< 3 >( rot );
	
	for (size_t iCol = 0; iCol < 3; iCol++)
	{
		akAxis[ iCol ].x() = rot( 0, iCol );
		akAxis[ iCol ].y() = rot( 1, iCol );
		akAxis[ iCol ].z() = rot( 2, iCol );
	}
}

	
	
template < typename T >
void
quaternion< T >::to_axes( vector< 3, T >& xaxis, vector< 3, T >& yaxis, vector< 3, T >& zaxis )
{
	matrix< 3, 3, T > rot;
	
	get_rotation_matrix< 3 >( rot );
	
	xaxis.x() = rot( 0, 0 );
	xaxis.y() = rot( 1, 0 );
	xaxis.z() = rot( 2, 0 );
	
	yaxis.x() = rot( 0, 1 );
	yaxis.y() = rot( 1, 1 );
	yaxis.z() = rot( 2, 1 );
	
	zaxis.x() = rot( 0, 2 );
	zaxis.y() = rot( 1, 2 );
	zaxis.z() = rot( 2, 2 );
}
	
	
	
template < typename T >
void
quaternion< T >::from_rotation_matrix( const matrix< 3, 3, T >& mtx )
{
	set ( mtx );
}

	
	
template < typename T >
void
quaternion< T >::to_rotation_matrix( matrix< 3, 3, T >& mtx ) const
{
	get_rotation_matrix< 3 >( mtx );
}
	
	
	
template < typename T >
vector< 3, T >
quaternion< T >::xAxis() const
{
	T fTy  = 2.0 * y();
	T fTz  = 2.0 * z();
	T fTwy = fTy * w();
	T fTwz = fTz * w();
	T fTxy = fTy * x();
	T fTxz = fTz * x();
	T fTyy = fTy * y();
	T fTzz = fTz * z();
	
	return vector< 3, T >( 1.0 - ( fTyy + fTzz ), fTxy + fTwz, fTxz - fTwy );
}
	
	
	
template < typename T >
vector< 3, T >
quaternion< T >::yAxis() const
{
	T fTx  = 2.0 * x();
	T fTy  = 2.0 * y();
	T fTz  = 2.0 * z();
	T fTwx = fTx * w();
	T fTwz = fTz * w();
	T fTxx = fTx * x();
	T fTxy = fTy * x();
	T fTyz = fTz * y();
	T fTzz = fTz * z();
	
	return vector< 3, T >( fTxy - fTwz, 1.0 - ( fTxx + fTzz ), fTyz + fTwx );
}

	
	
template < typename T >
vector< 3, T >
quaternion< T >::zAxis() const
{
	T fTx  = 2.0 * x();
	T fTy  = 2.0 * y();
	T fTz  = 2.0 * z();
	T fTwx = fTx * w();
	T fTwy = fTy * w();
	T fTxx = fTx * x();
	T fTxz = fTz * x();
	T fTyy = fTy * y();
	T fTyz = fTz * y();
	
	return vector< 3, T >( fTxz + fTwy, fTyz - fTwx, 1.0 - ( fTxx + fTyy ) );	
}

	
	
template < typename T >
matrix< 3, 3, T >
quaternion< T >::get_rotation_matrix() const
{
    matrix< 3, 3, T > result;
    get_rotation_matrix< 3 >( result );
    return result;
}



template < typename T >
template< size_t DIM >
void
quaternion< T >::get_rotation_matrix( matrix< DIM, DIM, T >& M ) const
{
    T w2 = w() * w();
    T x2 = x() * x();
    T y2 = y() * y();
    T z2 = z() * z();
    T wx = w() * x();
    T wy = w() * y();
    T wz = w() * z();
    T xy = x() * y();
    T xz = x() * z();
    T yz = y() * z();

    M( 0, 0 ) = w2 + x2 - y2 - z2;
    M( 0, 1 ) = 2. * (xy - wz);
    M( 0, 2 ) = 2. * (xz + wy);
    M( 1, 0 ) = 2. * (xy + wz);
    M( 1, 1 ) = w2 - x2 + y2 - z2;
    M( 1, 2 ) = 2. * (yz - wx);
    M( 2, 0 ) = 2. * (xz - wy);
    M( 2, 1 ) = 2. * (yz + wx);
    M( 2, 2 ) = w2 - x2 - y2 + z2;

}

template < typename T >
quaternion< T > quaternion< T >::
slerp( T a, const quaternion< T >& p, const quaternion< T >& q )
{
	quaternion< T > p1 = p.get_normalized();
	quaternion< T > q1 = q.get_normalized();
	T cosine = p1.dot(q1);
	quaternion< T > quat_t;
	
	// check if inverted rotation is needed
	if ( cosine < 0.0 )
	{
		cosine = -cosine;
		quat_t = -q1;
	}
	else
	{
		quat_t = q1;
	}
	
	if( fabs(cosine) < 1.0 - 1e-13 )
	{
		// standard slerp
        T angle = acos( cosine );
        return ( p1 * sin( angle * ( 1.0 - a ) ) + quat_t * sin( angle * a) ) / sin( angle );
	}
	else
	{
		// linear interpolation for very small angles  
		quaternion< T > quat_u = p1 * ( 1.0 - a ) + quat_t * a;
		quat_u.normalize();
		return quat_u;
	}
}
    
template < typename T >
quaternion< T > quaternion< T >::
slerp_no_invert( T a, const quaternion< T >& p, const quaternion< T >& q )
{
    quaternion< T > p1 = p.get_normalized();
    quaternion< T > q1 = q.get_normalized();
    T cosine = p1.dot(q1);
    
    const T delta = 1.0 - 1e-13;
    
    if( cosine > -delta && cosine < delta )
    {
        // standard slerp
        T angle = acos( cosine );
        return ( p1 * sin( angle * ( 1.0 - a ) ) + q1 * sin( angle * a) ) / sin( angle );
    }
    else
    {
        // linear interpolation for very small angles  
        quaternion< T > quat_u = p1 * ( 1.0 - a ) + q1 * a;
        quat_u.normalize();
        return quat_u;
    }
}

}
#endif
