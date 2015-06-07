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
#include <osgDB/ReadFile>
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>

//USE_OSGPLUGIN(osg)
//USE_OSGPLUGIN(imageio)

osg::ref_ptr<osgViewer::Viewer> viewer;
osg::observer_ptr<osgViewer::GraphicsWindow> window;

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
        default:
            if (window.valid())
            {
                window->getEventQueue()->keyPress( (osgGA::GUIEventAdapter::KeySymbol) key );
                window->getEventQueue()->keyRelease( (osgGA::GUIEventAdapter::KeySymbol) key );
            }
            break;
    }
}

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
    
    osg::setNotifyLevel(osg::DEBUG_INFO);
    osg::Geode *geode = new osg::Geode();
    geode->addDrawable(new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0.0f,0.0f,0.0f),5),0));
    
    osg::Texture2D* texture = new osg::Texture2D;
    texture->setDataVariance(osg::Object::DYNAMIC); // protect from being optimized away as static state.
    texture->setImage(osgDB::readImageFile("tex.jpg"));
    
    osg::StateSet* stateset = geode->getOrCreateStateSet();
    stateset->setTextureAttributeAndModes(0,texture,osg::StateAttribute::ON);
    
    
    // create the view of the scene.
    viewer = new osgViewer::Viewer;
    window = viewer->setUpViewerAsEmbeddedInWindow(0,0,800,600);
    //viewer->setSceneData(loadedModel.get());
    viewer->setSceneData(geode);
    viewer->setCameraManipulator(new osgGA::TrackballManipulator);
    viewer->addEventHandler(new osgViewer::StatsHandler);
    viewer->realize();
    
    glutMainLoop();
    return 0;
}

/*EOF*/

