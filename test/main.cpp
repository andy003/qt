/**
 ***********************************************************************************************************************
 * @file   MAIN.CPP
 * @author ZhangRan
 * @date   2019/12/13
 *
 * <h2><center>&copy; COPYRIGHT 2020 SCHRODER CORPORATION</center></h2>
 *
 ***********************************************************************************************************************
 */

#include "test.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    test w;
    w.show();
    return a.exec();
}
