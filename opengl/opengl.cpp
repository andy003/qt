/**
 ***********************************************************************************************************************
 * @file   OPENGL.CPP
 * @author ZhangRan
 * @date   2019/12/17
 *
 * <h2><center>&copy; COPYRIGHT 2020 SCHRODER CORPORATION</center></h2>
 *
 ***********************************************************************************************************************
 */

#include "opengl.h"

#include <QtGui/QMatrix4x4>

#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

OpenGLWindow::OpenGLWindow() : is_initialised_(false)
{
  resize(800, 600);

}

void OpenGLWindow::paintGL()
{
  if (!is_initialised_)
  {
    is_initialised_ = true;
    initializeOpenGLFunctions();

    glViewport(0, 0, width(), height());

    //QMatrix4x4 matrix;
    //matrix.perspective(50.0f, 800.0f / 600.0f, 0.1f, 1000.0f);

    glMatrixMode(GL_PROJECTION);  // tell the gpu processer that i would select the projection matrix
    gluPerspective(50.0f, 800.0f / 600.0f, 0.1f, 1000.0f);  // set some values to projection matrix
    glMatrixMode(GL_MODELVIEW);                             // tell .... model view matrix
    glLoadIdentity();

    glClearColor(0.1f, 0.4f, 0.6f, 0.1f);
  }
  glClear(GL_COLOR_BUFFER_BIT);

  glBegin(GL_TRIANGLES);
  glColor4ub(255, 0, 0, 255);
  glVertex3f(0.0f, 0.0f, -10.0f);
  glColor4ub(0, 255, 0, 255);
  glVertex3f(-5.0f, 0.0f, -10.0f);
  glColor4ub(0, 0, 255, 255);
  glVertex3f(-5.0f, -2.0f, -10.0f);
  glEnd();
}
