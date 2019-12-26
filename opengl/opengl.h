/**
 ***********************************************************************************************************************
 * @file   OPENGL.H
 * @author ZhangRan
 * @date   2019/12/17
 *
 * <h2><center>&copy; COPYRIGHT 2020 SCHRODER CORPORATION</center></h2>
 *
 ***********************************************************************************************************************
 */

#pragma once

#include <QOpenGLWindow>
#include <QOpenGLFunctions>
#include "ui_opengl.h"

class OpenGLWindow : public QOpenGLWindow, private QOpenGLFunctions
{
  Q_OBJECT

public:
  OpenGLWindow();

  virtual void paintGL() override;

private:
  Ui::openglClass ui;
  bool is_initialised_;
};
