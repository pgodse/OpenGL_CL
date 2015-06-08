//
//  main.cpp
//  osg_desktop
//
//  Created by Prabhat Godse on 5/19/15.
//  Copyright (c) 2015 Prabhat Godse. All rights reserved.
//

#include <osg/Config>

#if defined(_MSC_VER) && defined(OSG_DISABLE_MSVC_WARNINGS)
// disable warning "glutCreateMenu_ATEXIT_HACK' : unreferenced local function has been removed"
#pragma warning( disable : 4505 )
#endif

#include <iostream>
#ifdef WIN32
#include <windows.h>
#endif

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>
#include <osgDB/Registry>
#include <osg/ShapeDrawable>
#include <osg/Texture2D>
#include <osg/MatrixTransform>
#include <osgDB/ReadFile>
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>

#include <osgParticle/Particle>
#include <osgParticle/ParticleSystem>
#include <osgParticle/ModularEmitter>
#include <osgParticle/ParticleSystemUpdater>

#include "ShaderComposer.h"

//USE_OSGPLUGIN(osg)
//USE_OSGPLUGIN(imageio)

class CameraHandler : public osgGA::TrackballManipulator {
public:
    void performZoom(float factor) {
        zoomModel(factor);
    }
};

osg::ref_ptr<osgViewer::Viewer> viewer;
osg::observer_ptr<osgViewer::GraphicsWindow> window;
osg::ref_ptr<osg::StateSet> stateset;
osg::ref_ptr<CameraHandler> _cameraHandler;
osg::ref_ptr<ShaderComposer> _shaderComposer;

void display(void)
{
    // update and render the scene graph
    if (viewer.valid()) viewer->frame();
    
    // Swap Buffers
    glutSwapBuffers();
    glutPostRedisplay();
}

void reshape( int w, int h )
{
    // update the window dimensions, in case the window has been resized.
    if (window.valid())
    {
        window->resized(window->getTraits()->x, window->getTraits()->y, w, h);
        window->getEventQueue()->windowResize(window->getTraits()->x, window->getTraits()->y, w, h );
    }
}

void mousebutton( int button, int state, int x, int y )
{
    if (window.valid())
    {
        if (state==0) window->getEventQueue()->mouseButtonPress( x, y, button+1 );
        else window->getEventQueue()->mouseButtonRelease( x, y, button+1 );
    }
}

void mousemove( int x, int y )
{
    if (window.valid())
    {
        window->getEventQueue()->mouseMotion( x, y );
    }
}

void keyboard( unsigned char key, int /*x*/, int /*y*/ )
{
    switch( key )
    {
        case 27:
            // clean up the viewer
            if (viewer.valid()) viewer = 0;
            glutDestroyWindow(glutGetWindow());
            break;
        case 'q':
            _cameraHandler->performZoom(0.1);
            break;
        case 'a':
            _cameraHandler->performZoom(-0.1);
            break;
        default:
            if (window.valid())
            {
                window->getEventQueue()->keyPress( (osgGA::GUIEventAdapter::KeySymbol) key );
                window->getEventQueue()->keyRelease( (osgGA::GUIEventAdapter::KeySymbol) key );
            }
            break;
    }
}

osgParticle::ParticleSystem *create_simple_particle_system(osg::Group *root)
{
    
    // Ok folks, this is the first particle system we build; it will be
    // very simple, with no textures and no special effects, just default
    // values except for a couple of attributes.
    
    // First of all, we create the ParticleSystem object; it will hold
    // our particles and expose the interface for managing them; this object
    // is a Drawable, so we'll have to add it to a Geode later.
    
    osgParticle::ParticleSystem *ps = new osgParticle::ParticleSystem;
    
    // As for other Drawable classes, the aspect of graphical elements of
    // ParticleSystem (the particles) depends on the StateAttribute's we
    // give it. The ParticleSystem class has an helper function that let
    // us specify a set of the most common attributes: setDefaultAttributes().
    // This method can accept up to three parameters; the first is a texture
    // name (std::string), which can be empty to disable texturing, the second
    // sets whether particles have to be "emissive" (additive blending) or not;
    // the third parameter enables or disables lighting.
    
    ps->setDefaultAttributes("", true, false);
    
    // Now that our particle system is set we have to create an emitter, that is
    // an object (actually a Node descendant) that generate new particles at
    // each frame. The best choice is to use a ModularEmitter, which allow us to
    // achieve a wide variety of emitting styles by composing the emitter using
    // three objects: a "counter", a "placer" and a "shooter". The counter must
    // tell the ModularEmitter how many particles it has to create for the
    // current frame; then, the ModularEmitter creates these particles, and for
    // each new particle it instructs the placer and the shooter to set its
    // position vector and its velocity vector, respectively.
    // By default, a ModularEmitter object initializes itself with a counter of
    // type RandomRateCounter, a placer of type PointPlacer and a shooter of
    // type RadialShooter (see documentation for details). We are going to leave
    // these default objects there, but we'll modify the counter so that it
    // counts faster (more particles are emitted at each frame).
    
    osgParticle::ModularEmitter *emitter = new osgParticle::ModularEmitter;
    
    // the first thing you *MUST* do after creating an emitter is to set the
    // destination particle system, otherwise it won't know where to create
    // new particles.
    
    emitter->setParticleSystem(ps);
    
    // Ok, get a pointer to the emitter's Counter object. We could also
    // create a new RandomRateCounter object and assign it to the emitter,
    // but since the default counter is already a RandomRateCounter, we
    // just get a pointer to it and change a value.
    
    osgParticle::RandomRateCounter *rrc =
    static_cast<osgParticle::RandomRateCounter *>(emitter->getCounter());
    
    // Now set the rate range to a better value. The actual rate at each frame
    // will be chosen randomly within that range.
    
    rrc->setRateRange(20, 30);    // generate 20 to 30 particles per second
    
    // The emitter is done! Let's add it to the scene graph. The cool thing is
    // that any emitter node will take into account the accumulated local-to-world
    // matrix, so you can attach an emitter to a transform node and see it move.
    
    root->addChild(emitter);
    
    // Ok folks, we have almost finished. We don't add any particle modifier
    // here (see ModularProgram and Operator classes), so all we still need is
    // to create a Geode and add the particle system to it, so it can be
    // displayed.
    
    osg::Geode *geode = new osg::Geode;
    geode->addDrawable(ps);
    
    // add the geode to the scene graph
    root->addChild(geode);
    
    return ps;
    
}

class UpdateMatrix : public osg::NodeCallback {
    virtual void operator() (osg::Node *node, osg::NodeVisitor *nv) {
        osg::MatrixTransform *trans = dynamic_cast<osg::MatrixTransform*>(node);
        osg::Matrix mat;
        
        float ran = 1.0 + 0.1 * sin(osg::Timer::instance()->time_s());
        
        mat.makeScale(osg::Vec3(ran, ran, ran));
        trans->setMatrix(mat);
    }
};
int main( int argc, char **argv )
{
    glutInit(&argc, argv);
    
//    if (argc<2)
//    {
//        std::cout << argv[0] <<": requires filename argument." << std::endl;
//        return 1;
//    }
//    
//    // load the scene.
//    osg::ref_ptr<osg::Node> loadedModel = osgDB::readNodeFile(argv[1]);
//    if (!loadedModel)
//    {
//        std::cout << argv[0] <<": No data loaded." << std::endl;
//        return 1;
//    }
    
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_ALPHA );
    glutInitWindowPosition( 100, 100 );
    glutInitWindowSize( 800, 600 );
    glutCreateWindow( argv[0] );
    glutDisplayFunc( display );
    glutReshapeFunc( reshape );
    glutMouseFunc( mousebutton );
    glutMotionFunc( mousemove );
    glutKeyboardFunc( keyboard );
    
    osg::setNotifyLevel(osg::FATAL);
    
    //Set shader composer
    _shaderComposer = new ShaderComposer;
    
    osg::Group *sceneRoot = new osg::Group;
    osg::Geode *geode = new osg::Geode();
    sceneRoot->addChild(geode);
    
    geode->addDrawable(new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0.0f,0.0f,0.0f),5),0));
    
    osg::Texture2D* texture = new osg::Texture2D;
    texture->setDataVariance(osg::Object::DYNAMIC); // protect from being optimized away as static state.
    texture->setImage(osgDB::readImageFile("tex.jpg"));
    
    osg::Uniform *texUni = new osg::Uniform(osg::Uniform::SAMPLER_2D, "textureMap");
    texUni->set(0);
    stateset = geode->getOrCreateStateSet();
    stateset->setTextureAttributeAndModes(3,texture,osg::StateAttribute::ON);
    stateset->addUniform(texUni);
    
    stateset->setAttribute(_shaderComposer->getOrCreateShaderProgram(ShaderComposer::POINT_LIGHTS), osg::StateAttribute::ON);
    stateset->addUniform(new osg::Uniform("ambientColor", osg::Vec4(0.3, 0.3, 0.3, 1.0)));
    
    
    osg::Matrix mat;
    mat.makeScale(osg::Vec3(2.0, -1.0, 1.0));
    osg::MatrixTransform *coneTrans = new osg::MatrixTransform(mat);
    coneTrans->addUpdateCallback(new UpdateMatrix);
    
    osg::Geode *cone = new osg::Geode();
    cone->addDrawable(new osg::ShapeDrawable(new osg::Cone(osg::Vec3(13.0, 0.0, 0.0), 4, 5)));
    cone->setStateSet(stateset);
    coneTrans->addChild(cone);
    sceneRoot->addChild(coneTrans);
    
    osgParticle::ParticleSystem *ps1 = create_simple_particle_system(sceneRoot);
    osgParticle::ParticleSystemUpdater *psu = new osgParticle::ParticleSystemUpdater;
    psu->addParticleSystem(ps1);
    sceneRoot->addChild(psu);
    // create the view of the scene.
    viewer = new osgViewer::Viewer;
    window = viewer->setUpViewerAsEmbeddedInWindow(0,0,800,600);
    //viewer->setSceneData(loadedModel.get());
    viewer->setSceneData(sceneRoot);
    
    _cameraHandler = new CameraHandler;
    viewer->setCameraManipulator(_cameraHandler);
    viewer->addEventHandler(new osgViewer::StatsHandler);
    viewer->realize();
    glutMainLoop();
    return 0;
}

/*EOF*/

