
#pragma once

class Orientation
{
//! Construction:
public:
	Orientation();

	static Orientation look_at(const CL_Vec3f &eye, const CL_Vec3f &center, const CL_Vec3f &up);
	static Orientation look_at(float eye_x, float eye_y, float eye_z, float center_x, float center_y, float center_z, float up_x, float up_y, float up_z);
	static Orientation look_at(const CL_Vec3f &dir, const CL_Vec3f &up);
	static Orientation look_at(float dir_x, float dir_y, float dir_z, float up_x, float up_y, float up_z);
	static Orientation euler_angles(float angle_x, float angle_y, float angle_z);
	static Orientation axis_angle(float angle, const CL_Vec3f &axis);
	static Orientation quaternion(float x, float y, float z, float w);
	static Orientation quaternion(const CL_Vec4f &quaternion);
	static Orientation interpolate(const Orientation &orientation1, const Orientation &orientation2, float t);

//! Attributes:
public:
	float get_x() const { return q.x; }
	float get_y() const { return q.y; }
	float get_z() const { return q.z; }
	float get_w() const { return q.w; }
	CL_Vec4f get_quaternion() const { return CL_Vec4f(q.x, q.y, q.z, q.w); }

//! Operations:
public:
	CL_Mat4f to_matrix() const;

	static Orientation multiply(const Orientation &orientation1, const Orientation &orientation2);
	Orientation &multiply(const Orientation &orientation);

	Orientation &rotate(float angle, const CL_Vec3f &axis);
	Orientation &rotate(float angle_x, float angle_y, float angle_z);

//! Implementation:
private:
	CL_Quaternionf q;
};
