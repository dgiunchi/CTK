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
#include <QRegExp>
#include <QString>
#include <QStringList>

#include "ctkUtils.h"

// STD includes
#include <algorithm>

#ifdef _MSC_VER
  #pragma warning(disable: 4996)
#endif

//------------------------------------------------------------------------------
void ctk::qListToSTLVector(const QStringList& list,
                                 std::vector<char*>& vector)
{
  // Resize if required
  if (list.count() != static_cast<int>(vector.size()))
    {
    vector.resize(list.count());
    }
  for (int i = 0; i < list.count(); ++i)
    {
    // Allocate memory
    char* str = new char[list[i].size()+1];
    strcpy(str, list[i].toLatin1());
    vector[i] = str;
    }
}

//------------------------------------------------------------------------------
namespace
{
/// Convert QString to std::string
static std::string qStringToSTLString(const QString& qstring)
{
  return qstring.toStdString();
}
}

//------------------------------------------------------------------------------
void ctk::qListToSTLVector(const QStringList& list,
                                 std::vector<std::string>& vector)
{
  // To avoid unnessesary relocations, let's reserve the required amount of space
  vector.reserve(list.size());
  std::transform(list.begin(),list.end(),std::back_inserter(vector),&qStringToSTLString);
}

//------------------------------------------------------------------------------
void ctk::stlVectorToQList(const std::vector<std::string>& vector,
                                 QStringList& list)
{
  std::transform(vector.begin(),vector.end(),std::back_inserter(list),&QString::fromStdString);
}

//-----------------------------------------------------------------------------
const char *ctkNameFilterRegExp =
  "^(.*)\\(([a-zA-Z0-9_.*? +;#\\-\\[\\]@\\{\\}/!<>\\$%&=^~:\\|]*)\\)$";
const char *ctkValidWildCard =
  "^[\\w\\s\\.\\*\\_\\~\\$\\[\\]]+$";

//-----------------------------------------------------------------------------
QStringList ctk::nameFilterToExtensions(const QString& nameFilter)
{
  QRegExp regexp(QString::fromLatin1(ctkNameFilterRegExp));
  int i = regexp.indexIn(nameFilter);
  if (i < 0)
    {
    QRegExp isWildCard(QString::fromLatin1(ctkValidWildCard));
    if (isWildCard.indexIn(nameFilter) >= 0)
      {
      return QStringList(nameFilter);
      }
    return QStringList();
    }
  QString f = regexp.cap(2);
  return f.split(QLatin1Char(' '), QString::SkipEmptyParts);
}

//-----------------------------------------------------------------------------
QStringList ctk::nameFiltersToExtensions(const QStringList& nameFilters)
{
  QStringList extensions;
  foreach(const QString& nameFilter, nameFilters)
    {
    extensions << nameFilterToExtensions(nameFilter);
    }
  return extensions;
}

//-----------------------------------------------------------------------------
QString ctk::extensionToRegExp(const QString& extension)
{
  // typically *.jpg
  QRegExp extensionExtractor("\\*\\.(\\w+)");
  int pos = extensionExtractor.indexIn(extension);
  if (pos < 0)
    {
    return QString();
    }
  return ".*\\." + extensionExtractor.cap(1) + "?$";
}

//-----------------------------------------------------------------------------
QRegExp ctk::nameFiltersToRegExp(const QStringList& nameFilters)
{
  QString pattern;
  foreach(const QString& nameFilter, nameFilters)
    {
    foreach(const QString& extension, nameFilterToExtensions(nameFilter))
      {
      QString regExpExtension = extensionToRegExp(extension);
      if (!regExpExtension.isEmpty())
        {
        if (pattern.isEmpty())
          {
          pattern = "(";
          }
        else
          {
          pattern += "|";
          }
        pattern +=regExpExtension;
        }
      }
    }
  if (pattern.isEmpty())
    {
    pattern = ".+";
    }
  else
    {
    pattern += ")";
    }
  return QRegExp(pattern);
}
