Feature: Playing audio by using the engine

  Scenario: Playing MP3 file for a while
    Given I load the player with <file>
    When I play the player
    Then I wait for <time> seconds
    Then the player plays the file

      | file                          | time |
      | resource/kingdb1_resource.mp3 | 3    |