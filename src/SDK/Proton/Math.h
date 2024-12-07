#pragma once
#include <utility>
#include <vector>
#include <memory>
#include <cmath>
#include <algorithm>

template<class T>
class CL_Vec2 
{
public:
    constexpr inline CL_Vec2(const T& x = 0, const T& y = 0) noexcept : X(x), Y(y) {}

    constexpr inline bool operator==(const CL_Vec2<T>& other) const noexcept { return X == other.X && Y == other.Y; }
    constexpr inline bool operator!=(const CL_Vec2<T>& other) const noexcept { return X != other.X || Y != other.Y; }

    constexpr inline CL_Vec2<T> operator+(const CL_Vec2<T>& other) noexcept { return CL_Vec2<T>(X + other.X, Y + other.Y); }
    constexpr inline CL_Vec2<T> operator-(const CL_Vec2<T>& other) noexcept { return CL_Vec2<T>(X - other.X, Y - other.Y); }
    constexpr inline CL_Vec2<T> operator/(const CL_Vec2<T>& other) noexcept { return CL_Vec2<T>(X / other.X, Y / other.Y); }
    constexpr inline CL_Vec2<T> operator*(const CL_Vec2<T>& other) noexcept { return CL_Vec2<T>(X * other.X, Y * other.Y); }

    T X, Y;
};

template<class T>
class CL_Vec3 
{
public:
    constexpr inline CL_Vec3(const T& x = 0, const T& y = 0, const T& z = 0) noexcept : X(x), Y(y), Z(z) {}

    constexpr inline bool operator==(const CL_Vec3<T>& other) const noexcept { return X == other.X && Y == other.Y && Z == other.Z; }
    constexpr inline bool operator!=(const CL_Vec3<T>& other) const noexcept { return X != other.X || Y != other.Y || other.Z; }

    constexpr inline CL_Vec3<T> operator+(const CL_Vec3<T>& other) noexcept { return CL_Vec3<T>(X + other.X, Y + other.Y, Z + other.Z); }
    constexpr inline CL_Vec3<T> operator-(const CL_Vec3<T>& other) noexcept { return CL_Vec3<T>(X - other.X, Y - other.Y, Z - other.Z); }
    constexpr inline CL_Vec3<T> operator/(const CL_Vec3<T>& other) noexcept { return CL_Vec3<T>(X / other.X, Y / other.Y, Z / other.Z); }
    constexpr inline CL_Vec3<T> operator*(const CL_Vec3<T>& other) noexcept { return CL_Vec3<T>(X * other.X, Y * other.Y, Z * other.Z); }

    T X, Y, Z;
};

template<class T>
class CL_Rect 
{
public:
    T X;
    T Y;
    T W;
    T H;

    constexpr inline CL_Rect(const T& x, const T& y, const T& width, const T& height) noexcept : X(x), Y(y), W(width), H(height) {}

    constexpr inline bool operator==(const CL_Rect& other) const noexcept { return X == other.X && Y == other.Y && W == other.W && H == other.H; }
    constexpr inline bool operator!=(const CL_Rect& other) const noexcept { return X != other.X || Y != other.Y || W != other.W || H != other.H; }

    constexpr inline CL_Rect<T> operator+(const CL_Rect<T>& other) noexcept { return CL_Rect<T>(X + other.X, Y + other.Y, W + other.W, H + other.H); }
    constexpr inline CL_Rect<T> operator-(const CL_Rect<T>& other) noexcept { return CL_Rect<T>(X - other.X, Y - other.Y, W - other.W, H - other.H); }
    constexpr inline CL_Rect<T> operator*(const CL_Rect<T>& other) noexcept { return CL_Rect<T>(X * other.X, Y * other.Y, W * other.W, H * other.H); }
};

using CL_Vec2i = CL_Vec2<int>;
using CL_Vec2f = CL_Vec2<float>;
using CL_Vec3i = CL_Vec3<int>;
using CL_Vec3f = CL_Vec3<float>;
using CL_Recti = CL_Rect<int>;
using CL_Rectf = CL_Rect<float>;

struct Node 
{
    Node(CL_Vec2i pos) noexcept : pos(pos), g_cost(0), h_cost(0), pParentNode(NULL)
    {
        //
    }

    CL_Vec2i pos;
    uint32_t g_cost, h_cost;
    Node* pParentNode = NULL;

};

static Node* FindNode(const std::vector<Node*>& set, CL_Vec2i pos) noexcept 
{
    auto it = std::find_if(set.begin(), set.end(), [pos](Node* pNode) { return pNode->pos == pos; });
    if (it != set.end())
    {
        return *it;
    }

    return NULL;
}

static uint32_t CalculateHeuristic(CL_Vec2i pos, CL_Vec2i goal) noexcept 
{
    return std::abs(pos.X - goal.X) + std::abs(pos.Y - goal.Y);
}