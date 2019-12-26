/**
 ***********************************************************************************************************************
 * @file    MAIN.CPP
 * @author  ZhangRan
 * @date    2019/12/24
 * @version 1.0.0
 *
 * <h2><center>&copy; COPYRIGHT 2020 SCHRODER CORPORATION</center></h2>
 *
 ***********************************************************************************************************************
 */

#include "usr_modbus.h"
#include <QDebug>
#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
  QApplication a(argc, argv);
  UsrModbus w;

  w.setArgc(argc);
  if (argc > 1)
    w.setArgv(argv[1]);
  w.show();
  return a.exec();
}
