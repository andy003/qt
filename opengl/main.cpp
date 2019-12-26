/**
 ***********************************************************************************************************************
 * @file   MAIN.CPP
 * @author ZhangRan
 * @date   2019/12/17
 *
 * <h2><center>&copy; COPYRIGHT 2020 SCHRODER CORPORATION</center></h2>
 *
 ***********************************************************************************************************************
 */

#include "opengl.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OpenGLWindow w;
    w.show();
    return a.exec();
}
