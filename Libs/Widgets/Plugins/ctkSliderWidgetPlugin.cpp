/*=========================================================================

  Library:   CTK

  Copyright (c) Kitware Inc.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

      http://www.commontk.org/LICENSE

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

=========================================================================*/

// CTK includes
#include "ctkSliderWidgetPlugin.h"
#include "ctkSliderWidget.h"

//-----------------------------------------------------------------------------
ctkSliderWidgetPlugin::ctkSliderWidgetPlugin(QObject *_parent)
        : QObject(_parent)
{

}

//-----------------------------------------------------------------------------
QWidget *ctkSliderWidgetPlugin::createWidget(QWidget *_parent)
{
  ctkSliderWidget* _widget = new ctkSliderWidget(_parent);
  return _widget;
}

//-----------------------------------------------------------------------------
QString ctkSliderWidgetPlugin::domXml() const
{
  return "<widget class=\"ctkSliderWidget\" \
          name=\"SliderSpinBoxWidget\">\n"
          "</widget>\n";
}

// --------------------------------------------------------------------------
QIcon ctkSliderWidgetPlugin::icon() const
{
  return QIcon(":/Icons/sliderspinbox.png");
}

//-----------------------------------------------------------------------------
QString ctkSliderWidgetPlugin::includeFile() const
{
  return "ctkSliderWidget.h";
}

//-----------------------------------------------------------------------------
bool ctkSliderWidgetPlugin::isContainer() const
{
  return false;
}

//-----------------------------------------------------------------------------
QString ctkSliderWidgetPlugin::name() const
{
  return "ctkSliderWidget";
}
