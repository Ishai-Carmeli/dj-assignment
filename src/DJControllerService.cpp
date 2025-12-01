#include "DJControllerService.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>

DJControllerService::DJControllerService(size_t cache_size)
    : cache(cache_size) {}
/**
 * TODO: Implement loadTrackToCache method
 */
int DJControllerService::loadTrackToCache(AudioTrack& track) {
    if (cache.contains(track.get_title())) {
        cache.get(track.get_title());
        return 1;
    }

    PointerWrapper<AudioTrack> cloned_track = track.clone();
    AudioTrack* raw_ptr = cloned_track.release();
    if (raw_ptr == nullptr) {
        std::cerr << "[ERROR] Can't load a null track" << std::endl;
        return 0;
    }

    raw_ptr->load();
    raw_ptr->analyze_beatgrid();
    bool put_result = cache.put(PointerWrapper<AudioTrack>(raw_ptr));
    if (put_result) {
        return -1;
    }

    return 0; 
}

void DJControllerService::set_cache_size(size_t new_size) {
    cache.set_capacity(new_size);
}
//implemented
void DJControllerService::displayCacheStatus() const {
    std::cout << "\n=== Cache Status ===\n";
    cache.displayStatus();
    std::cout << "====================\n";
}

/**
 * TODO: Implement getTrackFromCache method
 */
AudioTrack* DJControllerService::getTrackFromCache(const std::string& track_title) {
    // Your implementation here
    return nullptr; // Placeholder
}
