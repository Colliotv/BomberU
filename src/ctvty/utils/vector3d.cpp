#include <cmath>
#include "ctvty/utils/vector3d.hh"

namespace ctvty {
  namespace utils {

    REGISTER_FOR_SERIALIZATION(ctvty::utils, Vector3D);

    Vector3D::		Vector3D()
      : Vector3D(zero) {}

    Vector3D::		Vector3D(float _x, float _y, float _z)
      : x(_x), y(_y), z(_z) { }

    Vector3D::		Vector3D(const serialization::Archive& __serial) {
      __serial["x"] & x;
      __serial["y"] & y;
      __serial["z"] & z;
    }

    void		Vector3D::Serialize(serialization::Archive& __serial_instance) {
      SERIALIZE_OBJECT_AS(ctvty::utils::Vector3D, __serial_instance);
      __serial["x"] & x;
      __serial["y"] & y;
      __serial["z"] & z;
    }


    const Vector3D	Vector3D::right		( 1.,  0.,  0.);
    const Vector3D	Vector3D::left		(-1.,  0.,  0.);
    const Vector3D	Vector3D::up		( 0.,  1.,  0.);
    const Vector3D	Vector3D::down		( 0., -1.,  0.);
    const Vector3D	Vector3D::forward	( 0.,  0.,  1.);
    const Vector3D	Vector3D::back		( 0.,  0., -1.);
    const Vector3D	Vector3D::zero		( 0.,  0.,  0.);
    const Vector3D	Vector3D::one		( 1.,  1.,  1.);


    const Vector3D	Vector3D::GetNormalized() const{
      float magnitude = GetMagnitude();
      return Vector3D(x / magnitude, y / magnitude, z / magnitude);
    }

    float		Vector3D::GetMagnitude() const{
      return std::sqrt(x*x + y*y + z*z);
    }



    const Vector3D	Vector3D::Project(const Vector3D& vector) const {
      if (vector.GetMagnitude() == 0)
	return Vector3D::zero;
      return vector * (DotProduct(vector) / (vector.GetMagnitude() * vector.GetMagnitude()));
    }

    const Vector3D	Vector3D::ProjectOnPlane(const Vector3D& normal) const {
      return Project(normal) - (*this);
    }

    const Vector3D	Vector3D::Reflect(const Vector3D& normal) const {
      return (normal * (-2 * DotProduct(normal))) + (*this);
    }



    float		Vector3D::DotProduct(const Vector3D& vector) const {
      return
	vector.x * x +
	vector.y * y +
	vector.z * z ;
    }

    const Vector3D	Vector3D::CrossProduct(const Vector3D& vector) const {
      return
	Vector3D(
		 y * vector.z - z * vector.y,
		 z * vector.x - x * vector.z,
		 x * vector.y - y * vector.x
		 );
    }

    float		Vector3D::ScalarProjection(const Vector3D& vector) const {
      if (vector.GetMagnitude() == 0)
	return 0;
      return (DotProduct(vector) / (vector.GetMagnitude() * vector.GetMagnitude()));
    }



    const Vector3D	Vector3D::operator * (float f) const {
      return Vector3D(x * f, y *f, z *f);
    }

    const Vector3D	Vector3D::operator / (float f) const {
      return Vector3D(x / f, y / f, z / f);
    }

    Vector3D&		Vector3D::operator *= (float f) {
      x *= f;
      y *= f;
      z *= f;
      return *this;
    }

    Vector3D&		Vector3D::operator /= (float f) {
      x /= f;
      y /= f;
      z /= f;
      return *this;
    }
    


    const Vector3D		Vector3D::operator - () const {
      return Vector3D(-x, -y, -z);
    }

    const Vector3D		Vector3D::operator - (const Vector3D& vector) const {
      return Vector3D(x - vector.x,
		      y - vector.y,
		      z - vector.z);
    }

    const Vector3D		Vector3D::operator + (const Vector3D& vector) const {
      return Vector3D(x + vector.x,
		      y + vector.y,
		      z + vector.z);
    }

    Vector3D&		Vector3D::operator += (const Vector3D& vector) {
      x += vector.x;
      y += vector.y;
      z += vector.z;
      return *this;
    }

    Vector3D&		Vector3D::operator -= (const Vector3D& vector) {
      x -= vector.x;
      y -= vector.y;
      z -= vector.z;
      return *this;
    }

    bool		Vector3D::operator == (const Vector3D& vector) const {
      return x == vector.x && y == vector.y && z == vector.z;
    }
  };
};
