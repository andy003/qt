#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_usr_modbus.h"

class UsrModbus : public QMainWindow
{
  Q_OBJECT

public:
  UsrModbus(QWidget* parent = Q_NULLPTR);

  void setArgc(int argc);

  void setArgv(char* argv);

private:
  Ui::UsrModbusClass ui;
};
