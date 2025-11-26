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
    for (AudioTrack* track: library) {
        delete track;
    }
}

/**
 * @brief Load a playlist from track indices referencing the library
 * @param library_tracks Vector of track info from config
 */
void DJLibraryService::buildLibrary(const std::vector<SessionConfig::TrackInfo>& library_tracks) {
    //Todo: Implement buildLibrary method
    for (SessionConfig::TrackInfo const& track: library_tracks) {
        AudioTrack* audio_track;

        if (track.type == "WAV") {
            audio_track = new WAVTrack(
                track.title, 
                track.artists, 
                track.duration_seconds, 
                track.bpm,
                track.extra_param1, 
                track.extra_param2
            );

            std::cout << "  – WAV: WAVTrack created: " << dynamic_cast<WAVTrack*>(audio_track)->get_sample_rate() << "Hz/" << dynamic_cast<WAVTrack*>(audio_track)->get_bit_depth() << "bit" << std::endl;

        } else if (track.type == "MP3") {
            audio_track = new MP3Track(
                track.title, 
                track.artists, 
                track.duration_seconds, 
                track.bpm, 
                track.extra_param1, 
                track.extra_param2
            );

            std::cout << "  – MP3: MP3Track created: " << dynamic_cast<MP3Track*>(audio_track)->get_bitrate() << " kbps" << std::endl;
        }

        library.push_back(std::move(audio_track));
    }
    
    std::cout << "[INFO] Track library built: " << library.size() << " tracks loaded" << std::endl;
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
    AudioTrack* track = playlist.find_track(track_title);
    return track;
}

void DJLibraryService::loadPlaylistFromIndices(const std::string& playlist_name, 
                                               const std::vector<int>& track_indices) {
    // Your implementation here
    std::cout << "[INFO] Loading playlist: " << playlist_name << std::endl;
    playlist = Playlist(playlist_name);
    for (int const index: track_indices) {
        if (index < 1 || static_cast<size_t>(index) > library.size()) {
            std::cout << "[WARNING] Invalid track index: " << index << std::endl; 
            return;
        }

        PointerWrapper<AudioTrack> track = library[index - 1]->clone();
        AudioTrack* track_pointer = track.release();

        if (track_pointer == nullptr) {
            std::cerr << "[ERROR] Can't create playlist from nullptr tracks" << std::endl;
            return;
        }

        track_pointer->load();
        track_pointer->analyze_beatgrid();
        playlist.add_track(track_pointer);
        std::cout << "Added " << track_pointer->get_title() << " to playlist " << playlist_name << std::endl; 
    }

    std::cout << "[INFO] Playlist loaded: " << playlist_name << " (" << playlist.get_track_count() << " tracks)" << std::endl;
}

/**
 * TODO: Implement getTrackTitles method
 * @return Vector of track titles in the playlist
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const {
    // Your implementation here
    return std::vector<std::string>(); // Placeholder
}
