/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//                           License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009-2011, Willow Garage Inc., all rights reserved.
// Copyright (C) 2014, Itseez Inc., all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

/* ////////////////////////////////////////////////////////////////////
//
//  Mat basic operations: Copy, Set
//
// */

#include "precomp.hpp"
#include "opencl_kernels_core.hpp"


namespace cv
{

template<typename T> static void
copyMask_(const uchar* _src, size_t sstep, const uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    for( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        const T* src = (const T*)_src;
        T* dst = (T*)_dst;
        int x = 0;
         #if CV_ENABLE_UNROLLED
        for( ; x <= size.width - 4; x += 4 )
        {
            if( mask[x] )
                dst[x] = src[x];
            if( mask[x+1] )
                dst[x+1] = src[x+1];
            if( mask[x+2] )
                dst[x+2] = src[x+2];
            if( mask[x+3] )
                dst[x+3] = src[x+3];
        }
        #endif
        for( ; x < size.width; x++ )
            if( mask[x] )
                dst[x] = src[x];
    }
}

template<> void
copyMask_<uchar>(const uchar* _src, size_t sstep, const uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_8u_C1MR, _src, (int)sstep, _dst, (int)dstep, ippiSize(size), mask, (int)mstep) >= 0)

    for( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        const uchar* src = (const uchar*)_src;
        uchar* dst = (uchar*)_dst;
        int x = 0;
        #if CV_SIMD
        {
            v_uint8 v_zero = vx_setzero_u8();

            for( ; x <= size.width - v_uint8::nlanes; x += v_uint8::nlanes )
            {
                v_uint8 v_src   = vx_load(src  + x),
                        v_dst   = vx_load(dst  + x),
                        v_nmask = vx_load(mask + x) == v_zero;

                v_dst = v_select(v_nmask, v_dst, v_src);
                v_store(dst + x, v_dst);
            }
        }
        vx_cleanup();
        #endif
        for( ; x < size.width; x++ )
            if( mask[x] )
                dst[x] = src[x];
    }
}

template<> void
copyMask_<ushort>(const uchar* _src, size_t sstep, const uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_16u_C1MR, (const Ipp16u *)_src, (int)sstep, (Ipp16u *)_dst, (int)dstep, ippiSize(size), mask, (int)mstep) >= 0)

    for( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        const ushort* src = (const ushort*)_src;
        ushort* dst = (ushort*)_dst;
        int x = 0;
        #if CV_SIMD
        {
            v_uint8 v_zero = vx_setzero_u8();

            for( ; x <= size.width - v_uint8::nlanes; x += v_uint8::nlanes )
            {
                v_uint16 v_src1 = vx_load(src + x), v_src2 = vx_load(src + x + v_uint16::nlanes),
                         v_dst1 = vx_load(dst + x), v_dst2 = vx_load(dst + x + v_uint16::nlanes);

                v_uint8 v_nmask1, v_nmask2;
                v_uint8 v_nmask = vx_load(mask + x) == v_zero;
                v_zip(v_nmask, v_nmask, v_nmask1, v_nmask2);

                v_dst1 = v_select(v_reinterpret_as_u16(v_nmask1), v_dst1, v_src1);
                v_dst2 = v_select(v_reinterpret_as_u16(v_nmask2), v_dst2, v_src2);
                v_store(dst + x, v_dst1);
                v_store(dst + x + v_uint16::nlanes, v_dst2);
            }
        }
        vx_cleanup();
        #endif
        for( ; x < size.width; x++ )
            if( mask[x] )
                dst[x] = src[x];
    }
}

static void
copyMaskGeneric(const uchar* _src, size_t sstep, const uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size, void* _esz)
{
    size_t k, esz = *(size_t*)_esz;
    for( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        const uchar* src = _src;
        uchar* dst = _dst;
        int x = 0;
        for( ; x < size.width; x++, src += esz, dst += esz )
        {
            if( !mask[x] )
                continue;
            for( k = 0; k < esz; k++ )
                dst[k] = src[k];
        }
    }
}


#define DEF_COPY_MASK(suffix, type) \
static void copyMask##suffix(const uchar* src, size_t sstep, const uchar* mask, size_t mstep, \
                             uchar* dst, size_t dstep, Size size, void*) \
{ \
    copyMask_<type>(src, sstep, mask, mstep, dst, dstep, size); \
}

#if defined HAVE_IPP
#define DEF_COPY_MASK_F(suffix, type, ippfavor, ipptype) \
static void copyMask##suffix(const uchar* src, size_t sstep, const uchar* mask, size_t mstep, \
                             uchar* dst, size_t dstep, Size size, void*) \
{ \
    CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_##ippfavor, (const ipptype *)src, (int)sstep, (ipptype *)dst, (int)dstep, ippiSize(size), (const Ipp8u *)mask, (int)mstep) >= 0)\
    copyMask_<type>(src, sstep, mask, mstep, dst, dstep, size); \
}
#else
#define DEF_COPY_MASK_F(suffix, type, ippfavor, ipptype) \
static void copyMask##suffix(const uchar* src, size_t sstep, const uchar* mask, size_t mstep, \
                             uchar* dst, size_t dstep, Size size, void*) \
{ \
    copyMask_<type>(src, sstep, mask, mstep, dst, dstep, size); \
}
#endif

#if IPP_VERSION_X100 == 901 // bug in IPP 9.0.1
DEF_COPY_MASK(32sC3, Vec3i)
DEF_COPY_MASK(8uC3, Vec3b)
#else
DEF_COPY_MASK_F(8uC3, Vec3b, 8u_C3MR, Ipp8u)
DEF_COPY_MASK_F(32sC3, Vec3i, 32s_C3MR, Ipp32s)
#endif
DEF_COPY_MASK(8u, uchar)
DEF_COPY_MASK(16u, ushort)
DEF_COPY_MASK_F(32s, int, 32s_C1MR, Ipp32s)
DEF_COPY_MASK_F(16uC3, Vec3s, 16u_C3MR, Ipp16u)
DEF_COPY_MASK(32sC2, Vec2i)
DEF_COPY_MASK_F(32sC4, Vec4i, 32s_C4MR, Ipp32s)
DEF_COPY_MASK(32sC6, Vec6i)
DEF_COPY_MASK(32sC8, Vec8i)

BinaryFunc copyMaskTab[] =
{
    0,
    copyMask8u,
    copyMask16u,
    copyMask8uC3,
    copyMask32s,
    0,
    copyMask16uC3,
    0,
    copyMask32sC2,
    0, 0, 0,
    copyMask32sC3,
    0, 0, 0,
    copyMask32sC4,
    0, 0, 0, 0, 0, 0, 0,
    copyMask32sC6,
    0, 0, 0, 0, 0, 0, 0,
    copyMask32sC8
};

BinaryFunc getCopyMaskFunc(size_t esz)
{
    return esz <= 32 && copyMaskTab[esz] ? copyMaskTab[esz] : copyMaskGeneric;
}

/* dst = src */
void Mat::copyTo( OutputArray _dst ) const
{
    CV_INSTRUMENT_REGION();

#ifdef HAVE_CUDA
    if (_dst.isGpuMat())
    {
        _dst.getGpuMat().upload(*this);
        return;
    }
#endif

    int dtype = _dst.type();
    if( _dst.fixedType() && dtype != type() )
    {
        CV_Assert( channels() == CV_MAT_CN(dtype) );
        convertTo( _dst, dtype );
        return;
    }

    if( empty() )
    {
        _dst.release();
        return;
    }

    if( _dst.isUMat() )
    {
        _dst.create( dims, size.p, type() );
        UMat dst = _dst.getUMat();
        CV_Assert(dst.u != NULL);
        size_t i, sz[CV_MAX_DIM] = {0}, dstofs[CV_MAX_DIM], esz = elemSize();
        CV_Assert(dims > 0 && dims < CV_MAX_DIM);
        for( i = 0; i < (size_t)dims; i++ )
            sz[i] = size.p[i];
        sz[dims-1] *= esz;
        dst.ndoffset(dstofs);
        dstofs[dims-1] *= esz;
        dst.u->currAllocator->upload(dst.u, data, dims, sz, dstofs, dst.step.p, step.p);
        return;
    }

    if( dims <= 2 )
    {
        _dst.create( rows, cols, type() );
        Mat dst = _dst.getMat();
        if( data == dst.data )
            return;

        if( rows > 0 && cols > 0 )
        {
            Mat src = *this;
            Size sz = getContinuousSize2D(src, dst, (int)elemSize());
            CV_CheckGE(sz.width, 0, "");

            const uchar* sptr = src.data;
            uchar* dptr = dst.data;

#if IPP_VERSION_X100 >= 201700
            CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_8u_C1R_L, sptr, (int)src.step, dptr, (int)dst.step, ippiSizeL(sz.width, sz.height)) >= 0)
#endif

            for (; sz.height--; sptr += src.step, dptr += dst.step)
                memcpy(dptr, sptr, sz.width);
        }
        return;
    }

    _dst.create( dims, size, type() );
    Mat dst = _dst.getMat();
    if( data == dst.data )
        return;

    if( total() != 0 )
    {
        const Mat* arrays[] = { this, &dst };
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs, 2);
        size_t sz = it.size*elemSize();

        for( size_t i = 0; i < it.nplanes; i++, ++it )
            memcpy(ptrs[1], ptrs[0], sz);
    }
}

#ifdef HAVE_IPP
static bool ipp_copyTo(const Mat &src, Mat &dst, const Mat &mask)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    if(mask.channels() > 1 || mask.depth() != CV_8U)
        return false;

    if (src.dims <= 2)
    {
        IppiSize size = ippiSize(src.size());
        return CV_INSTRUMENT_FUN_IPP(llwiCopyMask, src.ptr(), (int)src.step, dst.ptr(), (int)dst.step, size, (int)src.elemSize1(), src.channels(), mask.ptr(), (int)mask.step) >= 0;
    }
    else
    {
        const Mat      *arrays[] = {&src, &dst, &mask, NULL};
        uchar          *ptrs[3]  = {NULL};
        NAryMatIterator it(arrays, ptrs);

        IppiSize size = ippiSize(it.size, 1);

        for (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            if(CV_INSTRUMENT_FUN_IPP(llwiCopyMask, ptrs[0], 0, ptrs[1], 0, size, (int)src.elemSize1(), src.channels(), ptrs[2], 0) < 0)
                return false;
        }
        return true;
    }
#else
    CV_UNUSED(src); CV_UNUSED(dst); CV_UNUSED(mask);
    return false;
#endif
}
#endif

void Mat::copyTo( OutputArray _dst, InputArray _mask ) const
{
    CV_INSTRUMENT_REGION();

    Mat mask = _mask.getMat();
    if( !mask.data )
    {
        copyTo(_dst);
        return;
    }

    int cn = channels(), mcn = mask.channels();
    CV_Assert( mask.depth() == CV_8U && (mcn == 1 || mcn == cn) );
    bool colorMask = mcn > 1;
    if( dims <= 2 )
    {
        CV_Assert( size() == mask.size() );
    }

    Mat dst;
    {
        Mat dst0 = _dst.getMat();
        _dst.create(dims, size, type()); // TODO Prohibit 'dst' re-creation, user should pass it explicitly with correct size/type or empty
        dst = _dst.getMat();

        if (dst.data != dst0.data) // re-allocation happened
        {
#ifdef OPENCV_FUTURE
            CV_Assert(dst0.empty() &&
                "copyTo(): dst size/type mismatch (looks like a bug) - use dst.release() before copyTo() call to suppress this message");
#endif
            dst = Scalar(0); // do not leave dst uninitialized
        }
    }

    CV_IPP_RUN_FAST(ipp_copyTo(*this, dst, mask))

    size_t esz = colorMask ? elemSize1() : elemSize();
    BinaryFunc copymask = getCopyMaskFunc(esz);

    if( dims <= 2 )
    {
        Mat src = *this;
        Size sz = getContinuousSize2D(src, dst, mask, mcn);
        copymask(src.data, src.step, mask.data, mask.step, dst.data, dst.step, sz, &esz);
        return;
    }

    const Mat* arrays[] = { this, &dst, &mask, 0 };
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    Size sz((int)(it.size*mcn), 1);

    for( size_t i = 0; i < it.nplanes; i++, ++it )
        copymask(ptrs[0], 0, ptrs[2], 0, ptrs[1], 0, sz, &esz);
}


static bool can_apply_memset(const Mat &mat, const Scalar &s, int &fill_value)
{
    // check if depth is 1 byte.
    switch (mat.depth())
    {
    case CV_8U: fill_value = saturate_cast<uchar>( s.val[0] ); break;
    case CV_8S: fill_value = saturate_cast<schar>( s.val[0] ); break;
    default: return false;
    }

    // check if all element is same.
    const int64* is = (const int64*)&s.val[0];
    switch (mat.channels())
    {
    case 1: return true;
    case 2: return (is[0] == is[1]);
    case 3: return (is[0] == is[1] && is[1] == is[2]);
    case 4: return (is[0] == is[1] && is[1] == is[2] && is[2] == is[3]);
    default: return false;
    }
}

Mat& Mat::operator = (const Scalar& s)
{
    CV_INSTRUMENT_REGION();

    if (this->empty())
        return *this;

    const Mat* arrays[] = { this };
    uchar* dptr;
    NAryMatIterator it(arrays, &dptr, 1);
    size_t elsize = it.size*elemSize();
    const int64* is = (const int64*)&s.val[0];

    if( is[0] == 0 && is[1] == 0 && is[2] == 0 && is[3] == 0 )
    {
        for( size_t i = 0; i < it.nplanes; i++, ++it )
            memset( dptr, 0, elsize );
    }
    else
    {
        int fill_value = 0;
        if ( can_apply_memset(*this, s, fill_value) )
        {
            for (size_t i = 0; i < it.nplanes; i++, ++it)
                memset(dptr, fill_value, elsize);
            return *this;
        }

        if( it.nplanes > 0 )
        {
            double scalar[12];
            scalarToRawData(s, scalar, type(), 12);
            size_t blockSize = 12*elemSize1();

            for( size_t j = 0; j < elsize; j += blockSize )
            {
                size_t sz = MIN(blockSize, elsize - j);
                CV_Assert(sz <= sizeof(scalar));
                memcpy( dptr + j, scalar, sz );
            }
        }

        for( size_t i = 1; i < it.nplanes; i++ )
        {
            ++it;
            memcpy( dptr, data, elsize );
        }
    }
    return *this;
}

#ifdef HAVE_IPP
static bool ipp_Mat_setTo_Mat(Mat &dst, Mat &_val, Mat &mask)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    if(mask.empty())
        return false;

    if(mask.depth() != CV_8U || mask.channels() > 1)
        return false;

    if(dst.channels() > 4)
        return false;

    if (dst.depth() == CV_32F)
    {
        for (int i = 0; i < (int)(_val.total()); i++)
        {
            float v = (float)(_val.at<double>(i));  // cast to float
            if (cvIsNaN(v) || cvIsInf(v))  // accept finite numbers only
                return false;
        }
    }

    if(dst.dims <= 2)
    {
        IppiSize            size     = ippiSize(dst.size());
        IppDataType         dataType = ippiGetDataType(dst.depth());
        ::ipp::IwValueFloat s;
        convertAndUnrollScalar(_val, CV_MAKETYPE(CV_64F, dst.channels()), (uchar*)((Ipp64f*)s), 1);

        return CV_INSTRUMENT_FUN_IPP(llwiSetMask, s, dst.ptr(), (int)dst.step, size, dataType, dst.channels(), mask.ptr(), (int)mask.step) >= 0;
    }
    else
    {
        const Mat      *arrays[] = {&dst, mask.empty()?NULL:&mask, NULL};
        uchar          *ptrs[2]  = {NULL};
        NAryMatIterator it(arrays, ptrs);

        IppiSize            size     = {(int)it.size, 1};
        IppDataType         dataType = ippiGetDataType(dst.depth());
        ::ipp::IwValueFloat s;
        convertAndUnrollScalar(_val, CV_MAKETYPE(CV_64F, dst.channels()), (uchar*)((Ipp64f*)s), 1);

        for( size_t i = 0; i < it.nplanes; i++, ++it)
        {
            if(CV_INSTRUMENT_FUN_IPP(llwiSetMask, s, ptrs[0], 0, size, dataType, dst.channels(), ptrs[1], 0) < 0)
                return false;
        }
        return true;
    }
#else
    CV_UNUSED(dst); CV_UNUSED(_val); CV_UNUSED(mask);
    return false;
#endif
}
#endif

Mat& Mat::setTo(InputArray _value, InputArray _mask)
{
    CV_INSTRUMENT_REGION();

    if( empty() )
        return *this;

    Mat value = _value.getMat(), mask = _mask.getMat();

    CV_Assert( checkScalar(value, type(), _value.kind(), _InputArray::MAT ));
    int cn = channels(), mcn = mask.channels();
    CV_Assert( mask.empty() || (mask.depth() == CV_8U && (mcn == 1 || mcn == cn) && size == mask.size) );

    CV_IPP_RUN_FAST(ipp_Mat_setTo_Mat(*this, value, mask), *this)

    size_t esz = mcn > 1 ? elemSize1() : elemSize();
    BinaryFunc copymask = getCopyMaskFunc(esz);

    const Mat* arrays[] = { this, !mask.empty() ? &mask : 0, 0 };
    uchar* ptrs[2]={0,0};
    NAryMatIterator it(arrays, ptrs);
    int totalsz = (int)it.size*mcn;
    int blockSize0 = std::min(totalsz, (int)((BLOCK_SIZE + esz-1)/esz));
    blockSize0 -= blockSize0 % mcn;    // must be divisible without remainder for unrolling and advancing
    AutoBuffer<uchar> _scbuf(blockSize0*esz + 32);
    uchar* scbuf = alignPtr((uchar*)_scbuf.data(), (int)sizeof(double));
    convertAndUnrollScalar( value, type(), scbuf, blockSize0/mcn );

    for( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        for( int j = 0; j < totalsz; j += blockSize0 )
        {
            Size sz(std::min(blockSize0, totalsz - j), 1);
            size_t blockSize = sz.width*esz;
            if( ptrs[1] )
            {
                copymask(scbuf, 0, ptrs[1], 0, ptrs[0], 0, sz, &esz);
                ptrs[1] += sz.width;
            }
            else
                memcpy(ptrs[0], scbuf, blockSize);
            ptrs[0] += blockSize;
        }
    }
    return *this;
}

#if CV_SIMD128
template<typename V> CV_ALWAYS_INLINE void flipHoriz_single( const uchar* src, size_t sstep, uchar* dst, size_t dstep, Size size, size_t esz )
{
    typedef typename V::lane_type T;
    int end = (int)(size.width*esz);
    int width = (end + 1)/2;
    int width_1 = width & -v_uint8x16::nlanes;
    int i, j;

#if CV_STRONG_ALIGNMENT
    CV_Assert(isAligned<sizeof(T)>(src, dst));
#endif

    for( ; size.height--; src += sstep, dst += dstep )
    {
        for( i = 0, j = end; i < width_1; i += v_uint8x16::nlanes, j -= v_uint8x16::nlanes )
        {
            V t0, t1;

            t0 = v_load((T*)((uchar*)src + i));
            t1 = v_load((T*)((uchar*)src + j - v_uint8x16::nlanes));
            t0 = v_reverse(t0);
            t1 = v_reverse(t1);
            v_store((T*)(dst + j - v_uint8x16::nlanes), t0);
            v_store((T*)(dst + i), t1);
        }
        if (isAligned<sizeof(T)>(src, dst))
        {
            for ( ; i < width; i += sizeof(T), j -= sizeof(T) )
            {
                T t0, t1;

                t0 = *((T*)((uchar*)src + i));
                t1 = *((T*)((uchar*)src + j - sizeof(T)));
                *((T*)(dst + j - sizeof(T))) = t0;
                *((T*)(dst + i)) = t1;
            }
        }
        else
        {
            for ( ; i < width; i += sizeof(T), j -= sizeof(T) )
            {
                for (int k = 0; k < (int)sizeof(T); k++)
                {
                    uchar t0, t1;

                    t0 = *((uchar*)src + i + k);
                    t1 = *((uchar*)src + j + k - sizeof(T));
                    *(dst + j + k - sizeof(T)) = t0;
                    *(dst + i + k) = t1;
                }
            }
        }
    }
}

template<typename T1, typename T2> CV_ALWAYS_INLINE void flipHoriz_double( const uchar* src, size_t sstep, uchar* dst, size_t dstep, Size size, size_t esz )
{
    int end = (int)(size.width*esz);
    int width = (end + 1)/2;

#if CV_STRONG_ALIGNMENT
    CV_Assert(isAligned<sizeof(T1)>(src, dst));
    CV_Assert(isAligned<sizeof(T2)>(src, dst));
#endif

    for( ; size.height--; src += sstep, dst += dstep )
    {
        for ( int i = 0, j = end; i < width; i += sizeof(T1) + sizeof(T2), j -= sizeof(T1) + sizeof(T2) )
        {
            T1 t0, t1;
            T2 t2, t3;

            t0 = *((T1*)((uchar*)src + i));
            t2 = *((T2*)((uchar*)src + i + sizeof(T1)));
            t1 = *((T1*)((uchar*)src + j - sizeof(T1) - sizeof(T2)));
            t3 = *((T2*)((uchar*)src + j - sizeof(T2)));
            *((T1*)(dst + j - sizeof(T1) - sizeof(T2))) = t0;
            *((T2*)(dst + j - sizeof(T2))) = t2;
            *((T1*)(dst + i)) = t1;
            *((T2*)(dst + i + sizeof(T1))) = t3;
        }
    }
}
#endif

static void
flipHoriz( const uchar* src, size_t sstep, uchar* dst, size_t dstep, Size size, size_t esz )
{
#if CV_SIMD
#if CV_STRONG_ALIGNMENT
    size_t alignmentMark = ((size_t)src)|((size_t)dst)|sstep|dstep;
#endif
    if (esz == 2 * v_uint8x16::nlanes)
    {
        int end = (int)(size.width*esz);
        int width = end/2;

        for( ; size.height--; src += sstep, dst += dstep )
        {
            for( int i = 0, j = end - 2 * v_uint8x16::nlanes; i < width; i += 2 * v_uint8x16::nlanes, j -= 2 * v_uint8x16::nlanes )
            {
#if CV_SIMD256
                v_uint8x32 t0, t1;

                t0 = v256_load((uchar*)src + i);
                t1 = v256_load((uchar*)src + j);
                v_store(dst + j, t0);
                v_store(dst + i, t1);
#else
                v_uint8x16 t0, t1, t2, t3;

                t0 = v_load((uchar*)src + i);
                t1 = v_load((uchar*)src + i + v_uint8x16::nlanes);
                t2 = v_load((uchar*)src + j);
                t3 = v_load((uchar*)src + j + v_uint8x16::nlanes);
                v_store(dst + j, t0);
                v_store(dst + j + v_uint8x16::nlanes, t1);
                v_store(dst + i, t2);
                v_store(dst + i + v_uint8x16::nlanes, t3);
#endif
            }
        }
    }
    else if (esz == v_uint8x16::nlanes)
    {
        int end = (int)(size.width*esz);
        int width = end/2;

        for( ; size.height--; src += sstep, dst += dstep )
        {
            for( int i = 0, j = end - v_uint8x16::nlanes; i < width; i += v_uint8x16::nlanes, j -= v_uint8x16::nlanes )
            {
                v_uint8x16 t0, t1;

                t0 = v_load((uchar*)src + i);
                t1 = v_load((uchar*)src + j);
                v_store(dst + j, t0);
                v_store(dst + i, t1);
            }
        }
    }
    else if (esz == 8
#if CV_STRONG_ALIGNMENT
            && isAligned<sizeof(uint64)>(alignmentMark)
#endif
    )
    {
        flipHoriz_single<v_uint64x2>(src, sstep, dst, dstep, size, esz);
    }
    else if (esz == 4
#if CV_STRONG_ALIGNMENT
            && isAligned<sizeof(unsigned)>(alignmentMark)
#endif
    )
    {
        flipHoriz_single<v_uint32x4>(src, sstep, dst, dstep, size, esz);
    }
    else if (esz == 2
#if CV_STRONG_ALIGNMENT
            && isAligned<sizeof(ushort)>(alignmentMark)
#endif
    )
    {
        flipHoriz_single<v_uint16x8>(src, sstep, dst, dstep, size, esz);
    }
    else if (esz == 1)
    {
        flipHoriz_single<v_uint8x16>(src, sstep, dst, dstep, size, esz);
    }
    else if (esz == 24
#if CV_STRONG_ALIGNMENT
            && isAligned<sizeof(uint64_t)>(alignmentMark)
#endif
    )
    {
        int end = (int)(size.width*esz);
        int width = (end + 1)/2;

        for( ; size.height--; src += sstep, dst += dstep )
        {
            for ( int i = 0, j = end; i < width; i += v_uint8x16::nlanes + sizeof(uint64_t), j -= v_uint8x16::nlanes + sizeof(uint64_t) )
            {
                v_uint8x16 t0, t1;
                uint64_t t2, t3;

                t0 = v_load((uchar*)src + i);
                t2 = *((uint64_t*)((uchar*)src + i + v_uint8x16::nlanes));
                t1 = v_load((uchar*)src + j - v_uint8x16::nlanes - sizeof(uint64_t));
                t3 = *((uint64_t*)((uchar*)src + j - sizeof(uint64_t)));
                v_store(dst + j - v_uint8x16::nlanes - sizeof(uint64_t), t0);
                *((uint64_t*)(dst + j - sizeof(uint64_t))) = t2;
                v_store(dst + i, t1);
                *((uint64_t*)(dst + i + v_uint8x16::nlanes)) = t3;
            }
        }
    }
#if !CV_STRONG_ALIGNMENT
    else if (esz == 12)
    {
        flipHoriz_double<uint64_t,uint>(src, sstep, dst, dstep, size, esz);
    }
    else if (esz == 6)
    {
        flipHoriz_double<uint,ushort>(src, sstep, dst, dstep, size, esz);
    }
    else if (esz == 3)
    {
        flipHoriz_double<ushort,uchar>(src, sstep, dst, dstep, size, esz);
    }
#endif
    else
#endif // CV_SIMD
    {
        int i, j, limit = (int)(((size.width + 1)/2)*esz);
        AutoBuffer<int> _tab(size.width*esz);
        int* tab = _tab.data();

        for( i = 0; i < size.width; i++ )
            for( size_t k = 0; k < esz; k++ )
                tab[i*esz + k] = (int)((size.width - i - 1)*esz + k);

        for( ; size.height--; src += sstep, dst += dstep )
        {
            for( i = 0; i < limit; i++ )
            {
                j = tab[i];
                uchar t0 = src[i], t1 = src[j];
                dst[i] = t1; dst[j] = t0;
            }
        }
    }
}

static void
flipVert( const uchar* src0, size_t sstep, uchar* dst0, size_t dstep, Size size, size_t esz )
{
    const uchar* src1 = src0 + (size.height - 1)*sstep;
    uchar* dst1 = dst0 + (size.height - 1)*dstep;
    size.width *= (int)esz;

    for( int y = 0; y < (size.height + 1)/2; y++, src0 += sstep, src1 -= sstep,
                                                  dst0 += dstep, dst1 -= dstep )
    {
        int i = 0;
#if CV_SIMD
#if CV_STRONG_ALIGNMENT
        if (isAligned<sizeof(int)>(src0, src1, dst0, dst1))
#endif
        {
            for (; i <= size.width - CV_SIMD_WIDTH; i += CV_SIMD_WIDTH)
            {
                v_int32 t0 = vx_load((int*)(src0 + i));
                v_int32 t1 = vx_load((int*)(src1 + i));
                vx_store((int*)(dst0 + i), t1);
                vx_store((int*)(dst1 + i), t0);
            }
        }
#if CV_STRONG_ALIGNMENT
        else
        {
            for (; i <= size.width - CV_SIMD_WIDTH; i += CV_SIMD_WIDTH)
            {
                v_uint8 t0 = vx_load(src0 + i);
                v_uint8 t1 = vx_load(src1 + i);
                vx_store(dst0 + i, t1);
                vx_store(dst1 + i, t0);
            }
        }
#endif
#endif

        if (isAligned<sizeof(int)>(src0, src1, dst0, dst1))
        {
            for( ; i <= size.width - 16; i += 16 )
            {
                int t0 = ((int*)(src0 + i))[0];
                int t1 = ((int*)(src1 + i))[0];

                ((int*)(dst0 + i))[0] = t1;
                ((int*)(dst1 + i))[0] = t0;

                t0 = ((int*)(src0 + i))[1];
                t1 = ((int*)(src1 + i))[1];

                ((int*)(dst0 + i))[1] = t1;
                ((int*)(dst1 + i))[1] = t0;

                t0 = ((int*)(src0 + i))[2];
                t1 = ((int*)(src1 + i))[2];

                ((int*)(dst0 + i))[2] = t1;
                ((int*)(dst1 + i))[2] = t0;

                t0 = ((int*)(src0 + i))[3];
                t1 = ((int*)(src1 + i))[3];

                ((int*)(dst0 + i))[3] = t1;
                ((int*)(dst1 + i))[3] = t0;
            }

            for( ; i <= size.width - 4; i += 4 )
            {
                int t0 = ((int*)(src0 + i))[0];
                int t1 = ((int*)(src1 + i))[0];

                ((int*)(dst0 + i))[0] = t1;
                ((int*)(dst1 + i))[0] = t0;
            }
        }

        for( ; i < size.width; i++ )
        {
            uchar t0 = src0[i];
            uchar t1 = src1[i];

            dst0[i] = t1;
            dst1[i] = t0;
        }
    }
}

#ifdef HAVE_OPENCL

enum { FLIP_COLS = 1 << 0, FLIP_ROWS = 1 << 1, FLIP_BOTH = FLIP_ROWS | FLIP_COLS };

static bool ocl_flip(InputArray _src, OutputArray _dst, int flipCode )
{
    CV_Assert(flipCode >= -1 && flipCode <= 1);

    const ocl::Device & dev = ocl::Device::getDefault();
    int type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            flipType, kercn = std::min(ocl::predictOptimalVectorWidth(_src, _dst), 4);

    bool doubleSupport = dev.doubleFPConfig() > 0;
    if (!doubleSupport && depth == CV_64F)
        kercn = cn;

    if (cn > 4)
        return false;

    const char * kernelName;
    if (flipCode == 0)
        kernelName = "arithm_flip_rows", flipType = FLIP_ROWS;
    else if (flipCode > 0)
        kernelName = "arithm_flip_cols", flipType = FLIP_COLS;
    else
        kernelName = "arithm_flip_rows_cols", flipType = FLIP_BOTH;

    int pxPerWIy = (dev.isIntel() && (dev.type() & ocl::Device::TYPE_GPU)) ? 4 : 1;
    kercn = (cn!=3 || flipType == FLIP_ROWS) ? std::max(kercn, cn) : cn;

    ocl::Kernel k(kernelName, ocl::core::flip_oclsrc,
        format( "-D T=%s -D T1=%s -D DEPTH=%d -D cn=%d -D PIX_PER_WI_Y=%d -D kercn=%d",
                kercn != cn ? ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)) : ocl::vecopTypeToStr(CV_MAKE_TYPE(depth, kercn)),
                kercn != cn ? ocl::typeToStr(depth) : ocl::vecopTypeToStr(depth), depth, cn, pxPerWIy, kercn));
    if (k.empty())
        return false;

    Size size = _src.size();
    _dst.create(size, type);
    UMat src = _src.getUMat(), dst = _dst.getUMat();

    int cols = size.width * cn / kercn, rows = size.height;
    cols = flipType == FLIP_COLS ? (cols + 1) >> 1 : cols;
    rows = flipType & FLIP_ROWS ? (rows + 1) >> 1 : rows;

    k.args(ocl::KernelArg::ReadOnlyNoSize(src),
           ocl::KernelArg::WriteOnly(dst, cn, kercn), rows, cols);

    size_t maxWorkGroupSize = dev.maxWorkGroupSize();
    CV_Assert(maxWorkGroupSize % 4 == 0);

    size_t globalsize[2] = { (size_t)cols, ((size_t)rows + pxPerWIy - 1) / pxPerWIy },
            localsize[2] = { maxWorkGroupSize / 4, 4 };
    return k.run(2, globalsize, (flipType == FLIP_COLS) && !dev.isIntel() ? localsize : NULL, false);
}

#endif

#if defined HAVE_IPP
static bool ipp_flip(Mat &src, Mat &dst, int flip_mode)
{
#ifdef HAVE_IPP_IW
    CV_INSTRUMENT_REGION_IPP();

    // Details: https://github.com/opencv/opencv/issues/12943
    if (flip_mode <= 0 /* swap rows */
        && cv::ipp::getIppTopFeatures() != ippCPUID_SSE42
        && (int64_t)(src.total()) * src.elemSize() >= CV_BIG_INT(0x80000000)/*2Gb*/
    )
        return false;

    IppiAxis ippMode;
    if(flip_mode < 0)
        ippMode = ippAxsBoth;
    else if(flip_mode == 0)
        ippMode = ippAxsHorizontal;
    else
        ippMode = ippAxsVertical;

    try
    {
        ::ipp::IwiImage iwSrc = ippiGetImage(src);
        ::ipp::IwiImage iwDst = ippiGetImage(dst);

        CV_INSTRUMENT_FUN_IPP(::ipp::iwiMirror, iwSrc, iwDst, ippMode);
    }
    catch(const ::ipp::IwException &)
    {
        return false;
    }

    return true;
#else
    CV_UNUSED(src); CV_UNUSED(dst); CV_UNUSED(flip_mode);
    return false;
#endif
}
#endif


void flip( InputArray _src, OutputArray _dst, int flip_mode )
{
    CV_INSTRUMENT_REGION();

    CV_Assert( _src.dims() <= 2 );
    Size size = _src.size();

    if (flip_mode < 0)
    {
        if (size.width == 1)
            flip_mode = 0;
        if (size.height == 1)
            flip_mode = 1;
    }

    if ((size.width == 1 && flip_mode > 0) ||
        (size.height == 1 && flip_mode == 0) ||
        (size.height == 1 && size.width == 1 && flip_mode < 0))
    {
        return _src.copyTo(_dst);
    }

    CV_OCL_RUN( _dst.isUMat(), ocl_flip(_src, _dst, flip_mode))

    Mat src = _src.getMat();
    int type = src.type();
    _dst.create( size, type );
    Mat dst = _dst.getMat();

    CV_IPP_RUN_FAST(ipp_flip(src, dst, flip_mode));

    size_t esz = CV_ELEM_SIZE(type);

    if( flip_mode <= 0 )
        flipVert( src.ptr(), src.step, dst.ptr(), dst.step, src.size(), esz );
    else
        flipHoriz( src.ptr(), src.step, dst.ptr(), dst.step, src.size(), esz );

    if( flip_mode < 0 )
        flipHoriz( dst.ptr(), dst.step, dst.ptr(), dst.step, dst.size(), esz );
}

void rotate(InputArray _src, OutputArray _dst, int rotateMode)
{
    CV_Assert(_src.dims() <= 2);

    switch (rotateMode)
    {
    case ROTATE_90_CLOCKWISE:
        transpose(_src, _dst);
        flip(_dst, _dst, 1);
        break;
    case ROTATE_180:
        flip(_src, _dst, -1);
        break;
    case ROTATE_90_COUNTERCLOCKWISE:
        transpose(_src, _dst);
        flip(_dst, _dst, 0);
        break;
    default:
        break;
    }
}

#if defined HAVE_OPENCL && !defined __APPLE__

static bool ocl_repeat(InputArray _src, int ny, int nx, OutputArray _dst)
{
    if (ny == 1 && nx == 1)
    {
        _src.copyTo(_dst);
        return true;
    }

    int type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1,
            kercn = ocl::predictOptimalVectorWidth(_src, _dst);

    ocl::Kernel k("repeat", ocl::core::repeat_oclsrc,
                  format("-D T=%s -D nx=%d -D ny=%d -D rowsPerWI=%d -D cn=%d",
                         ocl::memopTypeToStr(CV_MAKE_TYPE(depth, kercn)),
                         nx, ny, rowsPerWI, kercn));
    if (k.empty())
        return false;

    UMat src = _src.getUMat(), dst = _dst.getUMat();
    k.args(ocl::KernelArg::ReadOnly(src, cn, kercn), ocl::KernelArg::WriteOnlyNoSize(dst));

    size_t globalsize[] = { (size_t)src.cols * cn / kercn, ((size_t)src.rows + rowsPerWI - 1) / rowsPerWI };
    return k.run(2, globalsize, NULL, false);
}

#endif

void repeat(InputArray _src, int ny, int nx, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(_src.getObj() != _dst.getObj());
    CV_Assert( _src.dims() <= 2 );
    CV_Assert( ny > 0 && nx > 0 );

    Size ssize = _src.size();
    _dst.create(ssize.height*ny, ssize.width*nx, _src.type());

#if !defined __APPLE__
    CV_OCL_RUN(_dst.isUMat(),
               ocl_repeat(_src, ny, nx, _dst))
#endif

    Mat src = _src.getMat(), dst = _dst.getMat();
    Size dsize = dst.size();
    int esz = (int)src.elemSize();
    int x, y;
    ssize.width *= esz; dsize.width *= esz;

    for( y = 0; y < ssize.height; y++ )
    {
        for( x = 0; x < dsize.width; x += ssize.width )
            memcpy( dst.ptr(y) + x, src.ptr(y), ssize.width );
    }

    for( ; y < dsize.height; y++ )
        memcpy( dst.ptr(y), dst.ptr(y - ssize.height), dsize.width );
}

Mat repeat(const Mat& src, int ny, int nx)
{
    if( nx == 1 && ny == 1 )
        return src;
    Mat dst;
    repeat(src, ny, nx, dst);
    return dst;
}


} // cv


/*
 Various border types, image boundaries are denoted with '|'

 * BORDER_REPLICATE:     aaaaaa|abcdefgh|hhhhhhh
 * BORDER_REFLECT:       fedcba|abcdefgh|hgfedcb
 * BORDER_REFLECT_101:   gfedcb|abcdefgh|gfedcba
 * BORDER_WRAP:          cdefgh|abcdefgh|abcdefg
 * BORDER_CONSTANT:      iiiiii|abcdefgh|iiiiiii  with some specified 'i'
 */
int cv::borderInterpolate( int p, int len, int borderType )
{
    CV_TRACE_FUNCTION_VERBOSE();

    CV_DbgAssert(len > 0);

#ifdef CV_STATIC_ANALYSIS
    if(p >= 0 && p < len)
#else
    if( (unsigned)p < (unsigned)len )
#endif
        ;
    else if( borderType == BORDER_REPLICATE )
        p = p < 0 ? 0 : len - 1;
    else if( borderType == BORDER_REFLECT || borderType == BORDER_REFLECT_101 )
    {
        int delta = borderType == BORDER_REFLECT_101;
        if( len == 1 )
            return 0;
        do
        {
            if( p < 0 )
                p = -p - 1 + delta;
            else
                p = len - 1 - (p - len) - delta;
        }
#ifdef CV_STATIC_ANALYSIS
        while(p < 0 || p >= len);
#else
        while( (unsigned)p >= (unsigned)len );
#endif
    }
    else if( borderType == BORDER_WRAP )
    {
        CV_Assert(len > 0);
        if( p < 0 )
            p -= ((p-len+1)/len)*len;
        if( p >= len )
            p %= len;
    }
    else if( borderType == BORDER_CONSTANT )
        p = -1;
    else
        CV_Error( CV_StsBadArg, "Unknown/unsupported border type" );
    return p;
}

namespace
{

void copyMakeBorder_8u( const uchar* src, size_t srcstep, cv::Size srcroi,
                        uchar* dst, size_t dststep, cv::Size dstroi,
                        int top, int left, int cn, int borderType )
{
    const int isz = (int)sizeof(int);
    int i, j, k, elemSize = 1;
    bool intMode = false;

    if( (cn | srcstep | dststep | (size_t)src | (size_t)dst) % isz == 0 )
    {
        cn /= isz;
        elemSize = isz;
        intMode = true;
    }

    cv::AutoBuffer<int> _tab((dstroi.width - srcroi.width)*cn);
    int* tab = _tab.data();
    int right = dstroi.width - srcroi.width - left;
    int bottom = dstroi.height - srcroi.height - top;

    for( i = 0; i < left; i++ )
    {
        j = cv::borderInterpolate(i - left, srcroi.width, borderType)*cn;
        for( k = 0; k < cn; k++ )
            tab[i*cn + k] = j + k;
    }

    for( i = 0; i < right; i++ )
    {
        j = cv::borderInterpolate(srcroi.width + i, srcroi.width, borderType)*cn;
        for( k = 0; k < cn; k++ )
            tab[(i+left)*cn + k] = j + k;
    }

    srcroi.width *= cn;
    dstroi.width *= cn;
    left *= cn;
    right *= cn;

    uchar* dstInner = dst + dststep*top + left*elemSize;

    for( i = 0; i < srcroi.height; i++, dstInner += dststep, src += srcstep )
    {
        if( dstInner != src )
            memcpy(dstInner, src, srcroi.width*elemSize);

        if( intMode )
        {
            const int* isrc = (int*)src;
            int* idstInner = (int*)dstInner;
            for( j = 0; j < left; j++ )
                idstInner[j - left] = isrc[tab[j]];
            for( j = 0; j < right; j++ )
                idstInner[j + srcroi.width] = isrc[tab[j + left]];
        }
        else
        {
            for( j = 0; j < left; j++ )
                dstInner[j - left] = src[tab[j]];
            for( j = 0; j < right; j++ )
                dstInner[j + srcroi.width] = src[tab[j + left]];
        }
    }

    dstroi.width *= elemSize;
    dst += dststep*top;

    for( i = 0; i < top; i++ )
    {
        j = cv::borderInterpolate(i - top, srcroi.height, borderType);
        memcpy(dst + (i - top)*dststep, dst + j*dststep, dstroi.width);
    }

    for( i = 0; i < bottom; i++ )
    {
        j = cv::borderInterpolate(i + srcroi.height, srcroi.height, borderType);
        memcpy(dst + (i + srcroi.height)*dststep, dst + j*dststep, dstroi.width);
    }
}


void copyMakeConstBorder_8u( const uchar* src, size_t srcstep, cv::Size srcroi,
                             uchar* dst, size_t dststep, cv::Size dstroi,
                             int top, int left, int cn, const uchar* value )
{
    int i, j;
    cv::AutoBuffer<uchar> _constBuf(dstroi.width*cn);
    uchar* constBuf = _constBuf.data();
    int right = dstroi.width - srcroi.width - left;
    int bottom = dstroi.height - srcroi.height - top;

    for( i = 0; i < dstroi.width; i++ )
    {
        for( j = 0; j < cn; j++ )
            constBuf[i*cn + j] = value[j];
    }

    srcroi.width *= cn;
    dstroi.width *= cn;
    left *= cn;
    right *= cn;

    uchar* dstInner = dst + dststep*top + left;

    for( i = 0; i < srcroi.height; i++, dstInner += dststep, src += srcstep )
    {
        if( dstInner != src )
            memcpy( dstInner, src, srcroi.width );
        memcpy( dstInner - left, constBuf, left );
        memcpy( dstInner + srcroi.width, constBuf, right );
    }

    dst += dststep*top;

    for( i = 0; i < top; i++ )
        memcpy(dst + (i - top)*dststep, constBuf, dstroi.width);

    for( i = 0; i < bottom; i++ )
        memcpy(dst + (i + srcroi.height)*dststep, constBuf, dstroi.width);
}

}

#ifdef HAVE_OPENCL

namespace cv {

static bool ocl_copyMakeBorder( InputArray _src, OutputArray _dst, int top, int bottom,
                                int left, int right, int borderType, const Scalar& value )
{
    int type = _src.type(), cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    bool isolated = (borderType & BORDER_ISOLATED) != 0;
    borderType &= ~cv::BORDER_ISOLATED;

    if ( !(borderType == BORDER_CONSTANT || borderType == BORDER_REPLICATE || borderType == BORDER_REFLECT ||
           borderType == BORDER_WRAP || borderType == BORDER_REFLECT_101) ||
         cn > 4)
        return false;

    const char * const borderMap[] = { "BORDER_CONSTANT", "BORDER_REPLICATE", "BORDER_REFLECT", "BORDER_WRAP", "BORDER_REFLECT_101" };
    int scalarcn = cn == 3 ? 4 : cn;
    int sctype = CV_MAKETYPE(depth, scalarcn);
    String buildOptions = format("-D T=%s -D %s -D T1=%s -D cn=%d -D ST=%s -D rowsPerWI=%d",
                                 ocl::memopTypeToStr(type), borderMap[borderType],
                                 ocl::memopTypeToStr(depth), cn,
                                 ocl::memopTypeToStr(sctype), rowsPerWI);

    ocl::Kernel k("copyMakeBorder", ocl::core::copymakeborder_oclsrc, buildOptions);
    if (k.empty())
        return false;

    UMat src = _src.getUMat();
    if( src.isSubmatrix() && !isolated )
    {
        Size wholeSize;
        Point ofs;
        src.locateROI(wholeSize, ofs);
        int dtop = std::min(ofs.y, top);
        int dbottom = std::min(wholeSize.height - src.rows - ofs.y, bottom);
        int dleft = std::min(ofs.x, left);
        int dright = std::min(wholeSize.width - src.cols - ofs.x, right);
        src.adjustROI(dtop, dbottom, dleft, dright);
        top -= dtop;
        left -= dleft;
        bottom -= dbottom;
        right -= dright;
    }

    _dst.create(src.rows + top + bottom, src.cols + left + right, type);
    UMat dst = _dst.getUMat();

    if (top == 0 && left == 0 && bottom == 0 && right == 0)
    {
        if(src.u != dst.u || src.step != dst.step)
            src.copyTo(dst);
        return true;
    }

    k.args(ocl::KernelArg::ReadOnly(src), ocl::KernelArg::WriteOnly(dst),
           top, left, ocl::KernelArg::Constant(Mat(1, 1, sctype, value)));

    size_t globalsize[2] = { (size_t)dst.cols, ((size_t)dst.rows + rowsPerWI - 1) / rowsPerWI };
    return k.run(2, globalsize, NULL, false);
}

}
#endif

#ifdef HAVE_IPP
namespace cv {

static bool ipp_copyMakeBorder( Mat &_src, Mat &_dst, int top, int bottom,
                                int left, int right, int _borderType, const Scalar& value )
{
#if defined HAVE_IPP_IW_LL && !IPP_DISABLE_PERF_COPYMAKE
    CV_INSTRUMENT_REGION_IPP();

    ::ipp::IwiBorderSize borderSize(left, top, right, bottom);
    ::ipp::IwiSize       size(_src.cols, _src.rows);
    IppDataType          dataType   = ippiGetDataType(_src.depth());
    IppiBorderType       borderType = ippiGetBorderType(_borderType);
    if((int)borderType == -1)
        return false;

    if(_src.dims > 2)
        return false;

    Rect dstRect(borderSize.left, borderSize.top,
        _dst.cols - borderSize.right - borderSize.left,
        _dst.rows - borderSize.bottom - borderSize.top);
    Mat  subDst = Mat(_dst, dstRect);
    Mat *pSrc   = &_src;

    return CV_INSTRUMENT_FUN_IPP(llwiCopyMakeBorder, pSrc->ptr(), pSrc->step, subDst.ptr(), subDst.step, size, dataType, _src.channels(), borderSize, borderType, &value[0]) >= 0;
#else
    CV_UNUSED(_src); CV_UNUSED(_dst); CV_UNUSED(top); CV_UNUSED(bottom); CV_UNUSED(left); CV_UNUSED(right);
    CV_UNUSED(_borderType); CV_UNUSED(value);
    return false;
#endif
}
}
#endif

void cv::copyMakeBorder( InputArray _src, OutputArray _dst, int top, int bottom,
                         int left, int right, int borderType, const Scalar& value )
{
    CV_INSTRUMENT_REGION();

    CV_Assert( top >= 0 && bottom >= 0 && left >= 0 && right >= 0 && _src.dims() <= 2);

    CV_OCL_RUN(_dst.isUMat(),
               ocl_copyMakeBorder(_src, _dst, top, bottom, left, right, borderType, value))

    Mat src = _src.getMat();
    int type = src.type();

    if( src.isSubmatrix() && (borderType & BORDER_ISOLATED) == 0 )
    {
        Size wholeSize;
        Point ofs;
        src.locateROI(wholeSize, ofs);
        int dtop = std::min(ofs.y, top);
        int dbottom = std::min(wholeSize.height - src.rows - ofs.y, bottom);
        int dleft = std::min(ofs.x, left);
        int dright = std::min(wholeSize.width - src.cols - ofs.x, right);
        src.adjustROI(dtop, dbottom, dleft, dright);
        top -= dtop;
        left -= dleft;
        bottom -= dbottom;
        right -= dright;
    }

    _dst.create( src.rows + top + bottom, src.cols + left + right, type );
    Mat dst = _dst.getMat();

    if(top == 0 && left == 0 && bottom == 0 && right == 0)
    {
        if(src.data != dst.data || src.step != dst.step)
            src.copyTo(dst);
        return;
    }

    borderType &= ~BORDER_ISOLATED;

    CV_IPP_RUN_FAST(ipp_copyMakeBorder(src, dst, top, bottom, left, right, borderType, value))

    if( borderType != BORDER_CONSTANT )
        copyMakeBorder_8u( src.ptr(), src.step, src.size(),
                           dst.ptr(), dst.step, dst.size(),
                           top, left, (int)src.elemSize(), borderType );
    else
    {
        int cn = src.channels(), cn1 = cn;
        AutoBuffer<double> buf(cn);
        if( cn > 4 )
        {
            CV_Assert( value[0] == value[1] && value[0] == value[2] && value[0] == value[3] );
            cn1 = 1;
        }
        scalarToRawData(value, buf.data(), CV_MAKETYPE(src.depth(), cn1), cn);
        copyMakeConstBorder_8u( src.ptr(), src.step, src.size(),
                                dst.ptr(), dst.step, dst.size(),
                                top, left, (int)src.elemSize(), (uchar*)buf.data() );
    }
}

/* dst = src */
CV_IMPL void
cvCopy( const void* srcarr, void* dstarr, const void* maskarr )
{
    if( CV_IS_SPARSE_MAT(srcarr) && CV_IS_SPARSE_MAT(dstarr))
    {
        CV_Assert( maskarr == 0 );
        CvSparseMat* src1 = (CvSparseMat*)srcarr;
        CvSparseMat* dst1 = (CvSparseMat*)dstarr;
        CvSparseMatIterator iterator;
        CvSparseNode* node;

        dst1->dims = src1->dims;
        memcpy( dst1->size, src1->size, src1->dims*sizeof(src1->size[0]));
        dst1->valoffset = src1->valoffset;
        dst1->idxoffset = src1->idxoffset;
        cvClearSet( dst1->heap );

        if( src1->heap->active_count >= dst1->hashsize*CV_SPARSE_HASH_RATIO )
        {
            cvFree( &dst1->hashtable );
            dst1->hashsize = src1->hashsize;
            dst1->hashtable =
                (void**)cvAlloc( dst1->hashsize*sizeof(dst1->hashtable[0]));
        }

        memset( dst1->hashtable, 0, dst1->hashsize*sizeof(dst1->hashtable[0]));

        for( node = cvInitSparseMatIterator( src1, &iterator );
             node != 0; node = cvGetNextSparseNode( &iterator ))
        {
            CvSparseNode* node_copy = (CvSparseNode*)cvSetNew( dst1->heap );
            int tabidx = node->hashval & (dst1->hashsize - 1);
            memcpy( node_copy, node, dst1->heap->elem_size );
            node_copy->next = (CvSparseNode*)dst1->hashtable[tabidx];
            dst1->hashtable[tabidx] = node_copy;
        }
        return;
    }
    cv::Mat src = cv::cvarrToMat(srcarr, false, true, 1), dst = cv::cvarrToMat(dstarr, false, true, 1);
    CV_Assert( src.depth() == dst.depth() && src.size == dst.size );

    int coi1 = 0, coi2 = 0;
    if( CV_IS_IMAGE(srcarr) )
        coi1 = cvGetImageCOI((const IplImage*)srcarr);
    if( CV_IS_IMAGE(dstarr) )
        coi2 = cvGetImageCOI((const IplImage*)dstarr);

    if( coi1 || coi2 )
    {
        CV_Assert( (coi1 != 0 || src.channels() == 1) &&
            (coi2 != 0 || dst.channels() == 1) );

        int pair[] = { std::max(coi1-1, 0), std::max(coi2-1, 0) };
        cv::mixChannels( &src, 1, &dst, 1, pair, 1 );
        return;
    }
    else
        CV_Assert( src.channels() == dst.channels() );

    if( !maskarr )
        src.copyTo(dst);
    else
        src.copyTo(dst, cv::cvarrToMat(maskarr));
}

CV_IMPL void
cvSet( void* arr, CvScalar value, const void* maskarr )
{
    cv::Mat m = cv::cvarrToMat(arr);
    if( !maskarr )
        m = value;
    else
        m.setTo(cv::Scalar(value), cv::cvarrToMat(maskarr));
}

CV_IMPL void
cvSetZero( CvArr* arr )
{
    if( CV_IS_SPARSE_MAT(arr) )
    {
        CvSparseMat* mat1 = (CvSparseMat*)arr;
        cvClearSet( mat1->heap );
        if( mat1->hashtable )
            memset( mat1->hashtable, 0, mat1->hashsize*sizeof(mat1->hashtable[0]));
        return;
    }
    cv::Mat m = cv::cvarrToMat(arr);
    m = cv::Scalar(0);
}

CV_IMPL void
cvFlip( const CvArr* srcarr, CvArr* dstarr, int flip_mode )
{
    cv::Mat src = cv::cvarrToMat(srcarr);
    cv::Mat dst;

    if (!dstarr)
      dst = src;
    else
      dst = cv::cvarrToMat(dstarr);

    CV_Assert( src.type() == dst.type() && src.size() == dst.size() );
    cv::flip( src, dst, flip_mode );
}

CV_IMPL void
cvRepeat( const CvArr* srcarr, CvArr* dstarr )
{
    cv::Mat src = cv::cvarrToMat(srcarr), dst = cv::cvarrToMat(dstarr);
    CV_Assert( src.type() == dst.type() &&
        dst.rows % src.rows == 0 && dst.cols % src.cols == 0 );
    cv::repeat(src, dst.rows/src.rows, dst.cols/src.cols, dst);
}

/* End of file. */
