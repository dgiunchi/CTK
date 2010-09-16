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

#include <ctkEventBusImpl_p.h>

#include <QApplication>
#include <QString>
#include <QStringList>
#include <QDirIterator>
#include <QWidget>
#include <QFileInfo>

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
  QString pluginName;
  if(qApp->arguments().size()>5) {
    pluginName = qApp->arguments().at(5);
  } else {
    pluginName = "org_commontk_eventbus";
    pluginName += QFileInfo(qApp->arguments().at(0)).fileName();
  }

  // try to find the plugin and install all plugins available in 
  // pluginPath (but do not start them)
  QStringList libFilter;
  libFilter << "*.dll" << "*.so" << "*.dylib";
  QDirIterator dirIter(pluginPath, libFilter, QDir::Files);
  bool pluginFound = false;
  QString pluginFileLocation;
  while(dirIter.hasNext()) {
    try {
      QString fileLocation = dirIter.next();
      if (fileLocation.contains("org_commontk_eventbus")) {
        ctkPlugin* plugin = framework->getPluginContext()->installPlugin(QUrl::fromLocalFile(fileLocation));
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

  // setup the communication infrastructure: DicomAppServer and DicomHostService
//  ctkDicomAppServer * appServer = new ctkDicomAppServer(QUrl(appURL).port()); // accesses the app-plugin via getService("ctkDicomAppInterface");
//  ctkDicomHostInterface * hostInterface = new ctkDicomHostService(QUrl(hostURL).port());
//  framework->getPluginContext()->registerService(QStringList("ctkDicomHostInterface"), hostInterface);

  // install and start the plugin with the business logic and remember pointer to start it later
  ctkPlugin* plugin;
  try
  {
    ctkPlugin* plugin = framework->getPluginContext()->installPlugin(QUrl::fromLocalFile(pluginFileLocation));
    plugin->start(ctkPlugin::START_TRANSIENT);
  }
  catch (const ctkPluginException& e)
  {
    qCritical() << e.what();
  }

  framework->start();

  return app.exec();
}
