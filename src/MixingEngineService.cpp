#include "MixingEngineService.h"
#include <iostream>
#include <memory>


/**
 * TODO: Implement MixingEngineService constructor
 */
MixingEngineService::MixingEngineService()
    : decks(), active_deck(1), auto_sync(false), bpm_tolerance(0)
{
    decks[0] = nullptr;
    decks[1] = nullptr;
    std::cout << "[MixingEngineService] Initialized with 2 empty decks." << std::endl;
}

/**
 * TODO: Implement MixingEngineService destructor
 */
MixingEngineService::~MixingEngineService() {
    std::cout << "[MixingEngineService] Cleaning up decks.... " << std::endl;
    delete decks[0];
    decks[0] = nullptr;
    delete decks[1];
    decks[1] = nullptr;
}


/**
 * TODO: Implement loadTrackToDeck method
 * @param track: Reference to the track to be loaded
 * @return: Index of the deck where track was loaded, or -1 on failure
 */
int MixingEngineService::loadTrackToDeck(const AudioTrack& track) {
    std::cout << "\n=== Loading Track to Deck ===" << std::endl;
    PointerWrapper<AudioTrack> cloned_track = track.clone();
    if (cloned_track.get() == nullptr) {
        std::cerr << "[ERROR] Track: \"" << track.get_title() << "\" failed to clone " << std::endl;
        return -1;
    }

    int target_deck = 1 - active_deck;
    std::cout << "[Deck Switch] Target deck: " << target_deck << std::endl;
    if (decks[target_deck] != nullptr) {
        delete decks[target_deck];
        decks[target_deck] = nullptr;
    }

    cloned_track.get()->load();
    cloned_track.get()->analyze_beatgrid();
    if (decks[active_deck] != nullptr && auto_sync && can_mix_tracks(cloned_track)) {
        sync_bpm(cloned_track);
    }

    AudioTrack* released_track = cloned_track.release();
    decks[target_deck] = released_track;
    std::cout << "[Load Complete] '" << released_track->get_title() << "' is now loaded on deck " << target_deck << std::endl;
    if (decks[active_deck] != nullptr) {
        std::cout << "[Unload] Unloading previous deck " << active_deck << " (" << decks[active_deck]->get_title() << ")" << std::endl;
        delete decks[active_deck];
        decks[active_deck] = nullptr;
    }

    active_deck = target_deck;
    std::cout << "[Active Deck] Switched to deck " << target_deck << std::endl;
    return active_deck;
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
    if (decks[active_deck] == nullptr || track.get() == nullptr) {
        return false;
    }

    int active_bpm = decks[active_deck]->get_bpm();
    int pending_bpm = track.get()->get_bpm();
    int bpm_difference = active_bpm - pending_bpm;
    if (bpm_difference <= bpm_tolerance && bpm_difference >= (-1) * bpm_tolerance) {
        return true;
    }

    return false;
}

/**
 * TODO: Implement sync_bpm method
 * @param track: Track to synchronize with active deck
 */
void MixingEngineService::sync_bpm(const PointerWrapper<AudioTrack>& track) const {
    if (decks[active_deck] != nullptr && track.get() != nullptr) {
        int active_bpm = decks[active_deck]->get_bpm();
        int original_bpm = track.get()->get_bpm();
        int average = (active_bpm + original_bpm) / 2;
        track.get()->set_bpm(average);
        std::cout << "[Sync BPM] Syncing BPM from " << active_bpm << " to " << track.get()->get_bpm() << std::endl;
    }
}
