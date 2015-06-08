//
//  ShaderComposer.cpp
//  osg_desktop
//
//  Created by Prabhat Godse on 6/7/15.
//  Copyright (c) 2015 Prabhat Godse. All rights reserved.
//

#include "ShaderComposer.h"

#include <osgDB/ReadFile>
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>

osg::Program* ShaderComposer::getOrCreateShaderProgram(int mask) {
    ProgramMap::iterator itr = _programMap.find(mask);
    if (itr == _programMap.end()) {
        osg::Program *program = createShaderProgram(mask);
        _programMap.insert(itr, ProgramMap::value_type(mask, program));
        return program;
    }
    return itr->second;
}

//Basic Shader
static const char *microshaderVertSource = {
    "// microshader - colors a fragment based on its position\n"
    "varying vec4 color;\n"
    "varying vec2 vUVCoord; \n"
    "void main(void)\n"
    "{\n"
    "    vUVCoord = gl_MultiTexCoord0.xy; \n"
    "    color = vec4(gl_Normal, 1.0);\n"
    "    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
    "}\n"
};

static const char *microshaderFragSource = {
    "varying vec4 color;\n"
    "varying vec2 vUVCoord; \n"
    "uniform vec4 ambientColor; \n"
    "uniform sampler2D textureMap; \n"
    "void main(void)\n"
    "{\n"
    "    gl_FragColor = color + texture2D(textureMap, vUVCoord);\n"
    "}\n"
};



osg::Program* ShaderComposer::createShaderProgram(int mask) {
    osg::Shader *vertex = new osg::Shader(osg::Shader::VERTEX);
    osg::Shader *fragment = new osg::Shader(osg::Shader::FRAGMENT);
    
    osg::Program *program = new osg::Program;
    if (mask == BASIC_SHADER) {
        vertex->setShaderSource(microshaderVertSource);
        fragment->setShaderSource(microshaderFragSource);
    } else if (mask == POINT_LIGHTS) {
        vertex->setFileName("PointLight.vert");
        fragment->setFileName("PointLight.frag");
        
        vertex->loadShaderSourceFromFile(vertex->getFileName());
        fragment->loadShaderSourceFromFile(fragment->getFileName());
    }
    
    program->addShader(vertex);
    program->addShader(fragment);
    return program;
}
