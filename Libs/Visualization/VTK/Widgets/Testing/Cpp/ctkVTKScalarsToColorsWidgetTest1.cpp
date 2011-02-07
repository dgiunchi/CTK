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

// Qt includes
#include <QApplication>
#include <QSharedPointer>
#include <QTimer>

// CTK includes
#include "ctkVTKScalarsToColorsView.h"
#include "ctkVTKScalarsToColorsWidget.h"

// VTK includes
#include <vtkChartXY.h>
#include <vtkPiecewiseFunction.h>
#include <vtkPlot.h>
#include <vtkSmartPointer.h>

// STD includes
#include <iostream>

//-----------------------------------------------------------------------------
int ctkVTKScalarsToColorsWidgetTest1(int argc, char * argv [] )
{
  QApplication app(argc, argv);

  // Opacity function
  vtkSmartPointer<vtkPiecewiseFunction> opacityFunction =
    vtkSmartPointer<vtkPiecewiseFunction>::New();
  opacityFunction->AddPoint(0.,0.3, 0.5, 0.5);
  opacityFunction->AddPoint(0.2, 0.1, 0.5, 0.5);
  opacityFunction->AddPoint(0.4,0.87, 0.5, 0.5);
  opacityFunction->AddPoint(0.6, 1., 0.5, 0.5);
  opacityFunction->AddPoint(0.8, 0.5, 0.5, 0.5);
  opacityFunction->AddPoint(1.,0.8, 0.5, 0.5);

  ctkVTKScalarsToColorsWidget widget(0);
  // add transfer function item
  vtkPlot* plot = widget.view()->addOpacityFunction(opacityFunction);
  plot->SetColor(0, 67,  247, 255);
  widget.view()->fitAxesToBounds();
  widget.show();

  QTimer autoExit;
  if (argc < 2 || QString(argv[1]) != "-I")
    {
    QObject::connect(&autoExit, SIGNAL(timeout()), &app, SLOT(quit()));
    autoExit.start(1000);
    }
  return app.exec();
}
