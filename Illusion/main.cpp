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

void screenReshape(int w, int h) {
    camera->screenReshape(w, h);
}

void motionFunction(int x, int y) {
    camera->mouseMove(x, y);
    glutPostRedisplay();
}

void mouseEvent(int button, int state, int x, int y) {
    camera->mouseEvent(button, state, x, y);
    glutPostRedisplay();
}

void keyboardEvent(unsigned char key, int x, int y) {
    camera->keyboardEvent(key, x, y);
    glutPostRedisplay();
}

void myinit() {
    Object *object = new Object("SimpleVertexShader.frag", "SimpleFragmentShader.frag");
    object->setObjFile("box.obj");
    object->setModelTranslate(glm::vec3(3.0, 0.0, 5.0));
    object->initGeometry();
    camera->addObject(object);
    
    //Add sphere
    Object *globe = new Object("SimpleVertexShader.frag", "SimpleFragmentShader.frag");
    globe->setObjFile("globe.obj");
    globe->setModelScale(glm::vec3(0.2, 0.2, 0.2));
    globe->initGeometry();
    camera->addObject(globe);
    
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.4, 1.0, 1.0, 1.0);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH | GLUT_3_2_CORE_PROFILE);
    
    glutInitWindowSize(1024, 960);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("GLUT Program");
    glutReshapeFunc(screenReshape);
    
    camera = new Camera(CAMERA_ORBIT, 1024, 960);
    myinit();
    
    glutDisplayFunc(display);
    
    //Mouse events
    glutMotionFunc(motionFunction);
    glutMouseFunc(mouseEvent);
    
    //Keyboard
    glutKeyboardFunc(keyboardEvent);
    
    glutMainLoop();
    return 0;
}