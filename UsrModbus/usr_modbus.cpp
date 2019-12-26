/**
 ***********************************************************************************************************************
 * @file    USR_MODBUS.CPP
 * @author  ZhangRan
 * @date    2019/12/24
 * @version 1.0.0
 *
 * <h2><center>&copy; COPYRIGHT 2020 SCHRODER CORPORATION</center></h2>
 *
 ***********************************************************************************************************************
 */

#include <string>

#include "usr_modbus.h"

UsrModbus::UsrModbus(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}

void UsrModbus::setArgc(int argc)
{
  ui.label->setText(std::to_string(argc).c_str());
}

void UsrModbus::setArgv(char* argv)
{
  ui.label_2->setText(QString(argv));
}
