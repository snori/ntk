#ifndef __GEOM_H__
#define __GEOM_H__


#include <vector>
#include <list>
#include <windows.h>
#include <gl/gl.h>
#include "vector.h"


typedef GLfloat real_t;
typedef vector2_<real_t> vector2_t;
typedef vector3_<real_t> vector3_t;
typedef vector4_<real_t> vector4_t;
typedef vector3_t point3_t;
typedef vector3_t color3_t;
//typedef ntk_vector4_<real_t> vector4_t;
//typedef ntk_point3_<real_t>  point3_t;
//typedef ntk_point4_<real_t>  point4_t;
//typedef ntk_color3_<real_t>  color3_t;
//typedef ntk_color4_<real_t>  color4_t;
typedef matrix4_<real_t> matrix_t;


struct vertex_t
{
	point3_t point;
	vector3_t coordinate;// uv

	vertex_t() {}
	vertex_t(const point3_t& p, const vector2_t& uv) : point(p), coordinate(uv.x, uv.y, 0.f) {}
	vertex_t(const point3_t& p, const vector3_t& uvw) : point(p), coordinate(uvw) {}
};

struct face_t
{
	enum{ MAX_VERTICES = 4 };

	int num_vertices;
	int vertex_id[MAX_VERTICES];

	face_t() {}
	face_t(int num_ids, int* id_list)
	:	num_vertices(num_ids)
	{
		memcpy(vertex_id, id_list, num_ids * sizeof(int));
	}

	int& operator[](int index) {return vertex_id[index];}
	const int& operator[](int index) const {return vertex_id[index];}
};

struct attribute_t
{
	color3_t diffuse;
};

class object_t {
public:
	object_t() : m_attribute(NULL) {}

	void draw();

	void add_vertex(const point3_t& point, const vector3_t& uv)
	{
		m_vertex_list.push_back(vertex_t(point, uv));
	}
	void add_face(int num_vertices, int id_list[4])
	{
		m_face_list.push_back(face_t(num_vertices, id_list));
	}

private:
	typedef std::vector<vertex_t> vertex_list_t;
	typedef std::vector<face_t> face_list_t;

	vertex_list_t m_vertex_list;
	face_list_t m_face_list;

	attribute_t* m_attribute;

	friend class scene_t;
};

class scene_t {
public:
	void draw();

	void add_object(object_t* obj)
	{
		object_list.push_back(obj);
	}

	bool load_MQO(const std::string& file_name);

private:
	typedef std::list<object_t*> object_list_t;

	object_list_t object_list;
};


#endif//EOH