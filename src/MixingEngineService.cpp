#include "MixingEngineService.h"
#include <iostream>
#include <memory>


/**
 * TODO: Implement MixingEngineService constructor
 */
MixingEngineService::MixingEngineService()
    : active_deck(0)
{
    // Your implementation here
    decks[0] = nullptr;
    decks[1] = nullptr;
    auto_sync = false;
    bpm_tolerance = 0;
    std::cout << "[MixingEngineService] Initialized with 2 empty decks.\n";
}

/**
 * TODO: Implement MixingEngineService destructor
 */
MixingEngineService::~MixingEngineService() {
    // Your implementation here
    std::cout << "[MixingEngineService] Cleaning up decks....\n";
    for (int i = 0; i < 2; i++){
        if (decks[i] != nullptr){
            delete decks[i];
            decks[i] = nullptr;
        }
    }
}


/**
 * TODO: Implement loadTrackToDeck method
 * @param track: Reference to the track to be loaded
 * @return: Index of the deck where track was loaded, or -1 on failure
 */
int MixingEngineService::loadTrackToDeck(const AudioTrack& track) {
    // Your implementation here

    std::cout << "\n=== Loading Track to Deck ===\n";

    bool first_load = decks[0] == nullptr && decks[1] == nullptr;
    
    PointerWrapper<AudioTrack> my_cloned_track = track.clone();
    if (my_cloned_track.get() == nullptr){
        std::cout << "[ERROR] Track: \"" << track.get_title() << "\" failed to clone" << std::endl;
        return -1;
    }

    int target_deck;
    if (first_load){
        target_deck = 0;
    }
    else {
        target_deck = 1 - active_deck;
        std::cout << "[Deck Switch] Target deck: " << target_deck << std::endl;
        if (decks[target_deck] != nullptr){
            delete decks[target_deck];
            decks[target_deck] = nullptr;
        }
    }

    my_cloned_track.get()->load();
    my_cloned_track.get()->analyze_beatgrid();

    if (!first_load && auto_sync){
        if (!can_mix_tracks){
            sync_bpm(my_cloned_track);    
        }
    }

    AudioTrack* unwrapped_track = my_cloned_track.release();
    decks[target_deck] = unwrapped_track;
    std::cout << "[Load Complete] '" << unwrapped_track->get_title() << "' is now loaded on deck " << target_deck << std::endl;

    if (!first_load){
        std::cout << "[Unload] Unloading previous deck " << active_deck << " (" <<  decks[active_deck]->get_title() << ")" << std::endl;
        delete decks[active_deck];
        decks[active_deck] = nullptr;
    }
    active_deck = target_deck;
    std::cout << "[Active Deck] Switched to deck " << target_deck << std::endl;
    return target_deck; // Placeholder
}

/**
 * @brief Display current deck status
 */
void MixingEngineService::displayDeckStatus() const {
    std::cout << "\n=== Deck Status ===\n";
    for (size_t i = 0; i < 2; ++i) {
        if (decks[i])
            std::cout << "Deck " << i << ": " << decks[i]->get_title() << "\n";
        else
            std::cout << "Deck " << i << ": [EMPTY]\n";
    }
    std::cout << "Active Deck: " << active_deck << "\n";
    std::cout << "===================\n";
}

/**
 * TODO: Implement can_mix_tracks method
 * 
 * Check if two tracks can be mixed based on BPM difference.
 * 
 * @param track: Track to check for mixing compatibility
 * @return: true if BPM difference <= tolerance, false otherwise
 */
bool MixingEngineService::can_mix_tracks(const PointerWrapper<AudioTrack>& track) const {
    // Your implementation here

    if (decks[active_deck] == nullptr || track.get() == nullptr){
        return false;
    }
    int track_bpm = track.get()->get_bpm();
    int active_bpm = decks[active_deck]->get_bpm();
    if (std::abs(track_bpm - active_bpm) <= bpm_tolerance){
        return true;
    }
    return false; // Placeholder
}

/**
 * TODO: Implement sync_bpm method
 * @param track: Track to synchronize with active deck
 */
void MixingEngineService::sync_bpm(const PointerWrapper<AudioTrack>& track) const {
    // Your implementation here

    if (decks[active_deck] != nullptr || track.get() != nullptr){
        int original_bpm = track.get()->get_bpm();
        int active_bpm = decks[active_deck]->get_bpm();
        int new_bpm = (original_bpm + active_bpm) / 2;
        track.get()->set_bpm(new_bpm);
        std::cout << "[Sync BPM] Syncing BPM from " << original_bpm << " to " << new_bpm << "\n";
    }
}
