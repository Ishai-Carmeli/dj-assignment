#include "DJLibraryService.h"
#include "SessionFileParser.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>
#include <filesystem>


DJLibraryService::DJLibraryService(const Playlist& playlist) 
    : playlist(playlist) {}

DJLibraryService::~DJLibraryService() {
    for (AudioTrack* track : library) {
        delete track;
    }
}


/**
 * @brief Load a playlist from track indices referencing the library
 * @param library_tracks Vector of track info from config
 */
void DJLibraryService::buildLibrary(const std::vector<SessionConfig::TrackInfo>& library_tracks) {
    //Todo: Implement buildLibrary method

    for (size_t i = 0; i < library_tracks.size(); ++i) {
        const auto& t = library_tracks[i];
        if (t.type == "MP3") {
            library.push_back(new MP3Track(t.title, t.artists, t.duration_seconds, t.bpm, t.extra_param1, t.extra_param2));
            //std::cout << "MP3Track created: " << t.extra_param1 << " kbps" << std::endl;
        }
        else {
            library.push_back(new WAVTrack(t.title, t.artists, t.duration_seconds, t.bpm, t.extra_param1, t.extra_param2));
            //std::cout << "WAVTrack created: " << t.extra_param1 << "Hz/" << t.extra_param2 << "bit" << std::endl;
        }
    }

    std::cout << "[INFO] Track library built: " << library_tracks.size() << " tracks loaded" << std::endl;
}

/**
 * @brief Display the current state of the DJ library playlist
 * 
 */
void DJLibraryService::displayLibrary() const {
    std::cout << "=== DJ Library Playlist: " 
              << playlist.get_name() << " ===" << std::endl;

    if (playlist.is_empty()) {
        std::cout << "[INFO] Playlist is empty.\n";
        return;
    }

    // Let Playlist handle printing all track info
    playlist.display();

    std::cout << "Total duration: " << playlist.get_total_duration() << " seconds" << std::endl;
}

/**
 * @brief Get a reference to the current playlist
 * 
 * @return Playlist& 
 */
Playlist& DJLibraryService::getPlaylist() {
    // Your implementation here
    return playlist;
}

/**
 * TODO: Implement findTrack method
 * 
 * HINT: Leverage Playlist's find_track method
 */
AudioTrack* DJLibraryService::findTrack(const std::string& track_title) {
    // Your implementation here
    return playlist.find_track(track_title);
}

void DJLibraryService::loadPlaylistFromIndices(const std::string& playlist_name, 
                                               const std::vector<int>& track_indices) {
    // Your implementation here

    playlist = Playlist(playlist_name);
    std::cout << "[INFO] Loading playlist: " << playlist_name << std::endl;
    for (int index : track_indices) {
        if (index < 1 || index > (int)library.size()) {
            std::cout << "[WARNING] Invalid track index: " << index << std::endl;
        }
        else{
            AudioTrack* my_track = library[index-1];
            PointerWrapper<AudioTrack> my_cloned_track = my_track->clone();
            AudioTrack* unwrapped_track = my_cloned_track.release();
            if (unwrapped_track == nullptr){
                std::cout << "[ERROR] Cloned track pointer is a nullptr " << std::endl;
            }
            else{
                unwrapped_track->load();
                unwrapped_track->analyze_beatgrid();
                playlist.add_track(unwrapped_track);
                //std::cout << "Added '" << unwrapped_track->get_title() << "' to playlist '" << playlist_name << "'" << std::endl;
            }
        }
    }
}
/**
 * TODO: Implement getTrackTitles method
 * @return Vector of track titles in the playlist
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const {
    // Your implementation here
    std::vector<std::string> titles;
    std::vector<AudioTrack*> tracks = playlist.getTracks();

    for (AudioTrack* track : tracks) {
        if (track != nullptr) {
            titles.push_back((*track).get_title());
        }
    }

    return titles;
}
