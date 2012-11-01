/* This file is part of qjson
  *
  * Copyright (C) 2009 Till Adam <adam@kde.org>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Library General Public
  * License as published by the Free Software Foundation; either
  * version 2 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Library General Public License for more details.
  *
  * You should have received a copy of the GNU Library General Public License
  * along with this library; see the file COPYING.LIB.  If not, write to
  * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
  * Boston, MA 02110-1301, USA.
  */

#ifndef QJSON_SERIALIZER_H
#define QJSON_SERIALIZER_H

#include <QByteArray>

class QIODevice;
class QString;
class QVariant;

namespace QJson {
  /**
  * @brief Main class used to convert QVariant objects to JSON data.
  *
  * QVariant objects are converted to a string containing the JSON data.
  * If QVariant object is empty or not valid a <em>null</em> json object is returned.
  */
  class Serializer {
  public:
    Serializer();
    ~Serializer();

     /**
      * This method generates a textual JSON representation and outputs it to the
      * passed in I/O Device.
      * @param variant The JSON document in its in-memory representation as generated by the
      * parser.
      * @param out Input output device
      * @param ok if a conversion error occurs, *ok is set to false; otherwise *ok is set to true
      */
    void serialize( const QVariant& variant, QIODevice* out, bool* ok = 0 );

    /**
      * This is a method provided for convenience. It turns the passed in in-memory
      * representation of the JSON document into a textual one, which is returned.
      * If the returned string is empty, the document was empty. If it was null, there
      * was a parsing error.
      *
      * @param variant The JSON document in its in-memory representation as generated by the
      * parser.
      */

    QByteArray serialize( const QVariant& variant );

  private:
    Q_DISABLE_COPY(Serializer)
    class SerializerPrivate;
    SerializerPrivate* const d;
  };
}

#endif // QJSON_SERIALIZER_H
