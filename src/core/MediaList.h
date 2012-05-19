/****************************************************************************
* VLC-Qt - Qt and libvlc connector library
* Copyright (C) 2012 Tadej Novak <tadej@tano.si>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#ifndef VLCQT_MEDIALIST_H_
#define VLCQT_MEDIALIST_H_

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QUrl>

#include "Enums.h"

class VlcInstance;
class VlcMedia;

struct libvlc_media_list_t;

/*!
    \class VlcMediaList Media.h vlc-qt/Media.h
    \brief Media list item

    VlcMediaList holds multiple VlcMedia items to play in sequence.
*/
class VlcMediaList : public QObject
{
Q_OBJECT
public:
    /*!
        \brief VlcMediaList constructor.

        This constructor creates a new media list instance.

        \param instance main libvlc instance
    */
    explicit VlcMediaList(VlcInstance *instance);

    /*!
        \brief VlcMediaList destructor
    */
    ~VlcMediaList();

    /*!
        \brief libvlc media list item
        \return libvlc media list item (libvlc_media_list_t *)
    */
    libvlc_media_list_t *core();

    /*!
        \brief libvlc media list item
        \return items count (int)
    */
    int count();

    /*!
        \brief Add media item to the list
        \param media media item
    */
    void addMedia(VlcMedia *media);

    /*!
        \brief Insert media item at the specific position of the list.
        \param media media item
        \param index item position
    */
    void insertMedia(VlcMedia *media,
                     const int &index);

    /*!
        \brief Remove media item from the specific position of the list.
        \param index item position
    */
    void removeMedia(const int &index);


private:
    void lock();
    void unlock();

    libvlc_media_list_t * _vlcMediaList;
};

#endif // VLCQT_MEDIALIST_H_