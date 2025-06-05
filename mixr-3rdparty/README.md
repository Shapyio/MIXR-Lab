
Mixed Reality Simulation Platform (MIXR) 3rd Party Libraries
============================================================

The most current version of Mixed Reality simulation platform can be found at [www.mixr-platform.org][mixr].

* This package has been provided as a convenience for compiling with Visual Studio. It contains executables, include files and linkable static libraries.

Executables
-----------

* [mcpp] - portable C preprocessor (can be used as an alternative to Visual Studio C preprocessor - useful if Visual Studio is not installed on target computer)

* [Google Protocol buffers] message compiler

Libraries
---------

* [freetype] - portable font engine

* [FTGL] - library to draw fonts in OpenGL

* [freeglut] - an alternative to the OpenGL Utility Toolkit (GLUT) library

* [CIGI] - Common Image Generator Interface

* [JSBSim] - a C++-based dynamics model

* [Google protocol buffers]

* [OSG] - Open Scene Graph

* [ZeroMQ] - networking library

Notes
-----

* Visual Studio: All libraries have been compiled assuming a multi-threaded DLL-based runtime library.  The supplied libraries have been compiled with:
   * Visual Studio 2015 update 3
   * Visual Studio 2017 15.3.4

* Visual Studio 2015 & 2017
   * To compile Protocol Buffers 2.6.1, you need to define "_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS"
   * To compile OpenSceneGraph, deselect "DYNAMIC_OPENSCENEGRAPH" and "DYNAMIC_OPENTHREADS" within
     cmake GUI is you wish to statically link to compiled libraries.  After solution is opened,
     for each project, set the "Debug Information Format" to "None" to avoid creating a PDB file.

Version Numbers
---------------

#### Graphics support
* [freetype2] - version 2.5.0.1
* [FTGL] - version 2.1.3rc5 (make sure to turn off "Whole Program Optimization")

#### Visual system interface
* [cigicl] - version 3.3.3a

#### Aerodynamics models
* [JSBSim] - pulled from CVS repository 04 July 2015

#### Binary data storage used by data recorder
* [Google protocol buffers] - version 2.6.1

Need to define "_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS" for VS2017, otherwise, library will not compile.

#### Graphical User Interface (GUI) toolkits
* [freeglut] - version 3.0.0

#### Scenegraph
* [OSG] - version 3.4.1

Note: The standard freeglut header file has been modified so that we link against a static library but don't specify its name in the header file.

```
// freeglut_std.h modifications
#ifndef FREEGLUT_STATIC
#define FREEGLUT_STATIC           // we do want to link to a static library
#endif
#ifdef FREEGLUT_LIB_PRAGMAS
#define FREEGLUT_LIB_PRAGMAS 0    // we do not want the default libraries
#endif
```

#### Networking
* [ZeroMQ] - version 4.2.1

[mixr]: http://www.mixr-platform.org
[Premake]: http://industriousone.com/premake
[mcpp]: http://mcpp.sourceforge.net/
[freetype]: http://www.freetype.org/
[FTGL]: http://sourceforge.net/projects/ftgl/
[freeglut]: http://freeglut.sourceforge.net
[CIGI]: http://cigi.sourceforge.net/index.php
[cigicl]: http://cigi.sourceforge.net/index.php
[JSBSIM]: http://www.jsbsim.org
[Google protocol buffers]: http://code.google.com/p/protobuf/
[ZeroMQ]: http://zeromq.org/
[freetype2]: http://www.freetype.org/freetype2/
[OSG]: http://www.openscenegraph.org
