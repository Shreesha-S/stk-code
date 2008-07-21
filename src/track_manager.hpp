//  $Id$
//
//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2006 SuperTuxKart-Team
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 3
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#ifndef HEADER_TRACKMANAGER_H
#define HEADER_TRACKMANAGER_H

#include <string>
#include <vector>
#include <map>

class Track;

/** Simple class to load and manage track data, track names and
    such */
class TrackManager
{
private:
    typedef std::vector<Track*>   Tracks;
    Tracks        m_tracks;
    std::map<std::string, std::vector<int> > m_groups;
    std::vector<std::string>                 m_all_groups;
    void          updateGroups(const Track* track);

public:
                  TrackManager();
                 ~TrackManager();

    /** get TrackData by the track ident (aka filename without .track) */
    const std::vector<std::string>&
                  getAllGroups()      const { return m_all_groups;    }
    size_t        getNumberOfTracks() const { return m_tracks.size(); }
    Track        *getTrack(size_t id) const { return m_tracks[id];    }
    Track        *getTrack(const std::string& ident) const;
    const std::vector<int>& 
                  getTracksInGroup(const std::string& g) {return m_groups[g];}
 
    /** load all .track files from all directories */
    void          loadTrackList ();
};

extern TrackManager* track_manager;

#endif
