#include<bits/stdc++.h>
#include<windows.h>
#include <glut.h>
using namespace std;

#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;
int flag = 0;


struct point{
    double x,y;
};
point P0;


void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}

point Second(stack<point> &S)
{
    point p = S.top();
    S.pop();
    point next = S.top();
    S.push(p);
    return next;
}

int find_bottom(vector<point> p)
{
    int min = 0;
    double min_y = p[0].y;

    for(int i=1;i<p.size();i++){
        if(p[i].y < min_y || (p[i].y == min_y && p[i].x < p[min].x)){
            min = i;
            min_y = p[min].y;
        }
    }
    return min;
}

int orientation(point p, point q, point r)
{
    double val = (q.y-p.y)*(r.x-q.x)-(q.x-p.x)*(r.y-q.y);

    if (val == 0) return 0;
    return (val > 0)? 1: 2;
}

double dist(point p1, point p2)
{
    return pow((p1.x - p2.x),2) + pow((p1.y - p2.y),2);
}


int compare(const void *a,const void *b)
{
    point *p1 = (point *)a;
    point *p2 = (point *)b;

    int orient = orientation(P0,*p1,*p2);

    if(orient==0) return(dist(P0, *p2) >= dist(P0, *p1))? -1 : 1;
    return (orient == 2)? -1: 1;
}

void drawSS()
{
    int n;
    double x,y;
    point p;

    ifstream input;

    input.open("1305073_input9.txt");

    input >> n;
    vector<point> points;

    while(n--){
        input >> x >> y;

        p.x = x;
        p.y = y;

        points.push_back(p);
    }

    glBegin (GL_POINTS);
    for (int i = 0; i < points.size(); i++){
        glColor3f(0,0,1);
        glVertex3f (points[i].x, points[i].y, 0);
    }
    glEnd ();

    int min = find_bottom(points);
    double ymin = points[min].y;

    swap(points[min],points[0]);
    //for(int i=0;i<points.size();i++) cout << points[i].x << " " << points[i].y << endl;
    //cout << "____________________________" << endl;
    P0 = points[0];
    //cout << P0.x << " " << P0.y;
    qsort(&points[1],points.size()-1,sizeof(point),compare);
    //for(int i=0;i<points.size();i++) cout << points[i].x << " " << points[i].y << endl;
    int m = 1;
    for(int i=1;i<points.size();i++){
        while(i<points.size()-1 && orientation(P0,points[i],points[i+1])==0) i++;

        points[m] = points[i];
        m++;
    }
    //cout << "____________________________" << endl;
    points.erase(points.begin()+m,points.end());
    //for(int i=0;i<points.size();i++) cout << points[i].x << " " << points[i].y << endl;
    if(m<3) return;

    stack<point> convex;
    convex.push(points[0]);
    convex.push(points[1]);
    convex.push(points[2]);

    for(int i=3;i<m;i++){
        while(orientation(Second(convex),convex.top(),points[i])!=2) convex.pop();
        convex.push(points[i]);
    }

    //stack<point> d = convex;
    vector<point> P;
    double length = 0;

    while(!convex.empty()){
        point p = convex.top();
        P.push_back(p);
        convex.pop();
    }

    for(int i=0;i<P.size();i++){
        length+=sqrt(dist(P[i],P[(i+1)%P.size()]));
    }

    if(flag==0){
        cout << length << endl;
        for(int i=P.size()-1;i>-1;i--){
            cout << P[i].x << "   " << P[i].y << endl;
        }
        flag = 1;
    }

    for(int i=0;i<P.size();i++){
        glBegin(GL_LINES);{
            glColor3f(1,0,0);
            glVertex3f(P[i].x,P[i].y,0);
            glVertex3f(P[(i+1)%P.size()].x,P[(i+1)%P.size()].y,0);
        }glEnd();
    }
}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
			drawgrid=1-drawgrid;
			break;

		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			cameraHeight -= 3.0;
			break;
		case GLUT_KEY_UP:		// up arrow key
			cameraHeight += 3.0;
			break;

		case GLUT_KEY_RIGHT:
			cameraAngle += 0.03;
			break;
		case GLUT_KEY_LEFT:
			cameraAngle -= 0.03;
			break;

		case GLUT_KEY_PAGE_UP:
			break;
		case GLUT_KEY_PAGE_DOWN:
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}



void display(){

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	gluLookAt(0,0,20,	0,0,0,	0,1,0);
	glMatrixMode(GL_MODELVIEW);

	drawAxes();
    drawSS();
	glutSwapBuffers();
}


void animate(){
	angle+=0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){

	drawgrid=0;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;

	glClearColor(0,0,0,0);
	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(80,	1,	1,	1000.0);

}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("Convex Hull");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
