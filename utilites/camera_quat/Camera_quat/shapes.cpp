#include "include_camera.h"
#include "shapes.h"

void drawDot(VECTOR3D position, float sradius, COLOUR color)
{
	glPushMatrix();
	glTranslatef(position.x, position.y, position.z);


	VECTOR3D p[4], n[4];
	int STEP = 30;
	for (int i = 0; i<360; i += STEP) {
		for (int j = -90; j<90; j += STEP) {

			p[0].x = sradius * cos(j*DTOR) * cos(i*DTOR);
			p[0].y = sradius * sin(j*DTOR);
			p[0].z = sradius * cos(j*DTOR) * sin(i*DTOR);
			n[0] = p[0];

			p[1].x = sradius * cos((j + STEP)*DTOR) * cos(i*DTOR);
			p[1].y = sradius * sin((j + STEP)*DTOR);
			p[1].z = sradius * cos((j + STEP)*DTOR) * sin(i*DTOR);
			n[1] = p[1];

			p[2].x = sradius * cos((j + STEP)*DTOR) * cos((i + STEP)*DTOR);
			p[2].y = sradius * sin((j + STEP)*DTOR);
			p[2].z = sradius * cos((j + STEP)*DTOR) * sin((i + STEP)*DTOR);
			n[2] = p[2];

			p[3].x = sradius * cos(j*DTOR) * cos((i + STEP)*DTOR);
			p[3].y = sradius * sin(j*DTOR);
			p[3].z = sradius * cos(j*DTOR) * sin((i + STEP)*DTOR);
			n[3] = p[3];

			glBegin(GL_POLYGON);
			if (i % (STEP * 4) == 0)
				glColor3f(color.r, color.g, color.b);
			else
				glColor3f(color.r*0.5, color.g*0.5, color.b*0.5);
			for (int k = 0; k<4; k++) {
				glNormal3f(n[k].x, n[k].y, n[k].z);
				glVertex3f(p[k].x, p[k].y, p[k].z);
			}
			glEnd();
		}
	}

	glPopMatrix();
}

void drawLine(LINE line, COLOUR color, bool doDrawDots) {
	glColor3f(color.r, color.g, color.b);

	glBegin(GL_LINE_STRIP);
	{
		for (std::vector<VECTOR3D>::iterator it = line.P.begin(); it != line.P.end(); ++it) {
			glVertex3f(it->x, it->y, it->z);
		}
	}
	glEnd();

	if (doDrawDots) {
		for (std::vector<VECTOR3D>::iterator it = line.P.begin(); it != line.P.end(); ++it) {
			drawDot(*it, 1, color);
		}
	}
}
void drawAxis() {

    LINE xLine;
    xLine.P.push_back({ 0.0, 0.0, 0.0 });
    xLine.P.push_back({ 1000.0, 0.0, 0.0 });
    LINE yLine;
    yLine.P.push_back({ 0.0, 0.0, 0.0 });
    yLine.P.push_back({ 0.0, 1000.0, 0.0 });
    LINE zLine;
    zLine.P.push_back({ 0.0, 0.0, 0.0 });
    zLine.P.push_back({ 0.0, 0.0, 1000.0 });

    LINE negxLine;
    negxLine.P.push_back({ 0.0, 0.0, 0.0 });
    negxLine.P.push_back({ -1000.0, 0.0, 0.0 });
    LINE negyLine;
    negyLine.P.push_back({ 0.0, 0.0, 0.0 });
    negyLine.P.push_back({ 0.0, -1000.0, 0.0 });
    LINE negzLine;
    negzLine.P.push_back({ 0.0, 0.0, 0.0 });
    negzLine.P.push_back({ 0.0, 0.0, -1000.0 });

    drawLine(xLine, red);
    drawLine(yLine, green);
    drawLine(zLine, blue);

    drawLine(negxLine, { 0.3, 0.1, 0.1 });
    drawLine(negyLine, { 0.1, 0.3, 0.1 });
    drawLine(negzLine, { 0.1, 0.1, 0.3 });
}

void drawGround() {
    LINE xLine;
    LINE zLine;
    int sign = 1;
    for (int i = -100; i < 100; i+=1) {
        if (i != 0) {
            sign *= -1;
            xLine.P.push_back({ sign * -1000.0, 0.0, i*5.0 });
            xLine.P.push_back({ sign * 1000.0, 0.0, i*5.0 });
            zLine.P.push_back({ i*5.0 , 0.0, sign * -1000.0 });
            zLine.P.push_back({ i*5.0 , 0.0, sign * 1000.0 });
        }
    }

    drawLine(xLine, { 0.3, 0.3, 0.3 });
    drawLine(zLine, { 0.3, 0.3, 0.3 });
}


