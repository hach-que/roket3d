#include "toolbox_stdafx.h"
#include "FloorPlane.h"
#include "ShaderPrograms.h"

FloorPlane::FloorPlane(void)
{
	m_shaderProgram = ShaderPrograms::GetCache().m_solidColor;
}

FloorPlane::~FloorPlane(void)
{
}

void FloorPlane::Render()
{
	if (m_shaderProgram) {
		glUseProgram(m_shaderProgram);
	}

	int steps;
	dFloat spacing;

	steps = 60;
	spacing = 0.5f;

	dVector p0 (- spacing * (steps >> 1), 0.0f, - spacing * (steps >> 1));
	dVector p1 (  spacing * (steps >> 1), 0.0f, - spacing * (steps >> 1));

	glColor3f(0.75f, 0.75f, 0.75f);
	glBegin(GL_LINES);
	for (int i = 0; i <= steps; i ++) {
	
		glVertex3f (p0.m_x, p0.m_y, p0.m_z);
		glVertex3f (p1.m_x, p1.m_y, p1.m_z);

		glVertex3f (p0.m_z, p0.m_y, p0.m_x);
		glVertex3f (p1.m_z, p1.m_y, p1.m_x);

		p0.m_z += spacing;
		p1.m_z += spacing;
	}
	glEnd();
}