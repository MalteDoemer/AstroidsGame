#pragma once
#include <cinttypes>
#include <Windows.h>


template <class T>
	struct v2d_generic
	{
		T x = 0;
		T y = 0;
		inline v2d_generic() : x(0), y(0)                        {                                                            }
		inline v2d_generic(T _x, T _y) : x(_x), y(_y)            {                                                            }
		inline v2d_generic(COORD c) : x((T)c.X), y((T)c.Y)       {                                                            }
		inline v2d_generic(const v2d_generic& v) : x(v.x), y(v.y){                                                            }
		inline T mag()                                           { return std::sqrt(x * x + y * y);                           }
		inline T mag2()					                         { return x * x + y * y;                                      }
		inline T angle()                                         { return atan2(y,x);                                         }
		inline v2d_generic rotate(T rot)                         {T m =mag();T a = angle() + rot; return {cos(a)*m,sin(a)*m}; }
		inline v2d_generic  norm()                               { T r = 1 / mag(); return v2d_generic(x*r, y*r);             }
		inline v2d_generic  perp()                               { return v2d_generic(-y, x);                                 }
		inline T dot(const v2d_generic& rhs)                     { return this->x * rhs.x + this->y * rhs.y;                  }
		inline T cross(const v2d_generic& rhs)                   { return this->x * rhs.y - this->y * rhs.x;                  }
		inline v2d_generic  operator +  (const v2d_generic& rhs) const { return v2d_generic(this->x + rhs.x, this->y + rhs.y);}
		inline v2d_generic  operator -  (const v2d_generic& rhs) const { return v2d_generic(this->x - rhs.x, this->y - rhs.y);}
		inline v2d_generic  operator *  (const T& rhs)           const { return v2d_generic(this->x * rhs, this->y * rhs);    }
		inline v2d_generic  operator *  (const v2d_generic& rhs) const { return v2d_generic(this->x * rhs.x, this->y * rhs.y);}
		inline v2d_generic  operator /  (const T& rhs)           const { return v2d_generic(this->x / rhs, this->y / rhs);    }
		inline v2d_generic  operator /  (const v2d_generic& rhs) const { return v2d_generic(this->x / rhs.x, this->y / rhs.y);}
		inline v2d_generic& operator += (const v2d_generic& rhs) { this->x += rhs.x; this->y += rhs.y; return *this;          }
		inline v2d_generic& operator -= (const v2d_generic& rhs) { this->x -= rhs.x; this->y -= rhs.y; return *this;          }
		inline v2d_generic& operator *= (const T& rhs)           { this->x *= rhs; this->y *= rhs; return *this;              }
		inline v2d_generic& operator /= (const T& rhs)           { this->x /= rhs; this->y /= rhs; return *this;              }
		inline operator v2d_generic<int32_t>() const { return {static_cast<int32_t>(this->x), static_cast<int32_t>(this->y) };}
		inline operator v2d_generic<float>() const { return { static_cast<float>(this->x), static_cast<float>(this->y) };     }
		inline operator v2d_generic<double>() const { return { static_cast<double>(this->x), static_cast<double>(this->y) };  }

	};

	// Note: joshinils has some good suggestions here, but they are complicated to implement at this moment, 
	// however they will appear in a future version of PGE
	template<class T> inline v2d_generic<T> operator * (const float& lhs, const v2d_generic<T>& rhs)
	{ return v2d_generic<T>((T)(lhs * (float)rhs.x), (T)(lhs * (float)rhs.y)); }
	template<class T> inline v2d_generic<T> operator * (const double& lhs, const v2d_generic<T>& rhs)
	{ return v2d_generic<T>((T)(lhs * (double)rhs.x), (T)(lhs * (double)rhs.y)); }
	template<class T> inline v2d_generic<T> operator * (const int& lhs, const v2d_generic<T>& rhs)
	{ return v2d_generic<T>((T)(lhs * (int)rhs.x), (T)(lhs * (int)rhs.y)); }
	template<class T> inline v2d_generic<T> operator / (const float& lhs, const v2d_generic<T>& rhs)
	{ return v2d_generic<T>((T)(lhs / (float)rhs.x), (T)(lhs / (float)rhs.y)); }
	template<class T> inline v2d_generic<T> operator / (const double& lhs, const v2d_generic<T>& rhs)
	{ return v2d_generic<T>((T)(lhs / (double)rhs.x), (T)(lhs / (double)rhs.y)); }
	template<class T> inline v2d_generic<T> operator / (const int& lhs, const v2d_generic<T>& rhs)
	{ return v2d_generic<T>((T)(lhs / (int)rhs.x), (T)(lhs / (int)rhs.y)); }

	typedef v2d_generic<int32_t> vi2d;
	typedef v2d_generic<uint32_t> vu2d;
	typedef v2d_generic<float> vf2d;
	typedef v2d_generic<double> vd2d;