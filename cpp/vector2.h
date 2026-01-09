#pragma once
/**
 * table.h
 */

#ifndef TABLE_H
#define TABLE_H

#include <cstddef>
#include <string>
#include <ostream>

template <typename T>
class Vector2 {
public:
	int x = 0;
	int y = x;
	Vector2() = default;
	Vector2(int x, int y) : x(x), y(y) {}

	#pragma region Operators
	const Vector2<T>& operator+(const Vector2<T>& other) const { return Vector2<T>(x + other.x, y + other.y); }
	const Vector2<T>& operator+(const T& other) const { return Vector2<T>(x + other, y + other); }
	const Vector2<T>& operator-(const Vector2<T>& other) const { return Vector2<T>(x - other.x, y - other.y); }
	const Vector2<T>& operator-(const T& other) const { return Vector2<T>(x - other, y - other); }
	const Vector2<T>& operator*(const T& scalar) const { return Vector2<T>(x * scalar, y * scalar); }
	const Vector2<T>& operator/(const T& scalar) const { return Vector2<T>(x / scalar, y / scalar); }
	const Vector2<T>& operator%(const T& scalar) const { return Vector2<T>(x % scalar, y % scalar); }
	const Vector2<T>& operator<<(const T& scalar) const { return Vector2<T>(x << scalar, y << scalar); }
	const Vector2<T>& operator>>(const T& scalar) const { return Vector2<T>(x >> scalar, y >> scalar); }
	const Vector2<T>& operator&(const T& scalar) const { return Vector2<T>(x & scalar, y & scalar); }
	const Vector2<T>& operator|(const T& scalar) const { return Vector2<T>(x | scalar, y | scalar); }
	const Vector2<T>& operator^(const T& scalar) const { return Vector2<T>(x ^ scalar, y ^ scalar); }
	const Vector2<T>& operator~() const { return Vector2<T>(~x, ~y); }
	const Vector2<T>& operator-() const { return Vector2<T>(-x, -y); }
	const Vector2<T>& operator++() { ++x; ++y; return *this; }
	const Vector2<T>& operator--() { --x; --y; return *this; }
	const Vector2<T>& operator++(int) { Vector2<T> temp = *this; ++x; ++y; return temp; }
	const Vector2<T>& operator--(int) { Vector2<T> temp = *this; --x; --y; return temp; }
	bool operator==(const Vector2<T>& other) const { return x == other.x && y == other.y; }
	bool operator!=(const Vector2<T>& other) const { return x != other.x || y != other.y; }
	bool operator<(const Vector2<T>& other) const { return x < other.x && y < other.y; }
	bool operator<=(const Vector2<T>& other) const { return x <= other.x && y <= other.y; }
	bool operator>(const Vector2<T>& other) const { return x > other.x && y > other.y; }
	bool operator>=(const Vector2<T>& other) const { return x >= other.x && y >= other.y; }
	#pragma endregion

	#pragma region Foreign Operators
	friend std::ostream& operator<<(std::ostream& os, const Vector2<T>& v) {
		os << v.toString();
		return os;
	}
	friend std::wostream& operator<<(std::wostream& os, const Vector2<T>& v) {
		os << v.toWString();
		return os;
	}
	#pragma endregion


	std::string toString() const { return std::to_string(x) + ";" + std::to_string(y); }
	std::wstring toWString() const { return std::to_wstring(x) + L";" + std::to_wstring(y); }

	#pragma region Static Methods
	static Vector2<T>& up() { return *(new Vector2<T>(static_cast<T>(0), static_cast<T>(1))); }
	static Vector2<T>& down() { return *(new Vector2<T>(static_cast<T>(0), static_cast<T>(-1))); }
	static Vector2<T>& left() { return *(new Vector2<T>(static_cast<T>(-1), static_cast<T>(0))); }
	static Vector2<T>& right() { return *(new Vector2<T>(static_cast<T>(1), static_cast<T>(0))); }

	struct Conversions {
		static constexpr T fokToRad = static_cast<T>(0.017453292519943295); // PI / 180
		static constexpr T degToRad = Vector2<T>::Conversions::fokToRad; // PI / 180
		static constexpr T radToFok = static_cast<T>(57.29577951308232); // 180 / PI
		static constexpr T radToDeg = Vector2<T>::Conversions::radToFok; // 180 / PI

		static constexpr T fok1rad = Vector2<T>::Conversions::fokToRad; // PI / 180
		static constexpr T deg1rad = Vector2<T>::Conversions::fokToRad; // PI / 180
		static constexpr T fok15rad = static_cast<T>(0.2617993877991494);
		static constexpr T deg15rad = Vector2<T>::Conversions::fok15rad;  // Alias for fok15rad
		static constexpr T fok30rad = static_cast<T>(0.5235987755982988);
		static constexpr T deg30rad = Vector2<T>::Conversions::fok30rad;  // Alias for fok30rad
		static constexpr T fok45rad = static_cast<T>(0.7853981633974483);
		static constexpr T deg45rad = Vector2<T>::Conversions::fok45rad;  // Alias for fok45rad
		static constexpr T fok90rad = static_cast<T>(1.5707963267948966);
		static constexpr T deg90rad = Vector2<T>::Conversions::fok90rad;  // Alias for fok90rad
		static constexpr T fok135rad = static_cast<T>(2.356194490192345);
		static constexpr T deg135rad = Vector2<T>::Conversions::fok135rad;  // Alias for fok135rad
		static constexpr T fok180rad = static_cast<T>(3.141592653589793);
		static constexpr T deg180rad = Vector2<T>::Conversions::fok180rad;  // Alias for fok180rad
	};

	#pragma endregion

	#pragma region Static Getters
	static Vector2<T>& null() { return *(new Vector2<T>(static_cast<T>(0), static_cast<T>(0))); }
	static Vector2<T>& one() { return *(new Vector2<T>(static_cast<T>(1), static_cast<T>(1))); }
	#pragma endregion


	#pragma region Getters
	T length() const { return static_cast<T>(std::sqrt(x * x + y * y)); }
	Vector2<T> normalized() const {
		T len = length();
		return Vector2<T>(x / len, y / len);
	}
	Vector2<T> rounded() const { return Vector2<T>(static_cast<T>(std::round(x)), static_cast<T>(std::round(y))); }
	Vector2<T> floor() const { return Vector2<T>(static_cast<T>(std::floor(x)), static_cast<T>(std::floor(y))); }
	Vector2<T> ceil() const { return Vector2<T>(static_cast<T>(std::ceil(x)), static_cast<T>(std::ceil(y))); }
	Vector2<T> self() const { return Vector2<T>(x, y); }
	T radians() const { return std::atan2(static_cast<long double>(y), static_cast<long double>(x)); }
	T fok() const { return radians() * Vector2<T>::Conversions::radToFok; }
	T deg() const { return radians() * Vector2<T>::Conversions::radToDeg; }
	T dot(const Vector2<T>& other) const { return x * other.x + y * other.y; }
	Vector2<T> up(T difference) const { return Vector2<T>(x, y + difference); }
	Vector2<T> down(T difference) const { return Vector2<T>(x, y - difference); }
	Vector2<T> left(T difference) const { return Vector2<T>(x - difference, y); }
	Vector2<T> right(T difference) const { return Vector2<T>(x + difference, y); }
	bool isNull() const { return x == static_cast<T>(0) && y == static_cast<T>(0); }
	Vector2<T> withLength(T newLength) const {
		Vector2<T> norm = normalized();
		return Vector2<T>(norm.x * newLength, norm.y * newLength);
	}
	Vector2<T> scaled(T scalar) const { return Vector2<T>(x * scalar, y * scalar); }
	bool similar(const Vector2<T>& other, T tolerance) const { return (std::abs(x - other.x) <= tolerance) && (std::abs(y - other.y) <= tolerance); }
	T distanceTo(const Vector2<T>& other) const {
		T dx = x - other.x;
		T dy = y - other.y;
		return static_cast<T>(std::sqrt(dx * dx + dy * dy));
	}
	T directionTo(const Vector2<T>& other, bool rad=true) const {
		T angle = std::atan2(static_cast<long double>(other.y - y), static_cast<long double>(other.x - x));
		return rad ? angle : angle * Vector2<T>::Conversions::radToDeg;
	}
	bool isSameAs(const Vector2<T>& other) const { return x == other.x && y == other.y; }
	Vector2<T> rotated(T angleRad, bool rad=true) const {
		Vector2<T> res = *this;
		return res.rotate(angleRad, rad);
	}
	#pragma endregion

	#pragma region Parsers
	static Vector2<T> as(T x, T y) { return Vector2<T>(x, y); }
	static Vector2<T> grid(T size) { return Vector2<T>(size, size); }
	static Vector2<T> fromX(T x) { return Vector2<T>(x, static_cast<T>(0)); }
	static Vector2<T> fromY(T y) { return Vector2<T>(static_cast<T>(0), y); }
	static Vector2<T> random(T min, T max) { return Vector2<T>(static_cast<T>(std::rand() % (max - min + 1) + min), static_cast<T>(std::rand() % (max - min + 1) + min)); }
	static Vector2<T> randomX(T min, T max) { return Vector2<T>(static_cast<T>(std::rand() % (max - min + 1) + min), static_cast<T>(1)); }
	static Vector2<T> randomY(T min, T max) { return Vector2<T>(static_cast<T>(1), static_cast<T>(std::rand() % (max - min + 1) + min)); }
	static Vector2<T> fromString(const std::string& str) {
		size_t delimiterPos = str.find(';');
		if (delimiterPos == std::string::npos) {
			throw std::invalid_argument("Invalid string format for Vector2");
		}
		T x = static_cast<T>(std::stod(str.substr(0, delimiterPos)));
		T y = static_cast<T>(std::stod(str.substr(delimiterPos + 1)));
		return Vector2<T>(x, y);
	}
	static Vector2<T> parseString(const std::string& str) { return fromString(str); }
	static Vector2<T> fromWString(const std::wstring& str) {
		size_t delimiterPos = str.find(L';');
		if (delimiterPos == std::wstring::npos) {
			throw std::invalid_argument("Invalid string format for Vector2");
		}
		T x = static_cast<T>(std::stod(str.substr(0, delimiterPos)));
		T y = static_cast<T>(std::stod(str.substr(delimiterPos + 1)));
		return Vector2<T>(x, y);
	}
	static Vector2<T> parseWString(const std::wstring& str) { return fromWString(str); }
	#pragma endregion

	#pragma region Setters

	Vector2<T>& set(T newX, T newY) { x = newX; y = newY; return *this; }
	Vector2<T>& set(Vector2<T>& v2) { x = v2.x; y = v2.y; return *this; }
	Vector2<T>& setx(T newX) { x = newX; return *this; }
	Vector2<T>& sety(T newY) { y = newY; return *this; }
	Vector2<T>& flip(bool flipX = true, bool flipY = true) {
		if (flipX) x = -x;
		if (flipY) y = -y;
		return *this;
	}
	Vector2<T>& flipX() { x = -x; return *this; }
	Vector2<T>& flipY() { y = -y; return *this; }
	Vector2<T>& normalize() {
		T len = length();
		x /= len;
		y /= len;
		return *this;
	}
	Vector2<T>& setLength(T newLength) {
		normalize();
		x *= newLength;
		y *= newLength;
		return *this;
	}
	Vector2<T>& scale(T scalar) {
		x *= scalar;
		y *= scalar;
		return *this;
	}
	Vector2<T> copy() const { return Vector2<T>(x, y); }
	Vector2<T>& move(T deltaX, T deltaY) {
		x += deltaX;
		y += deltaY;
		return *this;
	}
	Vector2<T>& move(const Vector2<T>& delta) {
		x += delta.x;
		y += delta.y;
		return *this;
	}
	Vector2<T>& rotate(T angleRad, bool rad=true) {
		if (!rad) angleRad *= Vector2<T>::Conversions::degToRad;
		T cosA = static_cast<T>(std::cos(static_cast<long double>(angleRad)));
		T sinA = static_cast<T>(std::sin(static_cast<long double>(angleRad)));
		T newX = x * cosA - y * sinA;
		T newY = x * sinA + y * cosA;
		x = newX;
		y = newY;
		return *this;
	}
	#pragma endregion

};

#endif