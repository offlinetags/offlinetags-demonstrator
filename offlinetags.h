/*
Copyright 2012 Markus Koebele (markus-koebele@web.de)

This file is part of Offlinetags.

Offlinetags is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Offlinetags is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef OFFLINETAGS_H
#define OFFLINETAGS_H

#define IMAGE_BORDER 20

#include "ui_offlinetags.h"
#include "ImageAnalyzer.h"


#include <QtGui/QMainWindow>
#include <QFile>
#include <QTimer>
#include <QShortcut>

#include <Vector>

class Offlinetags : public QMainWindow
{
	Q_OBJECT

public:
	Offlinetags(QWidget *parent = 0, Qt::WFlags flags = 0);
	~Offlinetags();

private slots:

	void analyseImage();
	void toggleFullscreen();
	void exitFullscreen();
	void pause();
	void toggleOptions();
	void setIntervall(int value);
	
	// Image Loader
	void setDevice(int device);

	// Image Analyzer
	void setBlur(bool value);
	void setBlurValue(int value);
	void changeContrast1(bool value);
	void changeContrast2(bool value);
	void changeContrast3(bool value);
	void changeContrast4(bool value);
	void setContrast1(double value);
	void setContrast2(double value);
	void setContrast3(double value);
	void setContrast4(double value);
	void setBrightness1(int value);
	void setBrightness2(int value);
	void setBrightness3(int value);
	void setBrightness4(int value);
	void setShowCircles(bool value);
	void setShowTags(bool value);
	void manipulateFaces(bool value);
	void setShowFaces(bool value);


	//Circle Detector
	void setMinDist(int value);
	void setParam1(int value);
	void setParam2(int value);
	void setMinRadius(int value);
	void setMaxRadius(int value);

	// Tag Detector
	void setThreshold1(int value);
	void setThreshold2(int value);
	void setDillate(bool value);


private:
	Ui::OfflinetagsClass ui;

	ImageAnalyzer analyzer;
	QTimer *timer;
	QShortcut *shortcutFullScreen, *shortcutExitFullScreen, *shortcutPause, *shortcutOptions;
	std::vector<QPixmap> miniImages;

	void updateMiniImages();
	void showImage(QPixmap *pixmap);
	void showMiniImage(int number, QLabel *label);

};

#endif // OFFLINETAGS_H
