TEMPLATE	 = app
TARGET		 = ReportMaker

QT		+= sql

FORMS		+= inc/calendar.ui inc/button.ui

HEADERS		+= defs.h parameter.h trend.h dbreader.h menu.h WxMain.h wxGrid.h \
                BoxButton.h BoxCalendar.h BoxObject.h BoxReport.h

SOURCES		+= main.cpp dbreader.cpp menu.cpp WxMain.cpp wxGrid.cpp \
                BoxButton.cpp BoxCalendar.cpp BoxObject.cpp BoxReport.cpp
