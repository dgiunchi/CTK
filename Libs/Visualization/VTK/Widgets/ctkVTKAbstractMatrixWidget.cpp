/*=========================================================================

  Library:   CTK
 
  Copyright (c) 2010  Kitware Inc.

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
#include <QDebug>
#include <QVector>

// CTK includes
#include "ctkVTKAbstractMatrixWidget_p.h"
#include "ctkVTKAbstractMatrixWidget.h"

// VTK includes
#include <vtkMatrix4x4.h>

// --------------------------------------------------------------------------
ctkVTKAbstractMatrixWidgetPrivate::ctkVTKAbstractMatrixWidgetPrivate(ctkVTKAbstractMatrixWidget& object)
  :QObject(0) // will be reparented in init()
  ,q_ptr(&object)
{
}

// --------------------------------------------------------------------------
ctkVTKAbstractMatrixWidget::~ctkVTKAbstractMatrixWidget()
{
}

// --------------------------------------------------------------------------
void ctkVTKAbstractMatrixWidgetPrivate::init()
{
  Q_Q(ctkVTKAbstractMatrixWidget);
  this->setParent(q);
  this->updateMatrix();
}

// --------------------------------------------------------------------------
void ctkVTKAbstractMatrixWidgetPrivate::setMatrix(vtkMatrix4x4* matrixVariable)
{
  qvtkReconnect(this->Matrix.GetPointer(), matrixVariable, 
                vtkCommand::ModifiedEvent, this, SLOT(updateMatrix()));

  this->Matrix = matrixVariable;
  this->updateMatrix();
}

// --------------------------------------------------------------------------
vtkMatrix4x4* ctkVTKAbstractMatrixWidgetPrivate::matrix() const
{
  return this->Matrix;
}

// --------------------------------------------------------------------------
void ctkVTKAbstractMatrixWidgetPrivate::updateMatrix()
{
  Q_Q(ctkVTKAbstractMatrixWidget);
  // if there is no transform to show/edit, disable the widget
  q->setEnabled(this->Matrix != 0);

  if (this->Matrix == 0)
    {
    q->reset();
    return;
    }
  QVector<double> vector;
  //todo: fasten the loop
  for (int i=0; i < 4; i++)
    {
    for (int j=0; j < 4; j++)
      {
      vector.append(this->Matrix->GetElement(i,j)); 
      }
    }
  q->setVector( vector );
}

// --------------------------------------------------------------------------
ctkVTKAbstractMatrixWidget::ctkVTKAbstractMatrixWidget(QWidget* parentVariable) : Superclass(parentVariable)
  , d_ptr(new ctkVTKAbstractMatrixWidgetPrivate(*this))
{
  Q_D(ctkVTKAbstractMatrixWidget);
  d->init();
}

// --------------------------------------------------------------------------
vtkMatrix4x4* ctkVTKAbstractMatrixWidget::matrix()const
{
  Q_D(const ctkVTKAbstractMatrixWidget);
  return d->matrix();
}

// --------------------------------------------------------------------------
void ctkVTKAbstractMatrixWidget::setMatrixInternal(vtkMatrix4x4* matrixVariable)
{
  Q_D(ctkVTKAbstractMatrixWidget);
  d->setMatrix(matrixVariable);
}
