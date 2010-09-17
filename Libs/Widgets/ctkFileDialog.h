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

#ifndef __ctkFileDialog_h
#define __ctkFileDialog_h

// Qt includes
#include <QFileDialog>

// CTK includes
#include <ctkPimpl.h>
#include "CTKWidgetsExport.h"

class ctkFileDialogPrivate;

/// Customizable QFileDialog.
/// An extra widget can be added at the bottom of the dialog
/// under the file format combobox. The Accept button is also controllable
/// using setAcceptButtonEnable()
class CTK_WIDGETS_EXPORT ctkFileDialog : public QFileDialog
{
  Q_OBJECT

public:
  // Superclass typedef
  typedef QFileDialog Superclass;
  /// Constructor
  /// By default, behaves like a QFileDialog
  /// \sa QFileDialog()
  explicit ctkFileDialog(QWidget *parent = 0,
              const QString &caption = QString(),
              const QString &directory = QString(),
              const QString &filter = QString());
  virtual ~ctkFileDialog();
  
  /// Add an extra widget under the file format combobox. If a label is
  /// given, it will appear in the first column.
  /// The widget is reparented to ctkFileDialog
  void setBottomWidget(QWidget* widget, const QString& label=QString());

  /// Return the extra widget if any
  QWidget* bottomWidget()const;

  /// Internally used
  bool eventFilter(QObject *obj, QEvent *event);
public slots:
  /// Can be used to prevent the accept button to be enabled. It's typically
  /// a slot that can be connected to assure that the user doesn't accept the
  /// dialog if a value is not set in the extra bottom widget.
  void setAcceptButtonEnable(bool enable);

protected:
  QScopedPointer<ctkFileDialogPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(ctkFileDialog);
  Q_DISABLE_COPY(ctkFileDialog);
};

#endif
