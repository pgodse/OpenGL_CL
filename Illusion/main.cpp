//
//  main.cpp
//  Illusion
//
//  Created by Prabhat Godse on 3/28/15.
//  Copyright (c) 2015 biodigital. All rights reserved.
//

#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <GLUT/glut.h>
#include "Object.h"
#include "Camera.h"

Camera *camera;

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera->renderCamera();
    glFlush();
}

void motionFunction(int x, int y) {
    camera->mouseMove(x, y);
    glutPostRedisplay();
}

void mouseEvent(int button, int state, int x, int y) {
    camera->mouseEvent(button, state, x, y);
    glutPostRedisplay();
}

void myinit() {
    Object *object = new Object("SimpleVertexShader.frag", "SimpleFragmentShader.frag");
    camera->addObject(object);
    
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.4, 1.0, 1.0, 1.0);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH | GLUT_3_2_CORE_PROFILE);
    
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("GLUT Program");
    
    camera = new Camera(CAMERA_ORBIT, 500, 500);
    
    glutDisplayFunc(display);
    
    //Mouse events
    glutMotionFunc(motionFunction);
    glutMouseFunc(mouseEvent);
    
    myinit();
    glutMainLoop();
    return 0;
}