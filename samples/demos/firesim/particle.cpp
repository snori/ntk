/******************************************************************************

	The NTK Library : samples/FireSim
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: particle.cpp,v 1.1.1.1 2003/02/26 06:49:06 nsuzuki Exp $

******************************************************************************/
#include "Particle.h"

#include <ntk/interface/dc.h>
#include <ntk/interface/gdiobject.h>

Scene::~Scene()
{
	while(! m_field_list.empty())
	{
		delete m_field_list.back();
		m_field_list.pop_back();
	}

	while(! m_emitter_list.empty())
	{
		delete m_emitter_list.back();
		m_emitter_list.pop_back();
	}

	while(! m_particle_list.empty())
	{
		delete m_particle_list.back();
		m_particle_list.pop_back();
	}
}

void
Scene::draw(ntk::DC& dc)
{
	dc.push_state();
	dc.set_pen(ntk::Pen(ntk::white_color()));

	for(ParticleList::iterator particle = m_particle_list.begin(); particle != m_particle_list.end(); ++particle)
		(*particle)->draw(dc);

	dc.pop_state();
}

void
Scene::next_frame()
{
	for(EmitterList::iterator emitter = m_emitter_list.begin(); emitter != m_emitter_list.end(); ++emitter)
		(*emitter)->emit(this);

	// erase dead particle
	for(ParticleList::iterator particle = m_particle_list.begin(); particle != m_particle_list.end(); ++particle)
	{
		if((*particle)->alive() == false)
		{
			delete *particle;
			m_particle_list.erase(particle);

			particle = m_particle_list.begin();
		}
	}

	for(FieldList::iterator field = m_field_list.begin(); field != m_field_list.end(); ++field)
	for(ParticleList::iterator particle = m_particle_list.begin(); particle != m_particle_list.end(); ++particle)
	{
		(*field)->force(*particle);
	}

	{for(FieldList::iterator field = m_field_list.begin(); field != m_field_list.end(); ++field)
		(*field)->next_frame();
	}

	{for(ParticleList::iterator particle = m_particle_list.begin(); particle != m_particle_list.end(); ++particle)
		(*particle)->next_frame();
	}
}

void
Scene::add_particle(Particle* particle)
{
	m_particle_list.push_back(particle);
	particle->m_scene = this;
}

void
Scene::add_emitter(Emitter* emitter)
{
	m_emitter_list.push_back(emitter);
	emitter->m_scene = this;
}

void
Scene::add_field(Field* field)
{
	m_field_list.push_back(field);
	field->m_scene = this;
}

void
Particle::draw(ntk::DC& dc)
{
	float a = (float)m_age / (float)m_life;

	ntk::RGBColor col(255, 100 + 150 * (1.f - a), 0);
	if(a > 0.7f)
	{
		col.red *= 1.f - a;
		col.green *= 1.f - a;
		col.blue *= 1.f - a;
	}

	dc.set_brush(col);

	Vector& pos = position();
	dc.fill_rect(pos.x -5, pos.y -5, pos.x +5, pos.y +5);
}
