#ifndef __TESTS_GTEST_FIXTURE_H__
#define __TESTS_GTEST_FIXTURE_H__

#include <string>

class KingDBTest {
  public:
    KingDBTest();

  protected:
    void SetUp();
    void TearDown();

    /* Common Utils */
    // @when I wait for (\d+) seconds
    void WaitForSeconds(int sec);

    /* Test for audio lib */
    // @given I load the player with (.*)
    void LoadAudioPlayer(std::string &fileroute);

    // @when I play the player
    void Play();

    // @then the player plays the file
    void PlayDone();
};

#endif // __TESTS_GTEST_FIXTURE_H__