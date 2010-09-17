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

#ifndef __ctkDirectoryButton_h
#define __ctkDirectoryButton_h

// Qt includes
#include <QDir>
#include <QFileDialog>

// CTK includes
#include <ctkPimpl.h>
#include "CTKWidgetsExport.h"
class ctkDirectoryButtonPrivate;

/// ctkDirectoryButton is a QPushButton to select a directory path.
/// The absolute path is displayed on the button. When clicked, a
/// file dialog pops up to select a new directory path.
/// \sa QPushButton, QDir
class CTK_WIDGETS_EXPORT ctkDirectoryButton: public QWidget
{
  Q_OBJECT
  Q_PROPERTY(QString directory READ directory WRITE setDirectory)
  Q_PROPERTY(QString caption READ caption WRITE setCaption)
#if QT_VERSION >= 0x040700
  Q_PROPERTY(QFileDialog::Options options READ options WRITE setOptions)
#else
  Q_PROPERTY(Options options READ options WRITE setOptions)
  // QFileDialog::Options is not a meta-type, we need to create our own.
  Q_FLAGS(Option Options);
#endif

public:
#if QT_VERSION < 0x040700
  // QFileDialog::Options is not a meta-type, we need to create our own.
  enum Option
  {
    ShowDirsOnly          = 0x00000001,
    DontResolveSymlinks   = 0x00000002,
    DontConfirmOverwrite  = 0x00000004,
    DontUseSheet          = 0x00000008,
    DontUseNativeDialog   = 0x00000010,
    ReadOnly              = 0x00000020,
    HideNameFilterDetails = 0x00000040
  };
  Q_DECLARE_FLAGS(Options, Option)
#endif

  /// Constructor
  /// Creates a default ctkDirectoryButton that points to the application
  /// current directory.
  ctkDirectoryButton(QWidget * parent = 0);
  /// Constructor
  /// Creates a ctkDirectoryButton that points to the given directory path
  ctkDirectoryButton(const QString& directory, QWidget * parent = 0);
  ctkDirectoryButton(const QIcon& icon, const QString& directory, QWidget * parent = 0);
  
  /// Destructor
  virtual ~ctkDirectoryButton();

  /// Set/get the current directory
  void setDirectory(const QString& directory);
  QString directory()const;

  ///
  /// The title of the file dialog used to select a new directory
  /// If caption is not set, internally use QWidget::tooltip()
  void setCaption(const QString& caption);
  const QString& caption()const;

  /// Options of the file dialog pop up.
  /// \sa QFileDialog::getExistingDirectory
#if QT_VERSION >= 0x040700
  void setOptions(const QFileDialog::Options& options);
  const QFileDialog::Options& options()const;
#else
  void setOptions(const Options& options);
  const Options& options()const;
#endif

public slots:
  /// browse() opens a pop up where the user can select a new directory for the
  /// button. browse() is automatically called when the button is clicked.
  void browse();

signals:
  /// directoryChanged is emitted when the current directory changes.
  /// Programatically or by the user via the file dialog that pop up when 
  /// clicking on the button.
  void directoryChanged(const QString&);
  /// directorySelected() is emitted anytime the current directory is set
  /// (even if the new directory is the same than the current value)
  void directorySelected(const QString&);
protected:
  QScopedPointer<ctkDirectoryButtonPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(ctkDirectoryButton);
  Q_DISABLE_COPY(ctkDirectoryButton);
};

#if QT_VERSION < 0x040700
Q_DECLARE_OPERATORS_FOR_FLAGS(ctkDirectoryButton::Options);
#endif

#endif
