#include <cmath>
#include <GL/glut.h>
#include <stdlib.h>
#include <cstdio>
#include <string>

static void resize(int width, int height)
{
	const float ar = (float) width / (float) height;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity() ;
}

class Planet {
public:
	std::string name = "";
	double x = 0;
	double y = 0;
	double c1 = 0;
	double c2 = 0;
	double c3 = 0;
	double f = 0;
	double a = 0;
	double s1 = 0;

	Planet(std::string,double,double,double,double,double,double);
	void glStuff(double t, double closeness);

};

// Simple planet constructor
Planet::Planet(std::string name="",double c1=0,double c2=0,double c3=0,double f=0,double a=0,double s1=0)
{
	this->name = name;
	this->x = 0;
	this->y = 0;
	this->c1 = c1;
	this->c2 = c2;
	this->c3 = c3;
	this->f = f;
	this->a = a;
	this->s1 = s1;
}

// Function will compute new planet position
void Planet::glStuff(double t, double closeness)
{
	x = -sin(t*f)*a*1.5;
	y = cos(t*f)*a;

	glColor3d(c1,c2,c3);
	glPushMatrix();
	glTranslated(x,y,closeness);
	glRotated(50.0*t,0,0,1);
	glutSolidSphere(s1,20,20);
	glPopMatrix();
}

static double closeness = -10.0;
Planet** planets;
double moon_x; // moon x position
double moon_y; // moon y position
static double t; // time

static void display(void) // void
{
    t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for(int i = 0; i < 4; i++) {
            if(i!=2){
		(*planets[i]).glStuff(t, closeness);}
    else{
    (*planets[i]).glStuff(-t, closeness);
    }
	}

	// moon orbit calculation
	moon_x = -sin(t)*0.5+ planets[3]->x;
	moon_y = cos(t)*0.5+ planets[3]->y;

    // "Moon"
	glColor3d(0.63137 , 0.62745 , 0.61961);
	glPushMatrix();
	glTranslated(moon_x , moon_y , closeness);
	glRotated(60,1,0,0);
	glRotated(50.0*t,0,0,1);
	glutSolidSphere(0.1,20,20);
	glPopMatrix();

	glutSwapBuffers();
}

static void idle(void)
{
	glutPostRedisplay();
}

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { -20.0f, 10.0f, 0.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 50.0f };

int main(int argc, char *argv[])
{
	planets = new Planet*[4];
	Planet q("sun", 1.00000 , 0.60392 , 0.24314, 0.0, 0.0, 1.5);        planets[0] = &q;
	Planet a("mercury", 0.63216 , 0.63216 , 0.62549, 1.0, 2.0, 0.15);   planets[1] = &a;
	Planet s("venus", 0.93725,0.93725,0.93725, 0.5, 3.0, 0.18);     planets[2] = &s;
	Planet d("earth", 0.41961 , 0.57647 , 1.00000, 0.2, 4.0, 0.25);      planets[3] = &d;

	glutInit(&argc, argv);
	glutInitWindowSize(1000,700);
	glutInitWindowPosition(100,20);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	glutCreateWindow("Sun and First Three Planets");

	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutIdleFunc(idle);

	glClearColor(0,0,0,0);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);

	glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

	glutMainLoop();

	return EXIT_SUCCESS;
}
