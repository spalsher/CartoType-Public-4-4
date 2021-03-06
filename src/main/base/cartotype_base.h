/*
CARTOTYPE_BASE.H
Copyright (C) 2004-2017 CartoType Ltd.
See www.cartotype.com for more information.
*/

#ifndef CARTOTYPE_BASE_H__
#define CARTOTYPE_BASE_H__

#include "cartotype_errors.h"

#include <memory>
#include <vector>
#include <cmath>

namespace CartoType
{

class TRectFP;

/** A point in two-dimensional space. */
class TPoint
    {
    public:
    /** Create a point with coordinates (0,0). */
    TPoint() { }
    /** Create a point, specifying coordinates. */
    TPoint(int32 aX,int32 aY): iX(aX), iY(aY) { }
    /** The equality operator. */
    bool operator==(const TPoint& aPoint) const { return iX == aPoint.iX && iY == aPoint.iY; }
    /** The inequality operator. */
    bool operator!=(const TPoint& aPoint) const { return !(*this == aPoint); }
    /** Offset a point by another point, treated as a positive vector. */
    void operator+=(const TPoint& aPoint) { iX += aPoint.iX; iY += aPoint.iY; }
    /** Offset a point by another point, treated as a negative vector. */
    void operator-=(const TPoint& aPoint) { iX -= aPoint.iX; iY -= aPoint.iY; }
    /** The less-than operator, ordering points by x coordinate, then y coordinate. */
    bool operator<(const TPoint& aPoint) const
        {
        return (iX < aPoint.iX) ||
               ((iX == aPoint.iX) && iY < aPoint.iY);
        }
    /** The greater-than operator, ordering points by x coordinate, then y coordinate. */
    bool operator>(const TPoint& aPoint) const
        {
        return (iX > aPoint.iX) ||
               ((iX == aPoint.iX) && iY > aPoint.iY);
        }

    /** Return a single number which can be used to sort points into order. */
    uint64 ComparisonValue() const { return (uint64(iX) << 32) + uint64(iY); }

    /** The x coordinate. */
    int32 iX { 0 };
    /** The y coordinate. */
    int32 iY { 0 };
    };

/**
A templated 2D point or vector class.
In the vector functions, left and right are defined using x increasing right and y increasing up.
*/
template<class T> class TPoint2
    {
    public:
    /** Create a point with coordinates (0,0). */
    TPoint2() { }
    /** Create a point, specifying coordinates. */
    TPoint2(T aX,T aY): iX(aX), iY(aY) { }
    /** Create a point from a TPoint object. */
    TPoint2(const TPoint& aPoint): iX(aPoint.iX), iY(aPoint.iY) { }
    /** The equality operator. */
    bool operator==(const TPoint2<T>& aPoint) const { return iX == aPoint.iX && iY == aPoint.iY; }
    /** The inequality operator. */
    bool operator!=(const TPoint2<T>& aPoint) const { return !(*this == aPoint); }
    /** Offset a point by another point, treated as a positive vector. */
    void operator+=(const TPoint2<T>& aPoint) { iX += aPoint.iX; iY += aPoint.iY; }
    /** Offset a point by another point, treated as a negative vector. */
    void operator-=(const TPoint2<T>& aPoint) { iX -= aPoint.iX; iY -= aPoint.iY; }
    /** Scale a point by a factor. */
    void operator*=(T aFactor) { iX *= aFactor; iY *= aFactor; }
    /** The less-than operator, ordering points by x coordinate, then y coordinate. */
    bool operator<(const TPoint2<T>& aPoint) const
        {
        if (iX < aPoint.iX)
            return true;
        if (iX > aPoint.iX)
            return false;
        if (iY < aPoint.iY)
            return true;
        return false;
        }
    /** The greater-than operator, ordering points by x coordinate, then y coordinate. */
    bool operator>(const TPoint2<T>& aPoint) const
        {
        if (iX > aPoint.iX)
            return true;
        if (iX < aPoint.iX)
            return false;
        if (iY > aPoint.iY)
            return true;
        return false;
        }
    T VectorLength() const
        {
        return sqrt(iX * iX + iY * iY);
        }
    T CrossProduct(const TPoint2<T>& aOther) const
        {
        return iX * aOther.iY - iY * aOther.iX;
        }
    TPoint2 UnitVector() const
        {
        T length = VectorLength();
        if (length > 0)
            {
            TPoint2<T> p(*this);
            p.iX /= length;
            p.iY /= length;
            return p;
            }
        return TPoint2<T>(1,0);
        }
    
    /** Returns a unit vector pointing 90 degrees left of this vector. */
    TPoint2 LeftUnitVector() const
        {
        TPoint2<T> u { UnitVector() };
        return TPoint2<T>(-u.iY,u.iX);
        }
    /** Returns a unit vector pointing 90 degrees right of this vector. */
    TPoint2 RightUnitVector() const
        {
        TPoint2<T> u { UnitVector() };
        return TPoint2<T>(u.iY,-u.iX);
        }
    /** Returns true if aPoint is left of this vector. */
    bool LeftOfVector(const TPoint2<T>& aPoint) const
        {
        return CrossProduct(aPoint) > 0;
        }
    /** Returns true if aPoint is right of this vector. */
    bool RightOfVector(const TPoint2<T>& aPoint) const
        {
        return CrossProduct(aPoint) < 0;
        }

    /** The x coordinate. */
    T iX { 0 };
    /** The y coordinate. */
    T iY { 0 };
    };

/** A templated 3D point class. */
template<class T> class TPoint3
    {
    public:
    /** Create a point with coordinates (0,0,0). */
    TPoint3() { }
    /** Create a point, specifying coordinates. */
    TPoint3(T aX,T aY,T aZ): iX(aX), iY(aY), iZ(aZ) { }
    /** Create a 3D point from a 2D point. */
    TPoint3(TPoint2<T> aPoint): iX(aPoint.iX), iY(aPoint.iY), iZ(0) { }
    /** The equality operator. */
    bool operator==(const TPoint3<T>& aPoint) const { return iX == aPoint.iX && iY == aPoint.iY && iZ == aPoint.iZ; }
    /** The inequality operator. */
    bool operator!=(const TPoint3<T>& aPoint) const { return !(*this == aPoint); }
    /** Offset a point by another point, treated as a positive vector. */
    void operator+=(const TPoint3<T>& aPoint) { iX += aPoint.iX; iY += aPoint.iY; iZ += aPoint.iZ; }
    /** Offset a point by another point, treated as a negative vector. */
    void operator-=(const TPoint3<T>& aPoint) { iX -= aPoint.iX; iY -= aPoint.iY; iZ -= aPoint.iZ; }
    /** The less-than operator, ordering points by x coordinate, then y coordinate, then z coordinate. */
    bool operator<(const TPoint3<T>& aPoint) const
        {
        if (iX < aPoint.iX)
            return true;
        if (iX > aPoint.iX)
            return false;
        if (iY < aPoint.iY)
            return true;
        if (iY > aPoint.iY)
            return false;
        if (iZ < aPoint.iZ)
            return true;
        return false;
        }
    /** The greater-than operator, ordering points by x coordinate, then y coordinate. */
    bool operator>(const TPoint2<T>& aPoint) const
        {
        if (iX > aPoint.iX)
            return true;
        if (iX < aPoint.iX)
            return false;
        if (iY > aPoint.iY)
            return true;
        if (iY < aPoint.iY)
            return false;
        if (iZ > aPoint.iZ)
            return true;
        return false;
        }
    T VectorLength() const
        {
        return sqrt(iX * iX + iY * iY + iZ * iZ);
        }
    TPoint3 UnitVector() const
        {
        T length = VectorLength();
        if (length > 0)
            {
            TPoint3 p(*this);
            p.iX /= length;
            p.iY /= length;
            p.iZ /= length;
            return p;
            }
        return TPoint3<T>(1,0,0);
        }

    /** The x coordinate. */
    T iX { 0 };
    /** The y coordinate. */
    T iY { 0 };
    /** The z coordinate. */
    T iZ { 0 };
    };

/** A double-precision floating-point 2D point class. */
typedef class TPoint2<double> TPointFP;

/** A double-precision floating-point 3D point class. */
typedef class TPoint3<double> TPoint3FP;

/** A set of modifiable coordinate pairs. */
class TWritableCoordSet
    {
    public:
    TWritableCoordSet() { } // define an explicit constructor to prevent aggregate initialisation, which stops braced initialisation from CGeometry working

    /** A pointer to the first x coordinate. */
    double* iX { nullptr };
    /** A pointer to the first y coordinate. */
    double* iY { nullptr };
    /** The step by which to increment iX and iY. */
    size_t iStep { 1 };
    /** The number of coordinates. */
    size_t iCount { 0 };
    };

/** A set of coordinate pairs. */
class TCoordSet
    {
    public:
    /** Creates an empty coordinate set. */
    TCoordSet() { }

    /** Creates a coordinate set from a writable coordinate set. */
    TCoordSet(const TWritableCoordSet& aOther):
        iX(aOther.iX),
        iY(aOther.iY),
        iStep(aOther.iStep),
        iCount(aOther.iCount)
        {
        }

    TCoordSet(const std::vector<TPointFP>& aPointArray):
        iX(&aPointArray.front().iX),
        iY(&aPointArray.front().iY),
        iStep(2),
        iCount(aPointArray.size())
        {
        }

    double DistanceFromPoint(bool aIsPolygon,double aX,double aY,double& aNearestX,double& aNearestY);
    bool PolygonContains(double aX,double aY) const;
    double X(size_t aIndex) const;
    double Y(size_t aIndex) const;

    /** A pointer to the first x coordinate. */
    const double* iX { nullptr };
    /** A pointer to the first y coordinate. */
    const double* iY { nullptr };
    /** The step by which to increment iX and iY. */
    size_t iStep { 1 };
    /** The number of coordinates. */
    size_t iCount { 0 };
    };

/** A set of coordinates consisting of a single point. */
class TCoordPair: public TCoordSet
    {
    public:
    TCoordPair(double aX,double aY):
        iOwnX(aX),
        iOwnY(aY)
        {
        iX = &iOwnX;
        iY = &iOwnY;
        iStep = 0;
        iCount = 1;
        }
        
    double iOwnX, iOwnY;
    };

/** A set of coordinates consising of two points. */
class TCoordSetOfTwoPoints: public TCoordSet
    {
    public:
    TCoordSetOfTwoPoints(double aX0,double aY0,double aX1,double aY1)
        {
        iOwnX[0] = aX0;
        iOwnX[1] = aX1;
        iOwnY[0] = aY0;
        iOwnY[1] = aY1;
        iX = iOwnX;
        iY = iOwnY;
        iStep = 1;
        iCount = 2;
        }

    double iOwnX[2];
    double iOwnY[2];
    };

/** Coordinate types. */
enum class TCoordType
    {
    /** Longitude (x) and latitude (y) in degrees. */
    Degree,
    /** Pixels on the display: x increases to the right and y increases downwards. */
    Display,
    /** A synonym for Display: pixels on the display: x increases to the right and y increases downwards.. */
    Screen = Display,
    /** Map coordinates: projected meters, 32nds of meters or 64ths of meters, depending on the map. */
    Map,
    /** Map meters: projected meters, not correcting for distortion introduced by the projection. */
    MapMeter
    };

/** A coordinate type synonym for backward compatibility. @deprecated: use TCoordType::Degree. */
constexpr TCoordType EDegreeCoordType = TCoordType::Degree;
/** A coordinate type synonym for backward compatibility. @deprecated: use TCoordType::Display. */
constexpr TCoordType EScreenCoordType = TCoordType::Display;
/** A coordinate type synonym for backward compatibility. @deprecated: use TCoordType::Map. */
constexpr TCoordType EMapCoordType = TCoordType::Map;
/** A coordinate type synonym for backward compatibility. @deprecated: use TCoordType::MapMeter. */
constexpr TCoordType EMapMeterCoordType = TCoordType::MapMeter;
/** A coordinate type synonym for backward compatibility. @deprecated: use TCoordType::Degree. */
constexpr TCoordType ELatLongCoordType = TCoordType::Degree;

/** A line in two-dimensional space. */
class TLine
    {
    public:
    TPoint iStart;
    TPoint iEnd;
    };

/** Types used by TOutlinePoint. */
enum class TPointType
    {
    /** A point on the curve. */
    OnCurve,
    /** A control point for a quadratic (conic) Bezier spline curve. */
    Quadratic,
    /** A control point for a cubic Bezier spline curve. */
    Cubic
    };

/** A point type synonym for backward compatibility. @deprecated: use TPointType::OnCurve. */
constexpr TPointType EOnCurvePoint = TPointType::OnCurve;
/** A point type synonym for backward compatibility. @deprecated: use TPointType::Quadratic. */
constexpr TPointType EQuadraticControlPoint = TPointType::Quadratic;
/** A point type synonym for backward compatibility. @deprecated: use TPointType::Cubic. */
constexpr TPointType ECubicControlPoint = TPointType::Cubic;

/** A point on a path. Paths are made from straight line segments and Bezier splines. */
class TOutlinePoint: public TPoint
    {
    public:
    /** Create an on-curve outline point with the coordinates (0,0). */
    TOutlinePoint() { }
    /** Create an on-curve outline point from a TPoint. */
    TOutlinePoint(const TPoint& aPoint): TPoint(aPoint) { }
    /** Create an on-curve outline point from a TPoint and a type. */
    TOutlinePoint(const TPoint& aPoint,TPointType aType): TPoint(aPoint), iType(aType) { }
    /** Create an outline point, specifying coordinates and type. */
    TOutlinePoint(int32 aX,int32 aY,TPointType aType): TPoint(aX,aY), iType(aType) { }
    /** The equality operator. */
    bool operator==(const TOutlinePoint& aPoint) const { return iX == aPoint.iX && iY == aPoint.iY && iType == aPoint.iType; }
    /** The inequality operator. */
    bool operator!=(const TOutlinePoint& aPoint) const { return !(*this == aPoint); }

    /** The type of an outline point: on-curve, or a quadratic or cubic control point. */
    TPointType iType { TPointType::OnCurve };
    };

/**
A rectangle in two-dimensional space, aligned with the coordinate system and defined by its
top-left and bottom-right corners.
*/
class TRect
    {
    public:
    /** Create an empty rectangle with both corners at the point (0,0). */
    TRect() { }
    /** Create a rectangle with the specified edges. */ 
    TRect(int32 aLeft,int32 aTop,int32 aRight,int32 aBottom):
        iTopLeft(aLeft,aTop),
        iBottomRight(aRight,aBottom)
        {
        }
    explicit TRect(const TRectFP& aRectFP);
    /** The equality operator. */
    bool operator==(const TRect& aRect) const { return iTopLeft == aRect.iTopLeft && iBottomRight == aRect.iBottomRight; }
    /** The inequality operator. */
    bool operator!=(const TRect& aRect) const { return !(*this == aRect); }
    /** Return the left edge. */
    int32 Left() const { return iTopLeft.iX; }
    /** Return the top edge. */
    int32 Top() const { return iTopLeft.iY; }
    /** Return the right edge. */
    int32 Right() const { return iBottomRight.iX; }
    /** Return the bottom edge. */
    int32 Bottom() const { return iBottomRight.iY; }
    /**
    Return true if the rectangle is empty, defined as having a width or height
    less than or equal to zero.
    */
    bool IsEmpty() const { return Left() >= Right() || Top() >= Bottom(); }
    /**
    Return true if the rectangle is maximal, defined as having left and top of INT32_MIN
    and bottom and right of INT32_MAX.
    */
    bool IsMaximal() const { return iTopLeft.iX == INT32_MIN && iTopLeft.iY == INT32_MIN && iBottomRight.iX == INT32_MAX && iBottomRight.iY == INT32_MAX; }
    /** Return the width. */
    int32 Width() const { return Right() - Left(); }
    /** Return the height. */
    int32 Height() const { return Bottom() - Top(); }
    /** Return the top right corner. */
    TPoint TopRight() const { return TPoint(iBottomRight.iX,iTopLeft.iY); }
    /** Return the bottom left corner. */
    TPoint BottomLeft() const { return TPoint(iTopLeft.iX,iBottomRight.iY); }
    /** Return the center. */
    TPoint Center() const { return TPoint((iTopLeft.iX + iBottomRight.iX) / 2,(iTopLeft.iY + iBottomRight.iY) / 2); }
    /**
    Return true if the rectangle contains the point, where containment is defined
    using half-open intervals: the rectangle includes points on its top and
    left edges but not its right and bottom edges.
    */
    bool Contains(const TPoint& aPoint) const
        { return aPoint.iX >= iTopLeft.iX && aPoint.iY >= iTopLeft.iY &&
                 aPoint.iX < iBottomRight.iX && aPoint.iY < iBottomRight.iY; }
    /** Return true if the rectangle contains another rectangle. */
    bool Contains(const TRect& aRect) const
        {
        return iTopLeft.iX <= aRect.iTopLeft.iX &&
               iTopLeft.iY <= aRect.iTopLeft.iY &&
               iBottomRight.iX >= aRect.iBottomRight.iX &&
               iBottomRight.iY >= aRect.iBottomRight.iY;
        }
    bool IsOnEdge(const TPoint& aPoint) const;
    bool Intersects(const TRect& aRect) const;
    bool Intersects(const TPoint& aStart,const TPoint& aEnd,
                    TPoint* aIntersectionStart = nullptr,TPoint* aIntersectionEnd = nullptr) const;
    void Intersection(const TRect& aRect);
    void Combine(const TRect& aRect);
    void Combine(const TPoint& aPoint);

    /** Constants for bits combined to make a return value for Region. */
    static constexpr unsigned KLeftRegion = 1;
    static constexpr unsigned KRightRegion = 1;
    static constexpr unsigned KTopRegion = 1;
    static constexpr unsigned KBottomRegion = 1;
    static constexpr unsigned KTopLeftRegion = KTopRegion | KLeftRegion;
    static constexpr unsigned KTopRightRegion = KTopRegion | KRightRegion;
    static constexpr unsigned KBottomLeftRegion = KBottomRegion | KLeftRegion;
    static constexpr unsigned KBottomRightRegion = KBottomRegion | KRightRegion;

    /**
    Return the region of a point relative to a rectangle.
    A rectangle defines nine possible regions. Region 0 is inside the rectangle,
    and is only returned if the point is inside and doesn't touch the boundary,
    which is useful when clipping paths to a rectangle.
    */
    int32 Region(const TPoint& aPoint) const
        {
        int32 region;
        if (aPoint.iX <= iTopLeft.iX)
            region = KLeftRegion;
        else if (aPoint.iX >= iBottomRight.iX)
            region = KRightRegion;
        else
            region = 0;
        if (aPoint.iY <= iTopLeft.iY)
            region |= KTopRegion;
        else if (aPoint.iY >= iBottomRight.iY)
            region |= KBottomRegion;
        return region;
        }


    /** The top left corner. */
    TPoint iTopLeft;
    /** The bottom right corner. */
    TPoint iBottomRight;
    };

/**
A floating-point rectangle in two-dimensional space, aligned with the coordinate system and defined by its
top-left (minimum coords) and bottom-right (maximum coords) corners.

The terms top-left and bottom-right are misleading but are retained for compatibility with TRect and with
existing CartoType practice.
*/
class TRectFP
    {
    public:
    /** Create an empty rectangle with both corners at the point (0,0). */
    TRectFP() { }
    /** Create a rectangle with the specified edges. */ 
    TRectFP(double aMinX,double aMinY,double aMaxX,double aMaxY):
        iTopLeft(aMinX,aMinY),
        iBottomRight(aMaxX,aMaxY)
        {
        }
    /** Create a rectangle from an integer rectangle. */
    TRectFP(const TRect& aRect):
        iTopLeft(aRect.iTopLeft),
        iBottomRight(aRect.iBottomRight)
        {
        }
    /** The equality operator. */
    bool operator==(const TRectFP& aRect) const { return iTopLeft == aRect.iTopLeft && iBottomRight == aRect.iBottomRight; }
    /** The inequality operator. */
    bool operator!=(const TRectFP& aRect) const { return !(*this == aRect); }
    /** Return the left edge. */
    double Left() const { return iTopLeft.iX; }
    /** Return the top edge. */
    double Top() const { return iTopLeft.iY; }
    /** Return the right edge. */
    double Right() const { return iBottomRight.iX; }
    /** Return the bottom edge. */
    double Bottom() const { return iBottomRight.iY; }
    /**
    Return true if the rectangle is empty, defined as having a width or height
    less than or equal to zero.
    */
    bool IsEmpty() const { return Left() >= Right() || Top() >= Bottom(); }
    /** Return the width. */
    double Width() const { return Right() - Left(); }
    /** Return the height. */
    double Height() const { return Bottom() - Top(); }
    /** Return the top right corner. */
    TPointFP TopRight() const { return TPointFP(iBottomRight.iX,iTopLeft.iY); }
    /** Return the bottom left corner. */
    TPointFP BottomLeft() const { return TPointFP(iTopLeft.iX,iBottomRight.iY); }
    /** Return the center. */
    TPointFP Center() const { return TPointFP((iTopLeft.iX + iBottomRight.iX) / 2.0,(iTopLeft.iY + iBottomRight.iY) / 2.0); }
    /**
    Return true if the rectangle contains the point, where containment is defined
    using half-open intervals: the rectangle includes points on its top and
    left edges but not its right and bottom edges.
    */
    bool Contains(const TPointFP& aPoint) const
        { return aPoint.iX >= iTopLeft.iX && aPoint.iY >= iTopLeft.iY &&
                 aPoint.iX < iBottomRight.iX && aPoint.iY < iBottomRight.iY; }
    /** Return true if the rectangle contains another rectangle. */
    bool Contains(const TRectFP& aRect) const
        {
        return iTopLeft.iX <= aRect.iTopLeft.iX &&
               iTopLeft.iY <= aRect.iTopLeft.iY &&
               iBottomRight.iX >= aRect.iBottomRight.iX &&
               iBottomRight.iY >= aRect.iBottomRight.iY;
        }
    /** Update a rectangle such that it contains the specified point. */
    void Combine(const TPointFP& aPoint)
        {
        if (aPoint.iX < iTopLeft.iX)
            iTopLeft.iX = aPoint.iX;
        if (aPoint.iY < iTopLeft.iY)
            iTopLeft.iY = aPoint.iY;
        if (aPoint.iX > iBottomRight.iX)
            iBottomRight.iX = aPoint.iX;
        if (aPoint.iY > iBottomRight.iY)
            iBottomRight.iY = aPoint.iY;
        }
    /** Set a rectangle to the smallest new rectangle that contains itself and aRect. */
    void Combine(const TRectFP& aRect)
        {
        if (!aRect.IsEmpty())
            {
            if (IsEmpty())
                *this = aRect;
            else
                {
                if (aRect.iTopLeft.iX < iTopLeft.iX)
                    iTopLeft.iX = aRect.iTopLeft.iX;
                if (aRect.iTopLeft.iY < iTopLeft.iY)
                    iTopLeft.iY = aRect.iTopLeft.iY;
                if (aRect.iBottomRight.iX > iBottomRight.iX)
                    iBottomRight.iX = aRect.iBottomRight.iX;
                if (aRect.iBottomRight.iY > iBottomRight.iY)
                    iBottomRight.iY = aRect.iBottomRight.iY;
                }
            }
        }
    /** Set a rectangle to its intersection with aRect. */
    void Intersection(const TRectFP& aRect)
        {
        if (iTopLeft.iX < aRect.iTopLeft.iX)
            iTopLeft.iX = aRect.iTopLeft.iX;
        if (iBottomRight.iX > aRect.iBottomRight.iX)
            iBottomRight.iX = aRect.iBottomRight.iX;
        if (iTopLeft.iY < aRect.iTopLeft.iY)
            iTopLeft.iY = aRect.iTopLeft.iY;
        if (iBottomRight.iY > aRect.iBottomRight.iY)
            iBottomRight.iY = aRect.iBottomRight.iY;
        }
    /**
    Return true if the rectangle and aRect have an intersection.
    If both rectangles are non-empty, return true only
    if the intersection is non-empy.
    */
    bool Intersects(const TRectFP& aRect) const
        {
        return iTopLeft.iX < aRect.iBottomRight.iX &&
               iBottomRight.iX > aRect.iTopLeft.iX &&
               iTopLeft.iY < aRect.iBottomRight.iY &&
               iBottomRight.iY > aRect.iTopLeft.iY;
        }

    /** The top left corner. */
    TPointFP iTopLeft;
    /** The bottom right corner. */
    TPointFP iBottomRight;
    };

/**
A templated interface class defining dictionaries in
which a key type is used to access a value.
*/
template <class Key,class Value> class MDictionary
    {
    public:
    /**
    Although pointers to this class are not owned,
    a virtual destructor is provided for safety and
    to avoid compiler warnings.
    */
    virtual ~MDictionary() { }
    
    /**
    Return true if aKey exists, false if not.
    If aKey exists in the dictionary, set aValue to
    the value associated with aKey.
    */
    virtual bool Find(const Key& aKey,Value& aValue) const = 0;
    };

/**
A templated interface class defining dictionaries in
which a key is used to access a pointer to a value.
*/
template <class Key,class Value> class MPointerDictionary
    {
    public:
    /**
    Although pointers to this class are not owned,
    a virtual destructor is provided for safety and
    to avoid compiler warnings.
    */
    virtual ~MPointerDictionary() { }

    /**
    Return true if aKey exists, false if not.
    If aKey exists in the dictionary, set aValue to point to
    the value associated with aKey.
    */
    virtual bool Find(const Key& aKey,const Value*& aValue) const = 0;
    };

/** File types of interest to CartoType. */
enum class TFileType
    {
    /** PNG (Portable Network Graphics) image files. */
    PNG,
    /** JPEG (Joint Photographic Expert Group) image files. */
    JPEG,
    /** TIFF (Tagged Image File Format) image files. */
    TIFF,
    /** CTM1 (CartoType Map Data Type 1) map data files. */
    CTM1,
    /** CTMS (CartoType Map Data, Serialized) map data files. */
    CTMS,
    /** KML (Keyhole Markup Language) map data files. */
    KML,
    /** CTSQL (CartoType SQLite format) map data files. */
    CTSQL,
    /** CTROUTE (CartoType XML route) files. */
    CTROUTE,
    /** GPX (GPS Exchange) files. */
    GPX
    };

/** A file type synonym for backward compatibility. @deprecated: use TFileType::PNG. */
constexpr TFileType EPngFileType = TFileType::PNG;
/** A file type synonym for backward compatibility. @deprecated: use TFileType::JPEG. */
constexpr TFileType EJpegFileType = TFileType::JPEG;
/** A file type synonym for backward compatibility. @deprecated: use TFileType::TIFF.*/
constexpr TFileType ETiffFileType = TFileType::TIFF;
/** A file type synonym for backward compatibility. @deprecated: use TFileType::CTM1. */
constexpr TFileType ECtm1FileType = TFileType::CTM1;
/** A file type synonym for backward compatibility. @deprecated: use TFileType::CTMS. */
constexpr TFileType ECtmsFileType = TFileType::CTMS;
/** A file type synonym for backward compatibility. @deprecated: use TFileType::KML. */
constexpr TFileType EKmlFileType = TFileType::KML;
/** A file type synonym for backward compatibility. @deprecated: use TFileType::CTSQL. */
constexpr TFileType ECtsqlFileType = TFileType::CTSQL;
/** A file type synonym for backward compatibility. @deprecated: use TFileType::CTROUTE. */
constexpr TFileType ECartoTypeRouteFileType = TFileType::CTROUTE;
/** A file type synonym for backward compatibility. @deprecated: use TFileType::GPX. */
constexpr TFileType EGpxFileType = TFileType::GPX;

/** A file location: used for returning the location of an XML or other parsing error. */
class TFileLocation
    {
    public:
    size_t iColumnNumber { 0 };
    size_t iLineNumber { 0 };
    size_t iByteIndex { 0 };
    };

/** Types of writable maps. */
enum class TWritableMapType
    {
    /** A writable map stored in memory. */
    Memory,
    
    /** A map stored as an SQLite database. */
    SQLite,
    
    /** Not a writable map type but the number of different writable map types. */
    Count,
    
    /** A value to use where a null type is needed. */
    None = -1
    };

/** A writable map type synonym for backward compatibility. @deprecated: use TWritableMapType::Memory. */
constexpr TWritableMapType EMemoryWritableMapType = TWritableMapType::Memory;
/** A writable map type synonym for backward compatibility. @deprecated: use TWritableMapType::SQLite. */
constexpr TWritableMapType ESQLiteWritableMapType = TWritableMapType::SQLite;
/** A writable map type synonym for backward compatibility. @deprecated: use TWritableMapType::Count. */
constexpr TWritableMapType EWritableMapTypeCount = TWritableMapType::Count;
/** A writable map type synonym for backward compatibility. @deprecated: use TWritableMapType::None. */
constexpr TWritableMapType ENoWritableMapType = TWritableMapType::None;

/** An enumerated type for map objects. */
enum class TMapObjectType
    {
    /** A point in two-dimensional space. */
    Point,

    /** A line in two-dimensional space, consisting of one or more open contours. */
    Line,

    /** A polygon in two-dimensional space, consisting of one or more closed contours. */
    Polygon,

    /** Information arranged as an array of values, such as an image or height data. */
    Array,

    /** The total number of map object types. This is not an actual type. */
    Count,

    /** A value to use where a null type is needed. */
    None = -1
    };

/** A map object type synonym for backward compatibility. @deprecated: use TMapObjectType::Point. */
constexpr TMapObjectType EPointObject = TMapObjectType::Point;
/** A map object type synonym for backward compatibility. @deprecated: use TMapObjectType::Line. */
constexpr TMapObjectType ELineObject = TMapObjectType::Line;
/** A map object type synonym for backward compatibility. @deprecated: use TMapObjectType::Polygon. */
constexpr TMapObjectType EPolygonObject = TMapObjectType::Polygon;
/** A map object type synonym for backward compatibility. @deprecated: use TMapObjectType::Array. */
constexpr TMapObjectType EArrayObject = TMapObjectType::Array;
/** A map object type synonym for backward compatibility. @deprecated: use TMapObjectType::Count. */
constexpr TMapObjectType EMapObjectTypes = TMapObjectType::Count;
/** A map object type synonym for backward compatibility. @deprecated: use TMapObjectType::None. */
constexpr TMapObjectType ENoObjectType = TMapObjectType::None;

/** Values used in the user-data value of polygon objects in the vector tile drawing system. */
enum class TPolygonClipType
    {
    /** This polygon can be drawn normally, */
    Standard,
    /** This polygon has been clipped and only the interior should be drawn, not the border. */
    Fill,
    /** This is a polyline representing the clipped border of a polygon. */
    Border
    };

/** A holder for arbitrary data. */
class MUserData
    {
    public:
    virtual ~MUserData() { }
    };

/** The maximum length for a map label in characters. */
constexpr int32 KMaxLabelLength = 128;

/**
A constant to convert radians to metres for the Mercator and other projections.
It is the semi-major axis (equatorial radius) used by the WGS 84 datum (see http://en.wikipedia.org/wiki/WGS84).
*/
constexpr int32 KEquatorialRadiusInMetres = 6378137;

constexpr double KRadiansToMetres = KEquatorialRadiusInMetres;
constexpr double KPiDouble = 3.141592654;
constexpr float KPiFloat = 3.141592654f;
constexpr double KRadiansToDegreesDouble = 180.0 / KPiDouble;
constexpr double KDegreesToRadiansDouble = KPiDouble / 180.0;

/** Find the great-circle distance in metres, assuming a spherical earth, between two lat-long points in degrees. */
inline double GreatCircleDistanceInMeters(double aLong1,double aLat1,double aLong2,double aLat2)
    {
    aLong1 *= KDegreesToRadiansDouble;
    aLat1 *= KDegreesToRadiansDouble;
    aLong2 *= KDegreesToRadiansDouble;
    aLat2 *= KDegreesToRadiansDouble;
    double cos_angle = std::sin(aLat1) * std::sin(aLat2) + std::cos(aLat1) * std::cos(aLat2) * std::cos(aLong2 - aLong1);
    
    /*
    Inaccurate trig functions can cause cos_angle to be a tiny amount greater than 1 if the two positions are very close.
    That in turn causes acos to gives a domain error and return the special floating point value -1.#IND000000000000,
    meaning 'indefinite', which becomes INT32_MIN when converted to an integer. Observed on 64-bit Windows. See case 1496.
    We fix the problem by means of the following check.
    */
    if (cos_angle >= 1)
        return 0;
    
    double angle = std::acos(cos_angle);
    return angle * KEquatorialRadiusInMetres;
    }

template<class T> static inline void Reverse(T* aStart,size_t aLength)
    {
    if (aLength > 1)
        {
        T* end = aStart + aLength - 1;
        while (aStart < end)
            {
            T temp = *aStart;
            *aStart = *end;
            *end = temp;
            aStart++;
            end--;
            }
        }
    }

/**
Get the azimuth (direction along a great circle line) from one point to another in degrees,
given two lat-long points in degrees.

This function is an approximation assuming a spherical earth.

North is 0 and angles increase clockwise.
*/
double AzimuthInDegrees(double aLong1,double aLat1,double aLong2,double aLat2);

/** The standard number of levels of the text index to load into RAM when loading a CTM1 file. */
constexpr int32 KDefaultTextIndexLevels = 1;

class CMapDataBase;
/** A type for an array of map databases: used internally. */
using CMapDataBaseArray = std::vector<std::unique_ptr<CMapDataBase>>;
/** A type for an array of non-owning pointers to map databases: used internally. */
using CMapDataBaseSet = std::vector<CMapDataBase*>;

inline uint8 ReadBigEndian(const uint8* aP)
    {
    return *aP;
    }
inline void WriteBigEndian(uint8* aP,uint8 aValue)
    {
    *aP = aValue;
    }
inline int16 ReadBigEndian(const int16* aP)
    {
    return (int16)( (((const uint8*)aP)[0] << 8) | ((const uint8*)aP)[1] );
    }
inline void WriteBigEndian(int16* aP,int16 aValue)
    {
    ((uint8*)aP)[0] = (uint8)(aValue >> 8);
    ((uint8*)aP)[1] = (uint8)aValue;
    }

/**
Use bilinear interpolation to get the value at (aX,aY) from a rectangular table of data.
Each data item has aChannels channels, and each channel takes up aWidth * sizeof(DataType) bytes.
The UnknownValue is ignored when interpolating.
*/
template<class DataType,int UnknownValue = INT_MIN> double InterpolatedValue(const DataType* aDataStart,int32 aWidth,int32 aHeight,int32 aStride,int32 aChannels,double aX,double aY,int aChannel)
    {
    double x_fraction = 1 - (aX - std::floor(aX));
    if ((int)aX == aWidth - 1)
        x_fraction = 1;
    double y_fraction = 1 - (aY - std::floor(aY));
    if ((int)aY == aHeight - 1)
        y_fraction = 1;
    int index = (int)aY * aWidth * aChannels + (int)aX * aChannels + aChannel;
    double top_value = ReadBigEndian(aDataStart + index);
    if (x_fraction < 1)
        {
        double top_right_value = ReadBigEndian(aDataStart + index + aChannels);
        if (top_right_value != UnknownValue)
            top_value = top_value * x_fraction + top_right_value * (1.0 - x_fraction);
        }
    double value = top_value;
    if (y_fraction < 1)
        {
        index += aStride * aChannels;
        double bottom_value = ReadBigEndian(aDataStart + index);
        if (x_fraction < 1)
            {
            double bottom_right_value = ReadBigEndian(aDataStart + index + aChannels);
            if (bottom_right_value != UnknownValue)
                bottom_value = bottom_value * x_fraction + bottom_right_value * (1.0 - x_fraction);
            }
        if (bottom_value != UnknownValue)
            value = top_value * y_fraction + bottom_value * (1.0 - y_fraction);
        }
    return value;
    }

} // namespace CartoType

#ifdef _WIN32_WCE
#ifdef __cplusplus
extern "C" {
#endif
char* getenv(const char* name);
void abort(void);
#ifdef __cplusplus
}
#endif
#endif

#endif
