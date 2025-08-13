#pragma once

#include <cmath>

#define M_PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679821480865132823066470938446095505822317253594081284811174502841027019385211055596446229489549303819644288109756659334461284756482337867831652712019091456485669234603486104543266482133936072602491412737245870066063155881748815209209628292540917153643678925903600113305305488204665213841469519415116094330572703657595919530921861173819326117931051185480744623799627495673518857527248912279381830119491298336733624406566430860213949463952247371907021798609437027705392171762931767523846748184676694051320005681271452635608277857713427577896091736371787214684409012249534301465495853710507922796892589235420199561121290219608640344181598136297747713099605187072113499999983729780499510597317328160963185950244594553469083026425223082533446850352619311881710100031378387528865875332083814206171776691473035982534904287554687311595628638823537875937519577818577805321712268066130019278766111959092164201989380952572010654858632788659361533818279682303019520353018529689957736225994138912497217752834791315155748572424541506959508295331168617278558890750983817546374649393192550604009277016711390098488240128583616035637076601047101819429555961989467678374494482553797747268471040475346462080466842590694912933136770289891521047521620569660240580

namespace unity {
    struct vector {
        union {
            struct { float x, y, z; };
            float data[3];
        };
        inline vector() : x(0), y(0), z(0) {};
        inline vector(float x, float y, float z) : x(x), y(y), z(z) {};
        [[maybe_unused]] inline static vector Back() { return vector(0, 0, -1); };
        [[maybe_unused]] inline static vector Down() { return vector(0, -1, 0); };
        [[maybe_unused]] inline static vector Forward() { return vector(0, 0, 1); };
        [[maybe_unused]] inline static vector Left() { return vector(-1, 0, 0); };
        [[maybe_unused]] inline static vector Right() { return vector(1, 0, 0); };
        [[maybe_unused]] inline static vector Up() { return vector(0, 1, 0); }
        [[maybe_unused]] inline static vector Zero() { return vector(0, 0, 0); }
        [[maybe_unused]] inline static vector One() { return vector(1, 1, 1); }
        inline static float Angle(vector from, vector to);
        inline static float Dot(vector, vector);
        [[maybe_unused]] inline static vector ClampMagnitude(vector, float);
        [[maybe_unused]] inline static float Component(vector, vector);
        inline static vector Cross(vector, vector);
        [[maybe_unused]] inline static float Distance(vector, vector);
        [[maybe_unused]] inline static vector FromSpherical(float, float, float);
        [[maybe_unused]] inline static vector Lerp(vector, vector, float);
        inline static vector LerpUnclamped(vector, vector, float);
        [[maybe_unused]] inline static vector Max(vector, vector);
        [[maybe_unused]] inline static vector Min(vector, vector);
        [[maybe_unused]] inline static vector MoveTowards(vector, vector, float);
        inline static vector Orthogonal(vector);
        [[maybe_unused]] inline static vector RotateTowards(vector, vector, float, float);
        [[maybe_unused]] inline static vector Scale(vector, vector);
        [[maybe_unused]] inline static vector Slerp(vector, vector, float);
        inline static vector SlerpUnclamped(vector, vector, float);
        [[maybe_unused]] inline static void ToSpherical(vector vector, float&, float&, float&);
        inline static float Magnitude(vector);
        inline static float SqrMagnitude(vector);
        inline static vector Normalize(vector);
        inline static vector NormalizeEuler(vector);
        inline static float NormalizeAngle(float f);
        inline vector orthogonal() { return Orthogonal(*this); }
        inline float magnitude() { return Magnitude(*this); }
        inline float sqrMagnitude() { return SqrMagnitude(*this); }
        inline vector normalized() { return Normalize(*this); }
        [[maybe_unused]] inline vector normalizedEuler() { return NormalizeEuler(*this); }
        inline const vector& operator+=(float v) { x += v; y += v; y += v; return *this; };
        inline const vector& operator-=(float v) { x -= v; y -= v; y -= v; return *this; };
        inline const vector& operator*=(float v) { x *= v; y *= v; z *= v; return *this; };
        inline const vector& operator/=(float v) { x /= v; y /= v; z /= v; return *this; };
        inline const vector& operator+=(vector v) { x += v.x; y += v.y; z += v.z; return *this; };
        inline const vector& operator-=(vector v) { x -= v.x; y -= v.y; z -= v.z; return *this; };
        inline const vector& operator*=(vector v) { x *= v.x; y *= v.y; z *= v.z; return *this; };
        inline vector& operator/=(vector v) { x /= v.x; y /= v.y; z /= v.z; return *this; };
    };
    inline vector operator+(vector lhs, const float rhs) { return vector(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs); }
    inline vector operator-(vector lhs, const float rhs) { return vector(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs); }
    inline vector operator*(vector lhs, const float rhs) { return vector(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs); }
    inline vector operator/(vector lhs, const float rhs) { return vector(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs); }
    inline vector operator+(const float lhs, vector rhs) { return vector(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z); }
    inline vector operator-(const float lhs, vector rhs) { return vector(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z); }
    inline vector operator*(const float lhs, vector rhs) { return vector(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z); }
    inline vector operator/(const float lhs, vector rhs) { return vector(lhs / rhs.x, lhs / rhs.y, lhs / rhs.z); }
    inline vector operator+(vector lhs, const vector rhs) { return vector(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z); }
    inline vector operator-(vector lhs, const vector rhs) { return vector(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z); }
    inline vector operator*(vector lhs, const vector rhs) { return vector(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z); }
    inline vector operator/(vector lhs, const vector rhs) { return vector(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z); }
    inline bool operator==(const vector lhs, const vector rhs) { return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z; }
    inline bool operator!=(const vector lhs, const vector rhs) { return lhs.x != rhs.x && lhs.y != rhs.y && lhs.z != rhs.z; }
    inline vector operator-(vector v) { return v * -1; }

    float vector::Angle(vector from, vector to) {
        float v = Dot(from, to) / (from.magnitude() * to.magnitude());
        v = fmax(v, -1.0);
        v = fmin(v, 1.0);
        return acos(v);
    }

    float vector::Dot(vector lhs, vector rhs) {
        return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
    }
    [[maybe_unused]] vector vector::ClampMagnitude(vector vector, float maxLength) {
        float length = vector.magnitude();
        if (length > maxLength) vector *= maxLength / length;
        return vector;
    }

    [[maybe_unused]] float vector::Component(vector a, vector b) {
        return Dot(a, b) / b.magnitude();
    }

    vector vector::Cross(vector lhs, vector rhs) {
        float x = lhs.y * rhs.z - lhs.z * rhs.y;
        float y = lhs.z * rhs.x - lhs.x * rhs.z;
        float z = lhs.x * rhs.y - lhs.y * rhs.x;
        return vector(x, y, z);
    }

    [[maybe_unused]] float vector::Distance(vector a, vector b) {
        return (a - b).magnitude();
    }

    [[maybe_unused]] vector vector::FromSpherical(float rad, float theta, float phi) {
        vector v;
        v.x = rad * sin(theta) * cos(phi);
        v.y = rad * sin(theta) * sin(phi);
        v.z = rad * cos(theta);
        return v;
    }

    [[maybe_unused]] vector vector::Lerp(vector a, vector b, float t) {
        if (t < 0) return a;
        else if (t > 1) return b;
        return LerpUnclamped(a, b, t);
    }

    vector vector::LerpUnclamped(vector a, vector b, float t) {
        return (b - a) * t + a;
    }

    [[maybe_unused]] vector vector::Max(vector a, vector b) {
        float x = a.x > b.x ? a.x : b.x;
        float y = a.y > b.y ? a.y : b.y;
        float z = a.z > b.z ? a.z : b.z;
        return vector(x, y, z);
    }

    [[maybe_unused]] vector vector::Min(vector a, vector b) {
        float x = a.x > b.x ? b.x : a.x;
        float y = a.y > b.y ? b.y : a.y;
        float z = a.z > b.z ? b.z : a.z;
        return vector(x, y, z);
    }

    [[maybe_unused]] vector vector::MoveTowards(vector current, vector target, float maxDistanceDelta) {
        vector d = target - current;
        float m = d.magnitude();
        if (m < maxDistanceDelta || m == 0)
            return target;
        return current + (d * maxDistanceDelta / m);
    }

    vector vector::Orthogonal(vector v) {
        return v.z < v.x ? vector(v.y, -v.x, 0) : vector(0, -v.z, v.y);
    }

    [[maybe_unused]] vector vector::RotateTowards(vector current, vector target, float maxRadiansDelta, float maxMagnitudeDelta) {
        float magCur = current.magnitude();
        float magTar = target.magnitude();
        float newMag = magCur + maxMagnitudeDelta * ((magTar > magCur) - (magCur > magTar));
        newMag = fmin(newMag, fmax(magCur, magTar));
        newMag = fmax(newMag, fmin(magCur, magTar));
        float totalAngle = Angle(current, target) - maxRadiansDelta;
        if (totalAngle <= 0) return Normalize(target) * newMag;
        else if (totalAngle >= M_PI) return -target.normalized() * newMag;
        vector axis = Cross(current, target);
        float magAxis = axis.magnitude();
        if (magAxis == 0) axis = Cross(current, current + vector(3.95, 5.32, -4.24)).normalized();
        else axis /= magAxis;
        current = current.normalized();
        vector newVector = current * cos(maxRadiansDelta) + Cross(axis, current) * sin(maxRadiansDelta);
        return newVector * newMag;
    }

    [[maybe_unused]] vector vector::Scale(vector a, vector b) {
        return vector(a.x * b.x, a.y * b.y, a.z * b.z);
    }

    [[maybe_unused]] vector vector::Slerp(vector a, vector b, float t) {
        if (t < 0) return a;
        else if (t > 1) return b;
        return SlerpUnclamped(a, b, t);
    }

    vector vector::SlerpUnclamped(vector a, vector b, float t) {
        float magA = a.magnitude();
        float magB = b.magnitude();
        a /= magA;
        b /= magB;
        float dot = Dot(a, b);
        dot = fmax(dot, -1.0);
        dot = fmin(dot, 1.0);
        float theta = acos(dot) * t;
        vector relativeVec = Normalize(b - a * dot);
        vector newVec = a * cos(theta) + relativeVec * sin(theta);
        return newVec * (magA + (magB - magA) * t);
    }

    [[maybe_unused]] void vector::ToSpherical(vector vector, float& rad, float& theta, float& phi) {
        rad = vector.magnitude();
        float v = vector.z / rad;
        v = fmax(v, -1.0);
        v = fmin(v, 1.0);
        theta = acos(v);
        phi = atan2(vector.y, vector.x);
    }

    float vector::Magnitude(vector v) {
        return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    }

    float vector::SqrMagnitude(vector v) {
        return v.x * v.x + v.y * v.y + v.z * v.z;
    }

    vector vector::Normalize(vector v) {
        float mag = v.magnitude();
        if (mag == 0) return vector::Zero();
        return v / mag;
    }

    float vector::NormalizeAngle(float f) {
        while (f > 360) f -= 360;
        while (f < 0) f += 360;
        return f;
    }

    vector vector::NormalizeEuler(vector vec) {
        vec.x = NormalizeAngle(vec.x);
        vec.y = NormalizeAngle(vec.y);
        vec.z = NormalizeAngle(vec.z);
        return vec;
    }

}