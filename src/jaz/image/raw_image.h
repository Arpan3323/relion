#ifndef JAZ_RAW_IMAGE_H
#define JAZ_RAW_IMAGE_H

#include <vector>
#include <string>
#include <src/macros.h>
#include <src/jaz/gravis/tImage.h>
#include <src/jaz/gravis/t3Vector.h>
#include <src/image.h>
#include <src/jaz/single_particle/vtk_helper.h>
#include <stddef.h>


template <typename T>
class RawImage
{
	public:
		
		static gravis::t3Vector<size_t> readSize(std::string filename);
		
		RawImage()
			: xdim(0), ydim(0), zdim(0)
		{}
		
		RawImage(size_t xdim, size_t ydim, size_t zdim, T* data);
		
		RawImage(const Image<T>& img);
		
		RawImage(const MultidimArray<T>& mda);
		
			
			long int xdim, ydim, zdim;
			T* data;
		
		
		/* operator (x,y,z): returns a reference to the indicated voxel.
		   The correct version (const or non-const) will be chosen by the compiler,
		   depending on whether the instance is declared as const or not.*/
		
		inline const T& operator() (size_t, size_t, size_t) const;
		inline T& operator() (size_t, size_t, size_t);
		
		inline const T& operator() (size_t, size_t) const;
		inline T& operator() (size_t, size_t);
		
		inline const T& operator[] (size_t) const;
		inline T& operator[] (size_t);
		
		inline const T* getData() const;
		inline T* getData();
		
		inline size_t getSize() const;
		
		std::string getSizeString() const;
		
		void fill(T t);
		
		RawImage<T> getFullRef();
		RawImage<T> getSliceRef(size_t z);
		const RawImage<T> getConstSliceRef(size_t z) const;
		RawImage<T> getSlabRef(size_t z, size_t thickness);
		//void copySliceFrom(size_t z, const RawImage<T>& src, size_t z_src = 0);
		
		template <typename SourceType>
		void copySliceFrom(size_t z, const RawImage<SourceType>& src, size_t z_src = 0);
		
		void swapWith(RawImage<T>& img);
		
		void write(std::string fn) const;
		void writePng(std::string fn) const;
		void writeVtk(std::string fn, gravis::d3Vector origin, gravis::d3Vector step) const;
		
		void copyTo(gravis::tImage<T>& img, int z = 0) const;
		void copyTo(Image<T>& img) const;
		void copyTo(RawImage<T>& img) const;
		
		void copyFrom(const Image<T>& img);
		void copyFrom(const RawImage<T>& img);
		
		bool hasSize(long int w, long int h, long int d);
		bool hasEqualSize(const RawImage<T>& img);
		
		std::string sizeString();
		
		template<typename T2> inline
		RawImage& operator += (const RawImage<T2>& v)
		{
			const long int s = xdim * ydim * zdim;
			
			for (int i = 0; i < s; i++)
			{
				data[i] += v.data[i];
			}
			
			return *this;
		}
		
		template<typename T2> inline
		RawImage& operator -= (const RawImage<T2>& v)
		{
			const long int s = xdim * ydim * zdim;
			
			for (int i = 0; i < s; i++)
			{
				data[i] -= v.data[i];
			}
			
			return *this;
		}
		
		RawImage& operator += (T t)
		{
			const long int s = xdim * ydim * zdim;
			
			for (int i = 0; i < s; i++)
			{
				data[i] += t;
			}
			
			return *this;
		}
		
		RawImage& operator -= (T t)
		{
			const long int s = xdim * ydim * zdim;
			
			for (int i = 0; i < s; i++)
			{
				data[i] -= t;
			}
			
			return *this;
		}
		
		template<typename T2> inline
		RawImage& operator *= (const RawImage<T2>& v)
		{
			const long int s = xdim * ydim * zdim;
			
			for (int i = 0; i < s; i++)
			{
				data[i] *= v.data[i];
			}
			
			return *this;
		}
		
		RawImage& operator *= (T t)
		{
			const long int s = xdim * ydim * zdim;
			
			for (int i = 0; i < s; i++)
			{
				data[i] *= t;
			}
			
			return *this;
		}
		
		template<typename T2> inline
		RawImage& operator /= (const RawImage<T2>& v)
		{
			const long int s = xdim * ydim * zdim;
			
			for (int i = 0; i < s; i++)
			{
				data[i] /= v.data[i];
			}
			
			return *this;
		}
		
		RawImage& operator /= (T t)
		{
			const long int s = xdim * ydim * zdim;
			
			for (int i = 0; i < s; i++)
			{
				data[i] /= t;
			}
			
			return *this;
		}
};

template <class T>
gravis::t3Vector<size_t> RawImage<T>::readSize(std::string filename)
{
	Image<T> img;
	img.read(filename, false);
	
	return gravis::t3Vector<size_t>(img.data.xdim, img.data.ydim, img.data.zdim);
}

// constructor(s)

template <class T>
RawImage<T>::RawImage(size_t xdim, size_t ydim, size_t zdim, T* data)
	:   xdim(xdim), ydim(ydim), zdim(zdim), data(data)
{
}

template<typename T>
RawImage<T>::RawImage(const Image<T> &img)
	:   xdim(img.data.xdim), ydim(img.data.ydim), zdim(img.data.zdim), 
		data(&img(0,0,0))
{	
}

template<typename T>
RawImage<T>::RawImage(const MultidimArray<T> &mda)
	:   xdim(mda.xdim), ydim(mda.ydim), zdim(mda.zdim), 
		data(&mda(0))
{	
}

template<typename T>
RawImage<T> RawImage<T>::getFullRef()
{
	return RawImage<T>(xdim, ydim, zdim, data);
}

template<typename T>
RawImage<T> RawImage<T>::getSliceRef(size_t z)
{
	return RawImage<T>(xdim, ydim, 1, data + xdim * ydim * z);
}

template<typename T>
const RawImage<T> RawImage<T>::getConstSliceRef(size_t z) const
{
	return RawImage<T>(xdim, ydim, 1, data + xdim * ydim * z);
}

template<typename T>
RawImage<T> RawImage<T>::getSlabRef(size_t z, size_t thickness)
{
	return RawImage<T>(xdim, ydim, thickness, data + xdim * ydim * z);
}

template<typename T> template<typename SourceType>
void RawImage<T>::copySliceFrom(size_t z_dest, const RawImage<SourceType>& src, size_t z_src)
{
	for (int y = 0; y < ydim; y++)
	for (int x = 0; x < xdim; x++)
	{
		data[z_dest * ydim * xdim + y * xdim + x] = (T) src(x,y,z_src);
	}
}

template<typename T>
void RawImage<T>::swapWith(RawImage<T>& img)
{
	T* sw = img.data;
	img.data = data;
	data = sw;
}

// access operators

template <class T> 
inline const T& RawImage<T>::operator() (size_t x, size_t y, size_t z) const
{
	return data[(z*ydim + y)*xdim + x];
}

template <class T>
inline T& RawImage<T>::operator() (size_t x, size_t y, size_t z)
{
	return data[(z*ydim + y)*xdim + x];
}

template <class T>
inline const T& RawImage<T>::operator() (size_t x, size_t y) const
{
	return data[y*xdim + x];
}

template <class T>
inline T& RawImage<T>::operator() (size_t x, size_t y)
{
	return data[y*xdim + x];
}

template <class T>
inline const T& RawImage<T>::operator[] (size_t i) const
{
	return data[i];
}

template <class T>
inline T& RawImage<T>::operator[] (size_t i)
{
	return data[i];
}

// utility functions

template <class T>
inline const T* RawImage<T>::getData() const
{
	return data;
}

template <class T>
inline T* RawImage<T>::getData()
{
	return data;
}

template <class T>
inline size_t RawImage<T>::getSize() const
{
	return xdim * ydim * zdim;
}

template <class T>
inline std::string RawImage<T>::getSizeString() const
{
	std::ostringstream sts;
	sts << xdim << 'x' << ydim << 'x' << zdim;
	return sts.str();
}

template <class T>
inline void RawImage<T>::fill(T t)
{
	const long int s = xdim * ydim * zdim;
	
	for (long int i = 0; i < s; i++)
	{
		data[i] = t;
	}
}

// I/O

template <class T>
void RawImage<T>::write(std::string fn) const
{
	std::string::size_type dot = fn.find_last_of('.');
	
	if (dot == std::string::npos)
	{
		REPORT_ERROR_STR("RawImage<T>::write: filename has no ending (" << fn << ").");
	}
	
	std::string end = fn.substr(dot+1);
	
	if (end == "vtk")
	{
		Image<T> img;
		copyTo(img);
		VtkHelper::writeVTK(img, fn);
	}
	else
	{
		Image<T> img;
		copyTo(img);
		img.write(fn);
	}
}

template <class T>
void RawImage<T>::writePng(std::string fn) const
{
	if (zdim > 1) 
	{
		REPORT_ERROR("RawImage<T>::write: cannot write 3D image into PNG.");
	}
	
	gravis::tImage<T> img;
	copyTo(img);
	img.write(fn);
}

template <class T>
void RawImage<T>::writeVtk(std::string fn, gravis::d3Vector origin, gravis::d3Vector step) const
{
	Image<T> img;
	copyTo(img);
	
	VtkHelper::writeVTK(
				img, fn, 
				origin.x, origin.y, origin.z, 
				step.x, step.y, step.z);
}

template <class T>
inline void RawImage<T>::copyTo(gravis::tImage<T>& img, int z) const
{
	img = gravis::tImage<T>(xdim, ydim);
	
	for (size_t y = 0; y < ydim; y++)
		for (size_t x = 0; x < xdim; x++)
		{
			img(x,y) = data[(z*ydim + y)*xdim + x];
		}
}

template <class T>
inline void RawImage<T>::copyTo(Image<T>& img) const
{
	img = Image<T>(xdim, ydim, zdim);
	
	for (size_t z = 0; z < zdim; z++)
	for (size_t y = 0; y < ydim; y++)
	for (size_t x = 0; x < xdim; x++)
	{
		img(z,y,x) = data[(z*ydim + y)*xdim + x];
	}
}

template <class T>
inline void RawImage<T>::copyTo(RawImage<T>& img) const
{
	for (size_t z = 0; z < zdim; z++)
	for (size_t y = 0; y < ydim; y++)
	for (size_t x = 0; x < xdim; x++)
	{
		img(x,y,z) = data[(z*ydim + y)*xdim + x];
	}
}

template <class T>
inline void RawImage<T>::copyFrom(const Image<T>& img)
{
	if (xdim < img.data.xdim || ydim < img.data.ydim || zdim < img.data.zdim)
	{
		REPORT_ERROR_STR("RawImage<T>::copyFrom: input image too small: "
			<< img.data.xdim << "x" << img.data.ydim << "x" << img.data.zdim 
			<< " (should be at least: " 
			<< xdim << "x" << ydim << "x" << zdim << ")");
	}
	
	for (size_t z = 0; z < zdim; z++)
	for (size_t y = 0; y < ydim; y++)
	for (size_t x = 0; x < xdim; x++)
	{
		data[(z*ydim + y)*xdim + x] = img(x,y,z);
	}
}

template <class T>
inline void RawImage<T>::copyFrom(const RawImage<T>& img)
{
	if (xdim < img.xdim || ydim < img.ydim || zdim < img.zdim)
	{
		REPORT_ERROR_STR("RawImage<T>::copyFrom: input image too small: "
			<< img.xdim << "x" << img.ydim << "x" << img.zdim 
			<< " (should be at least: " 
			<< xdim << "x" << ydim << "x" << zdim << ")");
	}
	
	for (size_t z = 0; z < zdim; z++)
	for (size_t y = 0; y < ydim; y++)
	for (size_t x = 0; x < xdim; x++)
	{
		data[(z*ydim + y)*xdim + x] = img(x,y,z);
	}
}

template <class T>
bool RawImage<T>::hasSize(long int w, long int h, long int d)
{
	return xdim == w && ydim == h && zdim == d;
}

template <class T>
bool RawImage<T>::hasEqualSize(const RawImage<T>& img)
{
	return xdim == img.xdim && ydim == img.ydim && zdim == img.zdim;
}

template <class T>
std::string RawImage<T>::sizeString()
{
	std::stringstream sts;
	sts << xdim << "x" << ydim << "x" << zdim;
	
	return sts.str();
}

template <class T1, class T2>
RawImage<T1> operator * (const RawImage<T1>& t1, const RawImage<T2>& t2)
{
	RawImage<T1> out = t1;
	out += t2;
	return out;
}

#endif
