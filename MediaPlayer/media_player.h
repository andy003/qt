/**
 ***********************************************************************************************************************
 * @file    MEDIA_PLAYER.H
 * @author  ZhangRan
 * @date    2019/12/24
 * @version 1.0.0
 *
 * <h2><center>&copy; COPYRIGHT 2020 SCHRODER CORPORATION</center></h2>
 *
 ***********************************************************************************************************************
 */

#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_media_player.h"

class MediaPlayer : public QMainWindow
{
    Q_OBJECT

public:
    MediaPlayer(QWidget *parent = Q_NULLPTR);

private:
    Ui::MediaPlayerClass ui;
};
