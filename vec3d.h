#ifndef __VEC3D_H
#define __VEC3D_H

#include <iostream>
#include <ostream>
#include <cmath>

using namespace std; 

class particle; 
class simulation; 
class Node;

class vec3D
{
        // for easy access we can refer to the elements
        // either using x,y,z or as an array 
        union {
                double v[3];
				struct {double x, y, z;};
        };

        // sets the variables
        // this function is private and it is for internal use only
        // the = after the variables assigns those variables to that value
        // if no variables are passed - see below for an example 
        void set(const double nx = 0.0, const double ny = 0.0, const double nz = 0.0) {x = nx; y = ny; z = nz;}

		
public:
        /* constructors */
        vec3D() {set(); }
        vec3D(const double nx, const double ny, const double nz) {set(nx, ny, nz);}
		vec3D(const double all) {set(all, all, all);}
		
		double magnitude() const {return sqrt(magnitudeSquared());};

		double inftynorm() const
		{
			double max = fabs(x);
			if (fabs(y) > max) {max = fabs(y);}
			if (fabs(z) > max) {max = fabs(z);}
			return max;
		}

        /* overloaded operators */
        // functions can be implemented within the class
        // as with the overloaded operator []
        double operator[](int n) {return v[n];}
		double operator[](int n) const {return v[n];}
        // functions can be declared here and implemented outside of the class
        void operator = (const vec3D & v);
        void operator += (const vec3D & v);
		

        friend bool operator == (const vec3D &v1, const vec3D &v2)
        {
        	for (int i = 0; i < 3; ++i)
        	{
        		if (v1[i] != v2[i]) {return false;}
        	}
        	return true;
        }

		void operator *= (double d)
		{
			x *= d;
			y *= d;
			z *= d;
		}

		void operator-=(const vec3D & nV)
		{
				for (unsigned int i = 0; i < 3; i++)
						v[i] -= nV[i];
		}

		
		void operator /= (double d) 
		{
			d = 1.0/d;
			*this *= d;
		}

		double magnitudeSquared() const {return (x*x) + (y*y) + (z*z);}
		void normalize() 
		{
			*this /= magnitude();
		} 

		/* friend overloaded operators */
		friend vec3D operator+(const vec3D &v1, const vec3D &v2) ;
		friend vec3D operator-(const vec3D &v1, const vec3D &v2) 
		{
			vec3D v3 = v1;
			v3 -= v2;
			return v3; 
		}
		friend vec3D operator*(const vec3D &v1, const double d) 
		{
			vec3D v2 = v1;
			v2 *= d;
			return v2; 
		}

		friend vec3D operator/(const vec3D &v1, const double d) 
		{
			vec3D v2 = v1;
			v2 /= d;
			return v2; 
		}

		friend ostream& operator<<(ostream& oStr, const vec3D &v);
		friend bool operator > (const vec3D &v1, const vec3D &v2) 
		{
			return v1.magnitudeSquared() > v2.magnitudeSquared(); 
		}

		friend istream& operator>>(istream& iStr, vec3D & v)
		{
			for (unsigned int i = 0; i < 3; i++)
				iStr >> v.v[i];
			return iStr;
		}

		// used for testing
		void absoluteMin() 
		{
			static const double m = 0.05;
			for (int i = 0; i < 3; i++) 
				v[i] > 0.0 ? v[i] = min(v[i], m) : x = min(v[i], -m);
		}

		friend class particle; 
		friend class simulation;
		friend class Node; 
		friend class BHTree;
};

void vec3D::operator=(const vec3D & nV)
{
        for (unsigned int i = 0; i < 3; i++)
                v[i] = nV[i];
}

void vec3D::operator+=(const vec3D & nV)
{
        for (unsigned int i = 0; i < 3; i++)
                v[i] += nV[i];
}

/* friend functions */

vec3D operator+(const vec3D & v1, const vec3D & v2) 
{
	vec3D v3 = v2;
	v3 += v1;
	return v3; 
}

ostream& operator<<(ostream& oStr, const vec3D & v)
{
	for (unsigned int i = 0; i < 3; i++)
		oStr << v[i] << ' ';
	//oStr << endl; 
    return oStr;
}

#endif