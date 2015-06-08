//
//  ShaderComposer.h
//  osg_desktop
//
//  Created by Prabhat Godse on 6/7/15.
//  Copyright (c) 2015 Prabhat Godse. All rights reserved.
//

#ifndef __osg_desktop__ShaderComposer__
#define __osg_desktop__ShaderComposer__

#include <stdio.h>
#include <osg/Referenced>
#include <osg/Program>
class ShaderComposer : public osg::Referenced {
public:
    enum ShaderMasks {
        BASIC_SHADER = (1 << 1),
        POINT_LIGHTS = (1 << 2)
    };
    osg::Program *getOrCreateShaderProgram(int mask);
    
    osg::Program* createShaderProgram(int mask);
protected:
    typedef std::map<int, osg::ref_ptr<osg::Program> > ProgramMap;
    ProgramMap _programMap;
};
#endif /* defined(__osg_desktop__ShaderComposer__) */
