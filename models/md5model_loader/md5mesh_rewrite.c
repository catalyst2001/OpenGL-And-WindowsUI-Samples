#include "main.h"
#include "md5model.h"

/*** An MD5 model ***/
struct md5_model_t md5file;

GLfloat g_LighPos[] = { -39.749374, -1.182379, 46.334176, 1.0f };
GLfloat g_LightAmbient[] = { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat g_LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat g_LightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat g_LighAttenuation0 = 1.0f;
GLfloat g_LighAttenuation1 = 0.0f;
GLfloat g_LighAttenuation2 = 0.0f;

void cleanup()
{
	FreeModel(&md5file);
	FreeVertexArrays();
}

void reshape(int w, int h)
{
	if (h == 0)
		h = 1;

	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, w / (GLdouble)h, 0.1, 1000.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void display()
{
	int i;
	static float angle = 0;
	static double curent_time = 0;
	static double last_time = 0;

	last_time = curent_time;
	curent_time = (double)glutGet(GLUT_ELAPSED_TIME) / 1000.0;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glClearColor(0, 0, 0, 0);

	//glPushMatrix();
	// glTranslatef(g_LighPos[0], g_LighPos[1], g_LighPos[2]);
	// glLightfv(GL_LIGHT0, GL_POSITION, g_LighPos);
	// glDisable(GL_LIGHTING);
	// glColor3f(1.0f, 1.0f, 1.0f);
	// glutWireSphere(1.0f, 8, 8);
	// glEnable(GL_LIGHTING);
	//glPopMatrix();


	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glTranslatef(0.0f, -35.0f, -150.0f);
	glRotatef(-90.0f, 1.0, 0.0, 0.0);
	glRotatef(angle, 0.0, 0.0, 1.0);

	angle += 1 * (curent_time - last_time);

	if (angle > 360.0f)
		angle -= 360.0f;

	/* Draw skeleton */
	DrawSkeleton(md5file.baseSkel, md5file.num_joints);

	glColor3ub(128, 170, 128);

	glEnableClientState(GL_VERTEX_ARRAY);

	/* Draw each mesh of the model */
	for (i = 0; i < md5file.num_meshes; ++i)
	{
		PrepareMesh(&md5file.meshes[i], md5file.baseSkel);
		glVertexPointer(3, GL_FLOAT, 0, GetVertsArray());
		glDrawElements(GL_TRIANGLES, md5file.meshes[i].num_tris * 3, GL_UNSIGNED_INT, GetVertsIndices());
	}
	glDisableClientState(GL_VERTEX_ARRAY);

	glutSwapBuffers();
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	/* Escape */
	if (key == 27)
		exit(0);
}

int main(int argc, char *argv[])
{
	//if (argc < 2)
	//{
	//	fprintf(stderr, "usage: %s <filename.md5mesh>\n", argv[0]);
	//	return 0;
	//}

	//glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(1280, 1024);
	glutCreateWindow("MD5 Model");
	atexit(cleanup);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);

	/* Load MD5 model file */
	if (!ReadMD5Model("boblampclean.md5mesh", &md5file))
		exit(EXIT_FAILURE);

	AllocVertexArrays();



	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, g_LightAmbient);
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, g_LightDiffuse);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, g_LightSpecular);
	//glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, g_LighAttenuation0);
	//glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, g_LighAttenuation1);
	//glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, g_LighAttenuation2);
	//glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}
