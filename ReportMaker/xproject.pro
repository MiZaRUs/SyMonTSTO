TEMPLATE	 = app
TARGET		 = ReportMaker

QT		+= sql

FORMS		+= inc_ui/calendar.ui inc_ui/button.ui

HEADERS		+= ../server_sd/defs.h parameter.h trend.h dbreader.h WxMain.h wxGrid.h \
                BoxButton.h BoxCalendar.h BoxObject.h BoxReport.h

SOURCES		+= main.cpp dbreader.cpp WxMain.cpp wxGrid.cpp \
                BoxButton.cpp BoxCalendar.cpp BoxObject.cpp BoxReport.cpp
