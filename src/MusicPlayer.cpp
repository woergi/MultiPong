#include "MusicPlayer.h"

MusicPlayer::MusicPlayer() : m_Music() , m_Filenames() , m_Volume(100.f) {
  //http://ericskiff.com/music/
  m_Filenames[Music::GameTheme] = "resources/GameMusic01.ogg";
}

void MusicPlayer::play(Music theme) {
  std::string filename = m_Filenames[theme];

  if (!m_Music.openFromFile(filename))
    throw std::runtime_error("Music " + filename + " could not be loaded.");

  m_Music.setVolume(m_Volume);
  m_Music.setLoop(true);
  m_Music.play();
}

void MusicPlayer::stop() {
  m_Music.stop();
}

void MusicPlayer::setPaused(bool paused) {
  if (paused)
    m_Music.pause();
  else
    m_Music.play();
}
