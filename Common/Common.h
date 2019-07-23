#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#ifdef __GNUC__
#define COMMON_INLINE inline
#elif _MSC_VER
#define COMMON_INLINE inline
#else
#error RendererLib::Unsupported compiler!
#endif // __GNUC__

//template <class T>
//class IsEqualTestable
//{
//
//public:
//
//	bool operator==(const T& rhs)
//	{
//		return this* == rhs;
//	}
//};


#endif // COMMON_H_INCLUDED
