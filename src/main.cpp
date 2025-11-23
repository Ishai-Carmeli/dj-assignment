#include <iostream>
#include <memory>
#include <vector>
#include <limits>


// Include all our classes
#include "AudioTrack.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include "Playlist.h"
// Phase 4 orchestrator
#include "DJSession.h"
#include "DJLibraryService.h"
#include "DJControllerService.h"
#include "MixingEngineService.h"
#include "PointerWrapper.h"
/**
 * DJ Track Session Manager - Test Program
 * 
 * This program demonstrates the assignment and tests student implementations.
 * As students complete each phase, more functionality will work correctly.
 * 
 * ⚠️  EXPECT MEMORY LEAKS AND CRASHES INITIALLY! ⚠️
 * Students must fix the issues to make this program run cleanly.
 */

bool del = false; // hint: what is the purpose of this variable? how it changes the ownership semantics?
void test_phase_1_memory_leaks() {
    
    std::cout << "\n======== PHASE 1: MEMORY LEAK TESTING ========" << std::endl;
    std::cout << "Testing the broken Playlist class..." << std::endl;

    // Create some tracks (these will leak if AudioTrack Rule of 5 isn't implemented)
    std::cout << "Creating tracks..." << std::endl;
    AudioTrack* mp3 = new MP3Track("Sunset Lover", {"Petit Biscuit"}, 180, 120, 320);
    AudioTrack* wav = new WAVTrack("Strobe", {"Deadmau5"}, 645, 128, 44100, 16);

    // Create playlist and add tracks (this will leak nodes if destructor is broken)
    std::cout << "Creating playlist..." << std::endl;
    Playlist* my_playlist = new Playlist("Chill House Mix");

    my_playlist->add_track(mp3);
    my_playlist->add_track(wav);
    my_playlist->display();

    // Remove a track (this will leak if remove_track is broken)
    std::cout << "Removing track..." << std::endl;
    my_playlist->remove_track("Strobe");
    my_playlist->display();

    std::cout << "Cleaning up..." << std::endl;
    delete my_playlist;  // Should clean up playlist nodes
    if(del){
        delete mp3;          // Should clean up AudioTrack data
        delete wav;          // Should clean up AudioTrack data
    }

    std::cout << "Phase 1 test complete. Check for memory leaks!\n" << std::endl;
}

void test_phase_2_rule_of_5() {
    std::cout << "\n======== PHASE 2: RULE OF 5 TESTING ========" << std::endl;
    std::cout << "Testing AudioTrack copy and move operations..." << std::endl;

    try {
        // Test copy constructor
        std::cout << "Testing copy constructor..." << std::endl;
        MP3Track original("Original Track", {"Test Artist"}, 200, 128, 256);
        MP3Track copied = original;  // Should call copy constructor

        std::cout << "Original: " << original.get_title() << std::endl;
        std::cout << "Copied: " << copied.get_title() << std::endl;

        // Test copy assignment
        std::cout << "\nTesting copy assignment..." << std::endl;
        MP3Track assigned("Temporary Track", {"Temp Artist"}, 100, 120, 128);
        assigned = original;  // Should call copy assignment

        std::cout << "Assigned: " << assigned.get_title() << std::endl;

        // Test move constructor
        std::cout << "\nTesting move constructor..." << std::endl;
        MP3Track moved = std::move(original);  // Should call move constructor

        std::cout << "Moved (Constructor): " << moved.get_title() << std::endl;

        std::cout << "\nTesting move assignment operator..." << std::endl;
        MP3Track original1("New Track", {"Test Artist"}, 200, 128, 256, false);
    
        moved = std::move(original1);
        
        moved.load();
        moved.analyze_beatgrid();
        std::cout << "MP3Track SCORE: " << moved.get_quality_score() << std::endl;

        std::cout << "\nTesting: mp3 object with max integers..." << std::endl;
        
        int imax1 = std::numeric_limits<int>::max();            
        MP3Track largestMP3("Heavy mp3 track", {"Ishai"}, imax1, imax1, 320, true);
        largestMP3.load();
        largestMP3.analyze_beatgrid(); // Should print: about 76861433318333918 estimated beats, 1 precision factor

        std::cout << "MP3Track SCORE: " << largestMP3.get_quality_score() << std::endl;

        std::cout << "\nTesting: wave object with max integers..." << std::endl;

        int imax = std::numeric_limits<int>::max();            
        WAVTrack largest("Heavy track", {"Ishai"}, imax, imax, imax, imax);

        largest.load();
        largest.analyze_beatgrid(); // Should print: about 76,861,433,568,873,676 estimated beats, 6,710,886.3 precision factor

        std::cout << "WAVTrack SCORE: " << largest.get_quality_score() << std::endl;

        //AudioTrack* tamir = new WAVTrack("Tamir WAV Track", {"abc"}, 645, 128, 44100, 16);
        //(*tamir).load();

        WAVTrack dalva("Dalva WAV Track", {"abc"}, 645, 128, 44100, 16);
        dalva.load();
        dalva.analyze_beatgrid();
        std::cout << "WAVTrack SCORE: " << dalva.get_quality_score() << std::endl;


        std::cout << "Moved (Operator): " << moved.get_title() << std::endl;

        std::cout << "\nTesting self move assignment operator..." << std::endl;
        moved = std::move(moved);
        
        std::cout << "Moved Self (Operator): " << moved.get_title() << std::endl;

        std::cout << "Phase 2 test complete!\n" << std::endl;

    } catch (const std::exception& e) {
        std::cout << "❌ Exception caught: " << e.what() << std::endl;
        std::cout << "This indicates Rule of 5 implementation issues!\n" << std::endl;
    }
}

void test_phase_3() {
    std::cout << "\n======== PHASE 3: SMART POINTER TESTING ========" << std::endl;
    std::cout << "Testing DJUniquePtr implementation..." << std::endl;

    try {
        // Test basic construction
        std::cout << "Testing DJUniquePtr construction..." << std::endl;
        PointerWrapper<MP3Track> smart_track(new MP3Track("Smart Track", {"AI Artist"}, 240, 130, 320));

        // Test access operators (will be TODO messages initially)
        std::cout << "\nTesting access operators..." << std::endl;
        if (smart_track) {
            std::cout << "Smart pointer contains: " << smart_track->get_title() << std::endl;
            std::cout << "BPM: " << (*smart_track).get_bpm() << std::endl;
        }

        // Test move operations
        std::cout << "\nTesting move operations..." << std::endl;
        smart_track = std::move(smart_track);
        PointerWrapper<MP3Track> moved_track = std::move(smart_track);
        std::cout << "Original pointer after move: " << (smart_track ? "still valid" : "null") << std::endl;
        std::cout << "Moved pointer: " << (moved_track ? "valid" : "null") << std::endl;
        
        std::cout << "moved_track pointer before reset: " << (moved_track ? (*moved_track).get_title() : "null") << std::endl;

        // Test reset
        std::cout << "\nTesting reset..." << std::endl;
        moved_track.reset(new MP3Track("Reset Track", {"Reset Artist"}, 180, 125, 256));

        std::cout << "moved_track pointer after reset: " << (moved_track ? (*moved_track).get_title() : "null") << std::endl;

        // Test release
        std::cout << "\nTesting release..." << std::endl;
        MP3Track* raw_ptr = moved_track.release();
        std::cout << "Released pointer: " << (raw_ptr ? raw_ptr->get_title() : "null") << std::endl;

        // Manual cleanup since we released
        delete raw_ptr;
        PointerWrapper<MP3Track> smart_track_2(new MP3Track("Smart Track 2", {"AI Artist"}, 240, 130, 320));
        
        std::cout << "Phase 3 test complete!\n" << std::endl;

    } catch (const std::exception& e) {
        std::cout << "❌ Exception caught: " << e.what() << std::endl;
        std::cout << "This indicates DJUniquePtr implementation issues!\n" << std::endl;
    }
}

void demonstrate_polymorphism() {
    std::cout << "\n======== POLYMORPHISM DEMONSTRATION ========" << std::endl;
    std::cout << "Showing virtual function calls with different track types...\n" << std::endl;

    // Create different track types
    std::vector<std::unique_ptr<AudioTrack>> tracks;
    tracks.push_back(std::unique_ptr<AudioTrack>(new MP3Track("MP3 Example", {"Artist 1"}, 200, 128, 320)));
    tracks.push_back(std::unique_ptr<AudioTrack>(new WAVTrack("WAV Example", {"Artist 2"}, 180, 130, 44100, 16)));

    // Call virtual functions polymorphically
    for (auto& track : tracks) {
        std::cout << "Track: " << track->get_title() << std::endl;
        track->load();                    // Virtual function call
        track->analyze_beatgrid();        // Virtual function call
        std::cout << "Quality: " << track->get_quality_score() << std::endl;  // Virtual function call

        // Test cloning
        auto cloned = track->clone();
        if (cloned) {
            std::cout << "Cloned: " << cloned->get_title() << std::endl;
        }
        std::cout << std::endl;
    }
}

void test_library() {
    SessionFileParser parser = SessionFileParser();
    SessionConfig config = SessionConfig();
    DJLibraryService library = DJLibraryService();
    bool parsed = parser.parse_config_file("./input_2/dj_config.txt", config);
    if (parsed) {
        library.buildLibrary(config.library_tracks);
        library.loadPlaylistFromIndices("armin_van_buuren_ASOT_1250", config.playlists["armin_van_buuren_ASOT_1250"]);
    //    std::cout << &config.library_tracks << std::endl;
    }
}

int main(int argc, char* argv[]) {    
    /**
     * Command-line argument parsing
     * - If "-I" is provided as the first argument, run interactive DJ software
     * - If "-A" is provided as the second argument, enable play_all mode
     */
    bool run_software = false;
    bool play_all = false;
    if (argc > 1 && std::string(argv[1]) == "-I") {
        run_software = true;
    }

    if (argc > 2 && std::string(argv[2]) == "-A") {
        play_all = true;
    }

    if (run_software) {
        std::cout << "\n============= RUNNING INTERACTIVE SOFTWARE =============" << std::endl;
        DJSession live_session("Interactive Session", play_all);
        live_session.simulate_dj_performance();
        std::cout << "============= INTERACTIVE SESSION ENDED =============\n" << std::endl;
    } else {
        std::cout << "==================================================" << std::endl;
        std::cout << "    DJ TRACK SESSION MANAGER - TEST PROGRAM" << std::endl;
        std::cout << "==================================================" << std::endl;
        std::cout << "This program tests all phases of the assignment." << std::endl;
        std::cout << "Initially, many features will be incomplete!" << std::endl;
        std::cout << "==================================================" << std::endl;
        
        // Test each phase individually
        //test_phase_1_memory_leaks();
        // test_phase_2_rule_of_5();
        //test_phase_3();
        //demonstrate_polymorphism();
        test_library();
        std::cout << "\n(Set 'run_software' to true in main.cpp to run the full interactive session.)\n" << std::endl;
    }
    return 0;
}