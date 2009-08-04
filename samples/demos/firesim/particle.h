/******************************************************************************

	The NTK Library : samples/FireSim
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: particle.h,v 1.1.1.1 2003/02/26 06:49:06 nsuzuki Exp $

******************************************************************************/
#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include <vector>
#include <list>
#include <stdio.h>
#include "vector3.h"

typedef ntk::Vector3f Vector;

namespace ntk { class DC; };
class Particle;
class Emitter;
class Field;

//
// utility
//
inline float random() {return ((float)::rand() / (float)RAND_MAX) *2.f - 1.f;}

class Scene {
public:
	//
	// types
	//
	typedef std::list<Particle*> ParticleList;
	typedef std::vector<Emitter*> EmitterList;
	typedef std::vector<Field*> FieldList;

	//
	// methods
	//
	~Scene();

	void draw(ntk::DC& dc);
	void next_frame();

	void add_particle(Particle* particle);
	void add_emitter(Emitter* emitter);
	void add_field(Field* field);

	ParticleList& particle_list() {return m_particle_list;}
	const ParticleList& particle_list() const {return m_particle_list;}

	EmitterList& emitter_list() {return m_emitter_list;}
	const EmitterList& emitter_list() const {return m_emitter_list;}

	FieldList& field_list() {return m_field_list;}
	const FieldList& field_list() const {return m_field_list;}

private:
	ParticleList m_particle_list;
	EmitterList m_emitter_list;
	FieldList m_field_list;

};

class Object {
public:
	Object(const Vector& position) : m_scene(NULL), m_position(position) {}

	Scene* scene() const {return m_scene;}

	Vector& position() {return m_position;}
	const Vector& position() const {return m_position;}

private:
	//
	// data
	//
	Scene* m_scene;
	Vector m_position;

friend class Scene;
};

class Particle : public Object {
public:

	//
	// methods
	//
	Particle(const Vector& position, int life, const Vector& velocity)
	:	Object(position)
	,	m_age(0)
	,	m_life(life)
	,	m_velocity(velocity)
	{
		m_velocity += Vector(random(), random(), random()) * 0.7f;
	}

	void draw(ntk::DC& dc);
	void next_frame()
	{
		position() += m_velocity;
		m_age++;
	}

	//
	// accessors and manipulators
	//
	bool alive() const {return m_age < m_life;}

	int age() const {return m_age;}
	int life() const {return m_life;}

	Vector& velocity() {return m_velocity;}
	const Vector& velocity() const {return m_velocity;}

private:
	//
	// data
	//
	int m_age, m_life;
	Vector m_velocity;

};

class Emitter : public Object {
public:
	Emitter(
		const Vector& position,
		const Vector& direction,
		uint32 amount = 2,
		int32 initial_life = 100,
		const Vector& initial_velocity = Vector(0, 1, 0))
	:	Object(position)
	,	m_amount(amount)
	,	m_initial_life(initial_life)
	,	m_direction(direction)
	,	m_initial_velocity(initial_velocity)
	{}

	void emit(Scene* scene)
	{
		for(int i=0; i<m_amount; ++i)
			scene->add_particle(new Particle(position(), m_initial_life * ((random() * .5f + .5f) * .5f + .5f), direction()));
	}

	Vector& direction() {return m_direction;}
	const Vector& direction() const {return m_direction;}

	uint& amoune() {return m_amount;}
	uint amount() const {return m_amount;}

	uint& initial_life() {return m_initial_life;}
	uint initial_life() const {return m_initial_life;}

	Vector& initial_velocity() {return m_initial_velocity;}
	const Vector& initial_velocity() const {return m_initial_velocity;}

private:
	uint m_amount, m_initial_life;
	Vector m_direction, m_initial_velocity;
};

class Field {
public:
	Field() : m_scene(NULL) {}
	virtual ~Field() {}

	virtual void force(Particle* particle) {}
	virtual void next_frame() {}

	Scene* scene() const {return m_scene;}

private:
	Scene* m_scene;

friend class Scene;
};

class Gravity : public Field {
public:
	void force(Particle* particle)
	{
		particle->velocity().x *= 0.98f;
	}
};

class Noise : public Field {
public:
	void force(Particle* particle)
	{
		particle->velocity() += Vector(random(), random(), random()) * 0.1f * ((float)particle->age() / (float)particle->life());
	}
};

class Wind : public Field {
public:
	Wind()
	:	Field()
	,	m_power(.1f)
	,	m_frame(0)
	{}

	void force(Particle* particle)
	{
		particle->velocity().x += m_power;
	}

	void next_frame()
	{
		++m_frame;

		if(m_frame > 200)
		{
			m_frame = 0;
			m_power = -m_power;
		}
	}

private:
	float m_power;
	uint m_frame;
};

class AirStream : public Field {
public:
	void force(Particle* particle)
	{
		particle->velocity().x *= 0.925f;
//		particle->velocity().y += -0.1f;
	}
#if 0
	void next_frame()
	{
		Scene::ParticleList& list = scene()->particle_list();

		for(Scene::ParticleList::iterator it = list.begin(); it != list.end(); ++it)
		{
			Vector& p = (*it)->position();
			Vector& v = _lattice(p.x, p.y);

			v += (*it)->velocity();
		}
	}
#endif

private:
//	Vector m_lattice[200][200];

//	Vector& _lattice(int x, int y) {return m_lattice[x - 100][y - 150];}
};

#endif