#ifndef JEPARCHMENT_H
#define JEPARCHMENT_H

#include "JEGraphicsNew.h"
#include "JELoadTextureNew.h"
#include "JEOGLMathHelper.h"
#include "JEInput.h"
#include "JEPad.h"

namespace JE
{
	class parchment
	{
	public:
		parchment()
		{
			m_size.m_x = .8f;
			m_size.m_y = .3f;
			m_ambient = 0.f;
			m_position = JE::Vector3f(-.01f, 1.1f, 0.f);
		}

		parchment(Vector3f pos, Vector2f size, float ambient)
		{
			m_size = size;
			m_ambient = ambient;
			m_position = pos;
		}

		Vector2f getParchmentSize(void)
		{
			return m_size;
		}

		Vector3f getParchmentPos(void)
		{
			return m_position;
		}

		float getParchAmbi(void)
		{
			return m_ambient;
		}

		void setParchAmbi(float newAmbi)
		{
			m_ambient = newAmbi;
		}

		void setParchPos(Vector3f newPos)
		{
			m_position = newPos;
		}

		void setParchSize(Vector2f newSize)
		{
			m_size = newSize;
		}
	private:
		JE::Vector2f m_size;
		JE::Vector3f m_position;
		float m_ambient;
	};

	void loadParchment(void);
	void initParchment(void);
	void drawParchment(double dt, bool woodDie, bool deerDie, bool shopTuto);
	void unloadParchment(void);
}

#endif