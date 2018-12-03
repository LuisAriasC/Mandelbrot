#include <iostream>
#include "FractalCreator.h"
#include <chrono>
#include <math.h>
#include <complex>

using namespace caveofprogramming;

FractalCreator fractalCreator(M_WIDTH, M_HEIGHT);


using namespace std;

void inicializar(void){
    glClearColor (0.0, 0.0, 0.0, 0.0);
    //glShadeModel (GL_FLAT);
}


void display_poligon(){
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    glPointSize(1.0);
		fractalCreator.drawFractal();
    glutSwapBuffers();
}

void escena(int w, int h){
    glViewport (0, 0, (GLsizei) M_WIDTH, (GLsizei) M_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
		gluOrtho2D(0, M_WIDTH, M_HEIGHT, 0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int x_cent = M_WIDTH / 2;
int y_cent = M_HEIGHT / 2;


void keyboard_mandelbrot(unsigned char key, int x_, int y_){
    switch (key) {
        case 'q':
            exit(0);
            break;

        default:
            break;
    }
}

void mouseClick(int button, int state, int x, int y){

  if (button == GLUT_LEFT_BUTTON) {
    if (state == GLUT_DOWN) {
      cout << "y:" << y << endl;
      fractalCreator.addZoom(x, M_HEIGHT - y, 0.1);
      float cpuTime = 0.0;
      auto start =  chrono::high_resolution_clock::now();
      fractalCreator.run();
      auto end =  chrono::high_resolution_clock::now();
      chrono::duration<float, std::milli> duration_ms = end - start;
      cpuTime = duration_ms.count();
      printf("Runtime: %f\n", cpuTime);
      glutPostRedisplay();
    }
  }

}


int main(int argc, char** argv) {


	//FractalCreator fractalCreator(M_WIDTH, M_HEIGHT);

	fractalCreator.addColorRange(0.0, RGB(0,0,0));
  fractalCreator.addColorRange(0.2, RGB(255,0,0));
  fractalCreator.addColorRange(0.4, RGB(255,0,255));
  fractalCreator.addColorRange(0.6, RGB(0,255,255));
  fractalCreator.addColorRange(0.8, RGB(255,255,0));
  fractalCreator.addColorRange(1.0, RGB(255,255,255));


	//fractalCreator.addZoom(313, 330, 0.1);
  //fractalCreator.addZoom(825, 130, 0.1);

  printf("Runtime.\n");
  float cpuTime = 0.0;
  auto start =  chrono::high_resolution_clock::now();
  fractalCreator.run();
  auto end =  chrono::high_resolution_clock::now();
  chrono::duration<float, std::milli> duration_ms = end - start;
  cpuTime = duration_ms.count();
  printf("Runtime: %f\n", cpuTime);
  //fractalCreator.drawFractal();

  for (int i = 0; i < M_WIDTH; i++) {
    for (int j = 0; j < M_HEIGHT; j++) {
      printf("It : %d\n", fractalCreator.m_histogram[j * M_WIDTH + i]);
    }
  }



	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(M_WIDTH, M_HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Mi ventana de OpenGL");
	inicializar();

	glutKeyboardFunc(keyboard_mandelbrot);
  glutMouseFunc(mouseClick);
	glutDisplayFunc(display_poligon);
	glutReshapeFunc(escena);
	glutMainLoop();

	//fractalCreator.run("test.bmp");

	cout << "Finished." << endl;
	return 0;
}
