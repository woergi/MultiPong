#pragma once
#include <map>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>

enum class Music {
  GameTheme,
};

class MusicPlayer
{
public:
  MusicPlayer();
  MusicPlayer(MusicPlayer const &) = delete;

  void play(Music theme);
  void stop();
  void setPaused(bool paused);
  void setVolume(float volume);

private:
  sf::Music m_Music;
  std::map<Music, std::string> m_Filenames;
  float m_Volume;
};
