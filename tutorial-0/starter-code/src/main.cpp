/*!
@file    	main.cpp
@author  	pghali@digipen.edu
@co-author  parminder.singh@digipen.edu
@date    	17/04/2024

This file uses functionality defined in type GLApp to initialize an OpenGL
context and implement a game loop.

*//*__________________________________________________________________________*/

/*                                                                   includes
----------------------------------------------------------------------------- */
// Extension loader library's header must be included before GLFW's header!!!
#include "../include/glapp.h"
#include <iostream>

#include "recorder.h"
#include "gleq.h"

void event_handler_impl(GLEQevent& event)
{
    switch (event.type)
    {
    case GLEQ_KEY_PRESSED:
    case GLEQ_KEY_RELEASED:
    case GLEQ_KEY_REPEATED: {
        GLApp::key_cb(GLApp::ptr_window, event.keyboard.key,
            event.keyboard.scancode,
            convertGleqToGlfwEvent(event.type),
            event.keyboard.mods);
        break;
    }
    case GLEQ_BUTTON_PRESSED:
    case GLEQ_BUTTON_RELEASED: {
        GLApp::mousebutton_cb(GLApp::ptr_window, event.mouse.button,
            convertGleqToGlfwEvent(event.type),
            event.mouse.mods);
        break;
    }
    case GLEQ_CURSOR_MOVED: {
        GLApp::mousepos_cb(GLApp::ptr_window, event.pos.x, event.pos.y);
        break;
    }
    default:
        break;
    }
}

/*                                                   type declarations
----------------------------------------------------------------------------- */

/*                                                      function declarations
----------------------------------------------------------------------------- */
static void draw();
static void update();
static void init();

/*                                                   objects with file scope
----------------------------------------------------------------------------- */

/*                                                      function definitions
----------------------------------------------------------------------------- */
/*  _________________________________________________________________________ */
/*! main

@param none

@return int

Indicates how the program existed. Normal exit is signaled by a return value of
0. Abnormal termination is signaled by a non-zero return value.
Note that the C++ compiler will insert a return 0 statement if one is missing.
*/
int main(int argc, char* argv[]) { 
  // start with a 16:9 aspect ratio
  ParseArguments& args = ParseArguments::getInstance();
  if (!args.parseArguments(argc, argv)) return 0;

  if (!GLApp::init(args.window_width, args.window_height, "Tutorial 0 | Setting up OpenGL 4.5")) {
    std::cout << "Unable to create OpenGL context" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  auto updateAndDraw = []() { update(); draw(); };
  auto eventHandler = [](GLEQevent event) { event_handler_impl(event); };
  render(GLApp::ptr_window, updateAndDraw, eventHandler, args.type, args.filename);

  GLApp::cleanup();
}

/*  _________________________________________________________________________ */
/*! draw

@param none

@return none

Call application to draw and then swap front and back frame buffers ...
Uses GLHelper::GLFWWindow* to get handle to OpenGL context.
*/
static void draw() {
  // render scene
  GLApp::draw();

  // swap buffers: front <-> back
  // GLApp::ptr_window is handle to window that defines the OpenGL context
  glfwSwapBuffers(GLApp::ptr_window); 
}

/*  _________________________________________________________________________ */
/*! update

@param none

@return none

Make sure callbacks are invoked when state changes in input devices occur.
Ensure time per frame and FPS are recorded.
Let application update state changes (such as animation).
*/
static void update() {
  // process events if any associated with input devices
  glfwPollEvents();

  // main loop computes fps and other time related stuff once for all apps ...
  GLApp::update_time(1.0);

  // animate scene
  GLApp::update();
}
