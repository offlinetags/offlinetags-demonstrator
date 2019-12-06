/*
Copyright 2012 Markus Koebele (markus-koebele@web.de)

This file is part of Offlinetags.

Offlinetags is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Offlinetags is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "offlinetags.h"

Offlinetags::Offlinetags(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	// Hide menu
	ui.widgetOptions->hide();
	
	// Set style sheet
	QFile file("style.qss");
	file.open(QFile::ReadOnly);
	QString styleSheet = QLatin1String(file.readAll());
	qApp->setStyleSheet(styleSheet);

	QFile options("options.qss");
	options.open(QFile::ReadOnly);
	QString optionsStyle = QLatin1String(options.readAll());
	ui.widgetOptions->setStyleSheet(optionsStyle);

	// Configure timer
	timer = new QTimer(parent);

	// Configure shortcut
	shortcutFullScreen = new QShortcut(QKeySequence("Alt+Return"), this);
	shortcutExitFullScreen = new QShortcut(QKeySequence("Esc"), this);
	shortcutPause = new QShortcut(QKeySequence("Ctrl+Space"), this);
	shortcutOptions = new QShortcut(QKeySequence("Ctrl+Alt+O"), this);
	
#ifdef _DEBUG
	timer->start(1000);
#else
	timer->start(2000); // take picture every 2 seconds
#endif
	// timer
	connect(timer, SIGNAL(timeout()), this, SLOT(analyseImage()));

	// shortcuts
	connect(shortcutFullScreen, SIGNAL(activated()), this, SLOT(toggleFullscreen()));
	connect(shortcutExitFullScreen, SIGNAL(activated()), this, SLOT(exitFullscreen()));
	connect(shortcutPause, SIGNAL(activated()), this, SLOT(pause()));
	connect(shortcutOptions, SIGNAL(activated()), this, SLOT(toggleOptions()));

	// options
	connect(ui.spinBoxDevice, SIGNAL(valueChanged(int)), this, SLOT(setDevice(int)));
	connect(ui.spinBoxIntervall, SIGNAL(valueChanged(int)), this, SLOT(setIntervall(int)));
	connect(ui.checkBoxBlur, SIGNAL(toggled(bool)), this, SLOT(setBlur(bool)));
	connect(ui.spinBoxBlur, SIGNAL(valueChanged(int)), this, SLOT(setBlurValue(int)));
	connect(ui.checkBoxContrast1, SIGNAL(toggled(bool)), this, SLOT(changeContrast1(bool)));
	connect(ui.checkBoxContrast2, SIGNAL(toggled(bool)), this, SLOT(changeContrast2(bool)));
	connect(ui.checkBoxContrast3, SIGNAL(toggled(bool)), this, SLOT(changeContrast3(bool)));
	connect(ui.checkBoxContrast4, SIGNAL(toggled(bool)), this, SLOT(changeContrast4(bool)));
	connect(ui.doubleSpinBoxContrast1, SIGNAL(valueChanged(double)), this, SLOT(setContrast1(double)));
	connect(ui.doubleSpinBoxContrast2, SIGNAL(valueChanged(double)), this, SLOT(setContrast2(double)));
	connect(ui.doubleSpinBoxContrast3, SIGNAL(valueChanged(double)), this, SLOT(setContrast3(double)));
	connect(ui.doubleSpinBoxContrast4, SIGNAL(valueChanged(double)), this, SLOT(setContrast4(double)));
	connect(ui.spinBoxBrightness1, SIGNAL(valueChanged(int)), this, SLOT(setBrightness1(int)));
	connect(ui.spinBoxBrightness2, SIGNAL(valueChanged(int)), this, SLOT(setBrightness2(int)));
	connect(ui.spinBoxBrightness3, SIGNAL(valueChanged(int)), this, SLOT(setBrightness3(int)));
	connect(ui.spinBoxBrightness4, SIGNAL(valueChanged(int)), this, SLOT(setBrightness4(int)));
	connect(ui.checkBoxFaces, SIGNAL(toggled(bool)), this, SLOT(setShowFaces(bool)));
	connect(ui.checkBoxCircle, SIGNAL(toggled(bool)), this, SLOT(setShowCircles(bool)));
	connect(ui.checkBoxTags, SIGNAL(toggled(bool)), this, SLOT(setShowTags(bool)));
	connect(ui.checkBoxResult, SIGNAL(toggled(bool)), this, SLOT(manipulateFaces(bool)));
	connect(ui.spinBoxMinDistance, SIGNAL(valueChanged(int)), this, SLOT(setMinDist(int)));
	connect(ui.spinBoxParam1, SIGNAL(valueChanged(int)), this, SLOT(setParam1(int)));
	connect(ui.spinBoxParam2, SIGNAL(valueChanged(int)), this, SLOT(setParam2(int)));
	connect(ui.spinBoxMinRadius, SIGNAL(valueChanged(int)), this, SLOT(setMinRadius(int)));
	connect(ui.spinBoxMaxRadius, SIGNAL(valueChanged(int)), this, SLOT(setMaxRadius(int)));
	connect(ui.spinBoxThreshold1, SIGNAL(valueChanged(int)), this, SLOT(setThreshold1(int)));
	connect(ui.spinBoxThreshold2, SIGNAL(valueChanged(int)), this, SLOT(setThreshold2(int)));
	connect(ui.checkBoxDillate, SIGNAL(toggled(bool)), this, SLOT(setDillate(bool)));

}

/**
* This slot calls the analyse method and sets its result as image
*/
void Offlinetags::analyseImage(){

	QPixmap image = QPixmap::fromImage(analyzer.getImage());
	miniImages.push_back(image);
	showImage(&image);
}

/**
* Helper method to set an image to the main frame
*/
void Offlinetags::showImage(QPixmap *pixmap){
	int width = ui.widgetImage->width();
	int height = ui.widgetImage->height();
	QPixmap image = pixmap->scaled(ui.widgetImage->width() - IMAGE_BORDER * 2, ui.widgetImage->height() - IMAGE_BORDER * 2, Qt::KeepAspectRatio);
	ui.labelImage->setMinimumSize(image.width() + IMAGE_BORDER, image.height() + IMAGE_BORDER);
	ui.labelImage->setMaximumSize(image.width() + IMAGE_BORDER, image.height() + IMAGE_BORDER);
	ui.labelImage->setPixmap(image);
	updateMiniImages();
}


/**
* Erases old images and connects the images in the miniImages vector to its labels
*/
void Offlinetags::updateMiniImages(){
	
	int num = miniImages.size();

	if(num >= 11)
		miniImages.erase(miniImages.begin()); // do not delete, if there are less than 10 images (+ the actual)
	
	showMiniImage(1, ui.labelMini10);
	showMiniImage(2, ui.labelMini09);
	showMiniImage(3, ui.labelMini08);
	showMiniImage(4, ui.labelMini07);
	showMiniImage(5, ui.labelMini06);
	showMiniImage(6, ui.labelMini05);
	showMiniImage(7, ui.labelMini04);
	showMiniImage(8, ui.labelMini03);
	showMiniImage(9, ui.labelMini02);
	showMiniImage(10, ui.labelMini01);
	
}

/**
* Sets a mini image from the vector to a given label
* This method also fits the perfect number of images in the bottom bar
*/
void Offlinetags::showMiniImage(int number, QLabel *label) {
	if(number <= miniImages.size()){
		QPixmap temp = miniImages[number-1].scaledToHeight(ui.frameMini->height() - 10);
		int numberOfImagesPossible = ui.frameMini->width() / temp.width();
		if((10 - number) < numberOfImagesPossible){
			label->setMinimumSize(temp.size());
			label->setMaximumSize(temp.size());
			label->setPixmap(temp);

			int spacer =  (ui.frameMini->width() - numberOfImagesPossible * temp.width() ) / (numberOfImagesPossible + 1);

			label->move( (temp.width() * (10 - number)) + (spacer * (11 - number)) , 5);
		}
		else{
			label->setMinimumSize(0,0);
			label->setMaximumSize(0,0);
		}
	}	
}

/**
* Change from normal to fullscreen and back
*/
void Offlinetags::toggleFullscreen(){
	if(this->isFullScreen())
		this->showNormal();
	else
		this->showFullScreen();

	showImage(const_cast<QPixmap *>(ui.labelImage->pixmap()));
}

/**
* Disable fullscreen (used for 'Esc')
*/

void Offlinetags::exitFullscreen(){
	if(this->isFullScreen())
		this->showNormal();

	showImage(const_cast<QPixmap *>(ui.labelImage->pixmap()));
}

/**
* Toggles the timer from active to stop and back
*/

void Offlinetags::pause(){
	if(timer->isActive())
		timer->stop();
	else
		timer->start();
}

/**
* Shows the options when hidden and vice versa
*/
void Offlinetags::toggleOptions(){
	if(ui.widgetOptions->isVisible())
		ui.widgetOptions->hide();
	else
		ui.widgetOptions->show();
}

/**
* Sets the intervall for the timer
*/
void Offlinetags::setIntervall(int value){
	timer->setInterval(value);
}

Offlinetags::~Offlinetags()
{
	delete timer;
	timer = 0;
}

/**
* Slots
*/

// Image Loader
void Offlinetags::setDevice(int device){analyzer.loader->setDevice(device);}

// this
void Offlinetags::setBlur(bool value){analyzer.blur = value;}
void Offlinetags::setBlurValue(int value){analyzer.blurValue = value;}
void Offlinetags::changeContrast1(bool value){analyzer.configureContrast1 = value;}
void Offlinetags::changeContrast2(bool value){analyzer.configureContrast2 = value;}
void Offlinetags::changeContrast3(bool value){analyzer.configureContrast3 = value;}
void Offlinetags::changeContrast4(bool value){analyzer.configureContrast4 = value;}
void Offlinetags::setContrast1(double value){analyzer.contrast1 = value;}
void Offlinetags::setContrast2(double value){analyzer.contrast2 = value;}
void Offlinetags::setContrast3(double value){analyzer.contrast3 =value;}
void Offlinetags::setContrast4(double value){analyzer.contrast4 = value;}
void Offlinetags::setBrightness1(int value){analyzer.brightness1 = value;}
void Offlinetags::setBrightness2(int value){analyzer.brightness2 = value;}
void Offlinetags::setBrightness3(int value){analyzer.brightness3 = value;}
void Offlinetags::setBrightness4(int value){analyzer.brightness4 = value;}
void Offlinetags::setShowCircles(bool value){analyzer.showCircles = value;}
void Offlinetags::setShowTags(bool value){analyzer.showTags = value;}
void Offlinetags::manipulateFaces(bool value){analyzer.showManipulation = value;}
void Offlinetags::setShowFaces(bool value){analyzer.showFaces = value;}

//Circle Detector
void Offlinetags::setMinDist(int value){analyzer.circleDetector->minDist = value;}
void Offlinetags::setParam1(int value){analyzer.circleDetector->param1 = value;}
void Offlinetags::setParam2(int value){analyzer.circleDetector->param2 = value;}
void Offlinetags::setMinRadius(int value){analyzer.circleDetector->minRadius = value;}
void Offlinetags::setMaxRadius(int value){analyzer.circleDetector->maxRadius = value;}

// Tag Detector
void Offlinetags::setThreshold1(int value){analyzer.tagDetector->threshold1 = value;}
void Offlinetags::setThreshold2(int value){analyzer.tagDetector->threshold2 = value;}
void Offlinetags::setDillate(bool value){analyzer.tagDetector->dillate = value;}
