/*=============================================================================

  Library: CTK

  Copyright (c) 2010 BioComputing Competence Centre - Super Computing Solutions,


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

#ifndef CTKSEARCHFILTER_H
#define CTKSEARCHFILTER_H

#include <QMap>
#include <QString>
#include <QVariant>
#include "ctkPluginFramework_global.h"

class ctkSearchFilter {

public:

  virtual ~ctkSearchFilter() {}

  virtual bool match(const ctkDictionary& dictionary) const = 0;
  virtual bool matchCase(const ctkDictionary& dictionary) const = 0;

};

Q_DECLARE_INTERFACE(ctkSearchFilter, "org.commontk.core.ctkSearchFilter")

#endif // CTKSEARCHFILTER_H
