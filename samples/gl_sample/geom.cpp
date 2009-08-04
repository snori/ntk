#include "geom.h"


#include <ntk/support/string.h>


#define for if(false);else for


//########################################################


static void clean_up(char* str)
{
	while(*str++ != '\0')
		if(*str == '(' || *str == ')')
			*str = ' ';
}

void
object_t::draw()
{
	if(m_attribute)
		glColor3f(m_attribute->diffuse[0], m_attribute->diffuse[1], m_attribute->diffuse[2]);

	for(face_list_t::iterator it = m_face_list.begin(); it != m_face_list.end(); ++it)
	{
		face_t& face = *it;
		if(face.num_vertices != 3 && face.num_vertices != 4)
			continue;

		glBegin(GL_POLYGON);
		{
			vector3_t normal = cross_product(
				(m_vertex_list[face.vertex_id[0]].point - m_vertex_list[face.vertex_id[1]].point),
				(m_vertex_list[face.vertex_id[1]].point - m_vertex_list[face.vertex_id[2]].point));

			normal.normalize();
			normal = -normal;
			glNormal3f(normal.x, normal.y, normal.z);

			for(int i=0; i < face.num_vertices; ++i)
			{
				int vertex_id = face.vertex_id[i];
				glVertex3f(
					m_vertex_list[vertex_id].point[0],
					m_vertex_list[vertex_id].point[1],
					m_vertex_list[vertex_id].point[2]);
			}
		}
		glEnd();
	}
}


//########################################################


void
scene_t::draw()
{
	for(object_list_t::iterator it = object_list.begin(); it != object_list.end(); ++it)
		(*it)->draw();
}

bool
scene_t::load_MQO(const std::string& file_name)
{
	FILE* fp = fopen(file_name.c_str(), "r");
	if(fp == NULL)
	{
		printf("cannot open $s!\n", file_name.c_str());
		return false;
	}

	char line[1024], tmp_str[1024], *p;
	int count, tmp_int;

	#define NEXTLINE() fgets(line, 1024, fp);p = line;
	#define SKIPSPACE() while(*p == ' ' || *p == '\t') ++p;

	while(true)
	{
		char* ret;
		do{
			ret = NEXTLINE();
			SKIPSPACE();

			if(ret == NULL)
				return true;// 読み込み終了
		} while(strncmp(p, "Object", 6) != 0);// Object行

		object_t* obj = new object_t;
		object_list.push_back(obj);

		count = 4;// 4行読み飛ばし
		while(count--)
			NEXTLINE();

		NEXTLINE();
		obj->m_attribute = new attribute_t;
		sscanf(p, "%s %f %f %f", &tmp_str,
			&obj->m_attribute->diffuse[0],
			&obj->m_attribute->diffuse[1],
			&obj->m_attribute->diffuse[2]);// { まで

		NEXTLINE();// 一行読み飛ばし

		NEXTLINE();
		int num_vertices = 0;
		sscanf(p, "%s %d %s", &tmp_str, &num_vertices, &tmp_str);// { まで
		// vertices
		obj->m_vertex_list.resize(num_vertices);

		for(int i=0; i<num_vertices; ++i)
		{
			NEXTLINE();
			sscanf(
				p,
				"%f %f %f",
				&obj->m_vertex_list[i].point[0],
				&obj->m_vertex_list[i].point[1],
				&obj->m_vertex_list[i].point[2]
				);

//			obj->m_vertex_list[i].point[0] *= -1;
			obj->m_vertex_list[i].point /= 100;
		}

		NEXTLINE();// 一行読み飛ばし

		// faces
		NEXTLINE();
		int num_faces = 0;
		sscanf(p, "%s %d %s", &tmp_str, &num_faces, &tmp_str);// { まで
		obj->m_face_list.resize(num_faces);

		for(int i=0; i<num_faces; ++i)
		{
			NEXTLINE();
			SKIPSPACE();

			clean_up(line);

			sscanf(p, "%d", &num_vertices);
			obj->m_face_list[i].num_vertices = num_vertices;

			if(num_vertices == 4)
			{
				sscanf(
					p,
					"%d %s %d %d %d %d",
					&tmp_int,
					&tmp_str,
					&obj->m_face_list[i][0],
					&obj->m_face_list[i][1],
					&obj->m_face_list[i][2],
					&obj->m_face_list[i][3]);
			}
			else if(num_vertices == 3)
			{
				sscanf(
					p,
					"%d %s %d %d %d",
					&tmp_int,
					&tmp_str,
					&obj->m_face_list[i][0],
					&obj->m_face_list[i][1],
					&obj->m_face_list[i][2]);
			}
		}
	}
}
