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

/// CTK includes
#include <ctkCheckableHeaderView.h>

// ctkDICOMWidgets includes
#include "ctkDICOMServerNodeWidget.h"
#include "ui_ctkDICOMServerNodeWidget.h"

// STD includes
#include <iostream>

// Qt includes
#include <QList>
#include <QMap>
#include <QVariant>
#include <QSettings>
#include <QTableWidgetItem>

//----------------------------------------------------------------------------
class ctkDICOMServerNodeWidgetPrivate: public Ui_ctkDICOMServerNodeWidget
{
public:
  ctkDICOMServerNodeWidgetPrivate(){}
};

//----------------------------------------------------------------------------
// ctkDICOMServerNodeWidgetPrivate methods


//----------------------------------------------------------------------------
// ctkDICOMServerNodeWidget methods

//----------------------------------------------------------------------------
ctkDICOMServerNodeWidget::ctkDICOMServerNodeWidget(QWidget* _parent):Superclass(_parent),
  d_ptr(new ctkDICOMServerNodeWidgetPrivate)
{
  Q_D(ctkDICOMServerNodeWidget);
 
  d->setupUi(this);

  // checkable headers.
  d->nodeTable->model()->setHeaderData(0, Qt::Horizontal, Qt::Unchecked, Qt::CheckStateRole);
  QHeaderView* previousHeaderView = d->nodeTable->horizontalHeader();
  ctkCheckableHeaderView* headerView = new ctkCheckableHeaderView(Qt::Horizontal, d->nodeTable);
  headerView->setClickable(previousHeaderView->isClickable());
  headerView->setMovable(previousHeaderView->isMovable());
  headerView->setHighlightSections(previousHeaderView->highlightSections());
  headerView->setPropagateToItems(true);
  d->nodeTable->setHorizontalHeader(headerView);

  d->removeButton->setEnabled(false);


  QSettings settings;

  QMap<QString, QVariant> node;
  if ( settings.value("ServerNodeCount").toInt() == 0 )
  {
    node["Name"] = "localhost";
    node["AETitle"] = "CTK_AE";
    node["Address"] = "localhost";
    node["Port"] = "11112";
    settings.setValue("ServerNodeCount", 3);
    settings.setValue("ServerNodes/0", QVariant(node));
    settings.sync();
  }

  int count = settings.value("ServerNodeCount").toInt();
  d->nodeTable->setRowCount(count);
  for (int row = 0; row < count; row++)
  {
    node = settings.value(QString("ServerNodes/%1").arg(row)).toMap();
    QTableWidgetItem *newItem;
    newItem = new QTableWidgetItem( node["Name"].toString() );
    d->nodeTable->setItem(row, 0, newItem);
    newItem = new QTableWidgetItem( node["AETitle"].toString() );
    d->nodeTable->setItem(row, 1, newItem);
    newItem = new QTableWidgetItem( node["Address"].toString() );
    d->nodeTable->setItem(row, 2, newItem);
    newItem = new QTableWidgetItem( node["Port"].toString() );
    d->nodeTable->setItem(row, 3, newItem);
  }

  connect(d->addButton
    ,SIGNAL(clicked()),
    this,
    SLOT(addNode()));
  connect(d->removeButton
    ,SIGNAL(clicked()),
    this,
    SLOT(removeNode()));
  connect(d->nodeTable,
    SIGNAL(cellChanged(int,int)),
    this,
    SLOT(onCellChanged(int,int)));
  connect(d->nodeTable,
    SIGNAL(currentItemChanged(QTableWidgetItem*, QTableWidgetItem*)),
    this,
    SLOT(onCurrentItemChanged(QTableWidgetItem*, QTableWidgetItem*)));
}

//----------------------------------------------------------------------------
ctkDICOMServerNodeWidget::~ctkDICOMServerNodeWidget()
{
}


//----------------------------------------------------------------------------
void ctkDICOMServerNodeWidget::addNode()
{
  Q_D(ctkDICOMServerNodeWidget);

  d->nodeTable->setRowCount( d->nodeTable->rowCount() + 1 );
}

//----------------------------------------------------------------------------
void ctkDICOMServerNodeWidget::removeNode()
{
  Q_D(ctkDICOMServerNodeWidget);

  d->nodeTable->removeRow( d->nodeTable->currentRow() );
  d->removeButton->setEnabled(false);
  this->saveSettings();
}

//----------------------------------------------------------------------------
void ctkDICOMServerNodeWidget::onCellChanged(int row, int column)
{
  Q_UNUSED(row);
  Q_UNUSED(column);

  this->saveSettings();
}

//----------------------------------------------------------------------------
void ctkDICOMServerNodeWidget::onCurrentItemChanged(QTableWidgetItem* current, QTableWidgetItem *previous)
{
  Q_UNUSED(current);
  Q_UNUSED(previous);

  Q_D(ctkDICOMServerNodeWidget);
  if (d->nodeTable->rowCount() > 1)
  {
    d->removeButton->setEnabled(true);
  }
}

//----------------------------------------------------------------------------
void ctkDICOMServerNodeWidget::saveSettings()
{
  Q_D(ctkDICOMServerNodeWidget);

  QSettings settings;
  QMap<QString, QVariant> node;
  int count = d->nodeTable->rowCount();
  QStringList keys;
  keys << "Name" << "AETitle" << "Address" << "Port";
  for (int row = 0; row < count; row++)
  {
    for (int k = 0; k < keys.size(); ++k)
    {
      if ( d->nodeTable->item(row,k) )
      {
        node[keys.at(k)] = d->nodeTable->item(row,k)->text();
      }
      settings.setValue(QString("ServerNodes/%1").arg(row), QVariant(node));
    }
  }
  settings.setValue("ServerNodeCount", count);
  settings.sync();
}

//----------------------------------------------------------------------------
void ctkDICOMServerNodeWidget::populateQuery(/*ctkDICOMQuery &query*/)
{
  //Q_D(ctkDICOMServerNodeWidget);

  std::cerr << "server node populate\n";
}


