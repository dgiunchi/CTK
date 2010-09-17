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

#ifndef __ctkDICOMDirectoryListWidget_h
#define __ctkDICOMDirectoryListWidget_h

// Qt includes 
#include <QWidget>
#include <QItemSelection>

// ctkDICOMWidgets includes
#include "CTKDICOMWidgetsExport.h"
#include "ctkDICOM.h"

class ctkDICOMDirectoryListWidgetPrivate;

class CTK_DICOM_WIDGETS_EXPORT ctkDICOMDirectoryListWidget : public QWidget
{
  Q_OBJECT

public:
  typedef QWidget Superclass;
  explicit ctkDICOMDirectoryListWidget(QWidget* parent=0);
  void setDICOM(ctkDICOM* dicom);
  virtual ~ctkDICOMDirectoryListWidget();

public slots:
  void addDirectoryClicked();
  void removeDirectoryClicked();
  void directorySelectionChanged( const QItemSelection  & selected, const QItemSelection  & deselected );

protected:
  QScopedPointer<ctkDICOMDirectoryListWidgetPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(ctkDICOMDirectoryListWidget);
  Q_DISABLE_COPY(ctkDICOMDirectoryListWidget);
};

#endif
