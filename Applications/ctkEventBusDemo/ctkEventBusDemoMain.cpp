/*=============================================================================

  Library: CTK

  Copyright (c) 2010 BioComputing Competence Centre - Super Computing Solutions

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

=============================================================================*/

#include <ctkPluginFrameworkFactory.h>
#include <ctkPluginFramework.h>
#include <ctkPluginException.h>
#include <ctkServiceReference.h>

#include <EventBus/ctkEventBus.h>

#include <QApplication>
#include <QString>
#include <QStringList>
#include <QDirIterator>
#include <QWidget>
#include <QFileInfo>

#include "MainWindow.h"

int main(int argv, char** argc) {
  QApplication app(argv, argc);
  qDebug() << "################################################################";

  qApp->setOrganizationName("CTK");
  qApp->setOrganizationDomain("commontk.org");
  qApp->setApplicationName("ctkEventBusDemoApp");

  qDebug() << "################################################################";

  // setup the plugin framework
  ctkPluginFrameworkFactory fwFactory;
  ctkPluginFramework* framework = fwFactory.getFramework();

  try {
    framework->init();
  } catch (const ctkPluginException& exc) {
    qCritical() << "Failed to initialize the plug-in framework:" << exc;
    exit(2);
  }

#ifdef CMAKE_INTDIR
  QString pluginPath = qApp->applicationDirPath() + "/../plugins/" CMAKE_INTDIR "/";
#else
  QString pluginPath = qApp->applicationDirPath() + "/plugins/";
#endif

  qApp->addLibraryPath(pluginPath);

  // construct the name of the plugin with the business logic
  // (thus the actual logic of the hosted app)
  QString pluginName = "org_commontk_eventbus";


  // try to find the plugin and install all plugins available in 
  // pluginPath (but do not start them)
  QStringList libFilter;
  libFilter << "*.dll" << "*.so" << "*.dylib";
  QDirIterator dirIter(pluginPath, libFilter, QDir::Files);
  bool pluginFound = false;
  QString pluginFileLocation;
  ctkPlugin* plugin;
  while(dirIter.hasNext()) {
    try {
      QString fileLocation = dirIter.next();
      if (fileLocation.contains("org_commontk_eventbus")) {
        plugin = framework->getPluginContext()->installPlugin(QUrl::fromLocalFile(fileLocation));
        plugin->start(ctkPlugin::START_TRANSIENT);
      }
      if (fileLocation.contains(pluginName)) {
        pluginFound = true;
        pluginFileLocation = fileLocation;
      }
    } catch (const ctkPluginException& e) {
      qCritical() << e.what();
    }
  }

  // if we did not find the business logic: abort
  if(!pluginFound) {
    qCritical() << "Could not find plugin.";
    qCritical() << "  Plugin name: " << pluginName;
    qCritical() << "  Plugin path: " << pluginPath;
    exit(3);
  }


  ctkServiceReference *ebr = framework->getPluginContext()->getServiceReference("ctkEventBus");
  ctkEventBus * eb = (ctkEventBus *)framework->getPluginContext()->getService(ebr);

  framework->start();

  MainWindow win;
  win.show();

  return app.exec();
}
