#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

#include "ray.h"
#include "aabb.h"

class material;

struct hit_record
{
	float t;
	vec3 p;
	vec3 normal;
	material *mat_ptr;
};

class hittable
{
public:
	virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const = 0;
	virtual bool bounding_box(aabb& box) const = 0;
};

//We added an emitted function. Like the background, it just tells the ray
//	what color it is and performs no reflection.

class flip_normals : public hittable
{
public: 
	flip_normals(hittable *p) : ptr(p) {}
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const
	{
		if (ptr->hit(r, t_min, t_max, rec))
		{
			rec.normal = -rec.normal;
			return true;
		}
		else
			return false;
	}

	virtual bool bounding_box(aabb& box) const
	{
		return ptr->bounding_box(box);
	}

	hittable *ptr;
};


/*
An instance is a geometric primitive that has
been moved or rotated somehow. This is especially easy in ray tracing because we don’t move
anything; instead we move the rays in the opposite direction.
*/
class translate : public hittable
{
public:
	translate(hittable *p, const vec3& displacement) : ptr(p), offset(displacement) {}
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
	virtual bool bounding_box(aabb& box) const;
	hittable *ptr;
	vec3 offset;
};

bool translate::hit(const ray& r, float t_min, float t_max, hit_record& rec) const
{
	ray moved_r(r.origin() - offset, r.direction());
	if (ptr->hit(moved_r, t_min, t_max, rec))
	{
		rec.p += offset;
		return true;
	}
	else
		return false;
}

bool translate::bounding_box(aabb& box) const
{
	if (ptr->bounding_box(box))
	{
		box = aabb(box.min() + offset, box.max() + offset);
		return true;
	}
	else
		return false;
}