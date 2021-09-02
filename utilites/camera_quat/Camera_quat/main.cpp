#include "include_camera.h"



void Display(void);
void Init(void);
void Render(void);
void Lighting(void);
void InitCamera(int);
void HandleKeyboard(unsigned char key,int x, int y);
void HandleMouseMotion(int x, int y);
void HandleMousePassiveMotion(int x, int y);
void HandleReshape(int,int);
void HandleIdle(void);

void UpdateCameraState();
void DrawScene();
void InitScene();

int fullscreen = FALSE;

int currentbutton = -1;
double rotatespeed = 3;
double tSpeed = 0.05;
float t = 0;

CPlane3 plane(0.f, 0.f, 0.f, 0.f, 1.f, 0.f);

VECTOR3D movement = { 0, 0, 0 };

CAMERA camera;
FRUSTUM centerFrustum;

int lastX = -1, lastY = -1;
double rotateangle = 0;
unsigned long long currentTime = 0;

std::vector<ROTATING_DOT> dots;

int main(int argc, char **argv)
{
    camera.screenwidth = 600;
    camera.screenheight = 400;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutCreateWindow("Geometria Proyectiva");
    if (fullscreen)
        glutFullScreen();
    glutDisplayFunc(Display);
    glutReshapeFunc(HandleReshape);
    glutReshapeWindow(camera.screenwidth, camera.screenheight);
    glutIdleFunc(HandleIdle);
    glutKeyboardFunc(HandleKeyboard);
    glutMotionFunc(HandleMouseMotion);
    glutPassiveMotionFunc(HandleMousePassiveMotion);
    Init();
    InitCamera(0);
    InitScene();
    Lighting();
    
    glutMainLoop();
    return(0);
}

void Init(void)
{
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_POINT_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glShadeModel(GL_SMOOTH);
    glDisable(GL_DITHER);
    glDisable(GL_CULL_FACE);
    
    glLineWidth(1.0);
    glPointSize(1.0);
    
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glFrontFace(GL_CW);
    glClearColor(0.0,0.0,0.0,0.0);         /* Background colour */
    glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
}

void Display(void)
{
    glDrawBuffer(GL_BACK_LEFT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    
    glDrawBuffer(GL_BACK);
    
	double nearValue = 0.1;
	double farValue = 10000;
    
    double aspectRatio  = camera.screenwidth / (double)camera.screenheight;
    FRUSTUM centerFrustum = makeFrustum(camera.aperture, aspectRatio, nearValue, farValue);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	//gluPerspective(camera.aperture, aspectRatio, nearValue, farValue);//QUITAR
	glFrustum(centerFrustum.left, centerFrustum.right, centerFrustum.bottom, centerFrustum.top, centerFrustum.nearValue, centerFrustum.farValue);
    
    glMatrixMode(GL_MODELVIEW);
    
    glLoadIdentity();
    UpdateCameraState();
    VECTOR3D target = Add(camera.position, camera.direction);
    gluLookAt(camera.position.x,camera.position.y,camera.position.z, target.x , target.y, target.z, camera.up.x,camera.up.y,camera.up.z); //QUITAR
    

    //MATRIX4 lookAtMatrix = lookAt(camera.position, target, camera.up);
    //glLoadMatrixf(lookAtMatrix.m);

    glViewport(0,0,camera.screenwidth,camera.screenheight);
    
    Render();
    
    glutSwapBuffers();
}


void Render(void) {

    GLfloat specular[4] = {1.0,1.0,1.0,1.0};
    GLfloat shiny[1] = {5.0};
    
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS, shiny);
    
    glPushMatrix();
    glRotatef(rotateangle,0.0,1.0,0.0);

    DrawScene();
    
    glPopMatrix();
}

void Lighting(void) {

    GLfloat fullambient[4] = {1.0,1.0,1.0,1.0};
    GLfloat position[4] = {0.0,0.0,0.0,0.0};
    GLfloat ambient[4]  = {0.2,0.2,0.2,1.0};
    GLfloat diffuse[4]  = {1.0,1.0,1.0,1.0};
    GLfloat specular[4] = {0.0,0.0,0.0,1.0};
    
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHT1);
    glDisable(GL_LIGHT2);
    glDisable(GL_LIGHT3);
    glDisable(GL_LIGHT4);
    glDisable(GL_LIGHT5);
    glDisable(GL_LIGHT6);
    glDisable(GL_LIGHT7);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_FALSE);
    
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT,fullambient);
    glLightfv(GL_LIGHT0,GL_POSITION,position);
    glLightfv(GL_LIGHT0,GL_AMBIENT,ambient);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuse);
    glLightfv(GL_LIGHT0,GL_SPECULAR,specular);
    glEnable(GL_LIGHT0);
    
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
}

void HandleKeyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case ESC:
        case 'Q':
        case 'q':
            exit(0);
            break;
        case 'R':
        case 'r':
            rotateangle += rotatespeed;
            break;
            
        case '+':
            t += tSpeed;
            t = t > 1 ? 1:t;
            break;

        case '-':
            t -= tSpeed;
            t = t < 0 ? 0 : t;
            break;

        case 'h':
        case 'H':
            InitCamera(0);
            break;
        case 'w':
        case 'W':
            movement.z = 1;
            break;
        case 'a':
        case 'A':
            movement.x = 1;
            break;
        case 's':
        case 'S':
            movement.z = -1;
            break;
        case 'd':
        case 'D':
            movement.x = -1;
            break;
        case ' ':
            movement.y = 1;
            break;
        case 'm':
        case 'M':
			movement.y = -1;
			break;
        case 'z':
        case 'Z':
            movement.y = 1;
            break;
    }

}
// when mouse down
void HandleMouseMotion(int x, int y) {
    //if (lastX == -1 || lastY == -1) {
    //    lastX = x;
    //    lastY = y;
    //    return;
    //}
	//
    //int deltaX = (lastX - x);
    //int deltaY = (lastY - y);
    //lastX = x;
    //lastY = y;
	//
    //camera.euler.yaw += deltaX * 0.1f;
    //camera.euler.pitch += deltaY * 0.1f;
	//
    //printf("%f, %f\n", deltaX * 0.1f, deltaY * 0.1f);
}
// when mouse is release state
void HandleMousePassiveMotion(int x, int y) {
	//if (lastX == -1 || lastY == -1) {
	//	lastX = x;
	//	lastY = y;
	//	return;
	//}
	//
	//int deltaX = ((camera.screenwidth >> 1) - x);
	//int deltaY = ((camera.screenheight >> 1) - y);
	//
	//camera.euler.yaw += deltaX * 0.1f;
	//camera.euler.pitch += deltaY * 0.1f;
	//SetCursorPos(camera.screenwidth >> 1, camera.screenheight >> 1);
}

void move()
{
	POINT pp;
	GetCursorPos(&pp);
	int deltaX = ((camera.screenwidth >> 1) - pp.x);
	int deltaY = ((camera.screenheight >> 1) - pp.y);
	camera.euler.yaw += deltaX * -0.1f;
	camera.euler.pitch += deltaY * -0.1f;
	SetCursorPos(camera.screenwidth >> 1, camera.screenheight >> 1);
}

void HandleIdle(void)
{
//    rotateangle += rotatespeed;
    glutPostRedisplay();

}

void HandleReshape(int w,int h)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0,0,(GLsizei)w,(GLsizei)h);
    camera.screenwidth = w;
    camera.screenheight = h;
}


void InitCamera(int mode)
{
    camera.aperture = 45;

    camera.position.x = 5;
    camera.position.y = 10;
    camera.position.z = 20;
    
    camera.euler = { -25, 25, 0, {0, {0, 0, 0}}};
    updateEulerOrientation(camera.euler);
    camera.direction = getForward(camera.euler);
    camera.up = getUp(camera.euler);
}


void UpdateCameraState() {
    updateEulerOrientation(camera.euler);
    camera.direction = getForward(camera.euler);
    camera.up = getUp(camera.euler);

    movement = Normalize(movement);

    // updateForwardMovement
    camera.position = Add(camera.position, MultiplyWithScalar(movement.z, camera.direction));
    // updateUpMovement
    camera.position = Add(camera.position, MultiplyWithScalar(movement.y, camera.up));
    // updateSideMovement
    VECTOR3D sideVector = Normalize(CrossProduct(camera.up, camera.direction));
    camera.position = Add(camera.position, MultiplyWithScalar(movement.x, sideVector));

    movement = { 0, 0, 0 };
}

void DrawScene() {
	move();
    drawGround();
    drawAxis();
    auto start = std::chrono::high_resolution_clock::now().time_since_epoch();
    currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(start).count();

	printf("( %f %f %f )  ( %f %f %f )\n", camera.position.x, camera.position.y, camera.position.z, camera.direction.x, camera.direction.y, camera.direction.z);

	CRay ray;
	ray.SetOrigin(camera.position.x, camera.position.y, camera.position.z);
	ray.SetDirection(camera.direction.x, camera.direction.y, camera.direction.z);

	vec3 intersect;
	if (ray.PlaneIntersection5(plane.m_origin, plane.m_normal, ray.m_origin, ray.m_direction, intersect)) {

		drawDot({ intersect.x,intersect.y,intersect.z }, 1.0f, {0,255,0});


	}

    for (std::vector<ROTATING_DOT>::iterator it = dots.begin(); it != dots.end(); ++it) {
        (*it).pos = RotateWithQuaternion((*it).pos, (*it).q);
        VECTOR3D interpolation = { 1, sin(currentTime * 0.002), 1 };
        drawDot({ Multiply(interpolation, (*it).pos) }, (*it).radius, (*it).color);
    }

}

void InitScene() {
    EULER dotEulerRotation1 = { 1, 0, 0,{ 0,{ 0, 0, 0 } } };
    EULER dotEulerRotation2 = { -1, 0, 0,{ 0,{ 0, 0, 0 } } };
    updateEulerOrientation(dotEulerRotation1);
    updateEulerOrientation(dotEulerRotation2);

    auto start = std::chrono::high_resolution_clock::now().time_since_epoch();
    currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(start).count();


    dots.push_back({ { 5.0, 5.0, -5.0 }, 0.3f, { 0.5, 0, 0.7 }, dotEulerRotation1.orientation });
    dots.push_back({ { -5.0, 5.0, -5.0 }, 0.3f, { 0.5, 0, 0.7 }, dotEulerRotation1.orientation });
    dots.push_back({ { -5.0, 5.0, 5.0 }, 0.3f, { 0.5, 0, 0.7 }, dotEulerRotation1.orientation });
    dots.push_back({ { 5.0, 5.0, 5.0 }, 0.3f, { 0.5, 0, 0.7 }, dotEulerRotation1.orientation });

    dots.push_back({ { 3.0, 3.0, -3.0 }, 0.3f,{ 0.5, 0.7, 0 }, dotEulerRotation2.orientation });
    dots.push_back({ { -3.0, 3.0, -3.0 }, 0.3f,{ 0.5, 0.7, 0 }, dotEulerRotation2.orientation });
    dots.push_back({ { -3.0, 3.0, 3.0 }, 0.3f,{ 0.5, 0.7, 0 }, dotEulerRotation2.orientation });
    dots.push_back({ { 3.0, 3.0, 3.0 }, 0.3f,{ 0.5, 0.7, 0 }, dotEulerRotation2.orientation });
}