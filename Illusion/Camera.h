//
//  Camera.h
//  Illusion
//
//  Created by Prabhat Godse on 3/29/15.
//  Copyright (c) 2015 biodigital. All rights reserved.
//

#ifndef __Illusion__Camera__
#define __Illusion__Camera__

#include <iostream>
#include <vector>
#include <OpenGL/gl3.h>
#include "Object.h"

enum CameraType {
    CAMERA_ORBIT = (1 << 0),
    CAMERA_GAME = (1 << 1)
};

class Camera {
    std::vector<Object*> sceneObjects;
    CameraType _type;
    
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 cameraMatrix;
    glm::mat4 cameraRotation;
    
    glm::vec3 _eyePosition;
    float _zoom;
    
    void initMatrix(float width, float height);
    
    float _width, _height;
    int _prevX, _prevY;
    float _orbitSpeed;
public:
    
    Camera(CameraType type, float width, float height);
    void screenReshape(float w, float h);
    virtual void destroy();
    
    void mouseMove(int x, int y);
    void mouseEvent(int button, int state, int x, int y);
    
    void keyboardEvent(unsigned char key, int x, int y);
    
    void addObject(Object *obj);
    
    virtual void renderCamera();
};

#endif /* defined(__Illusion__Camera__) */
