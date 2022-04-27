#pragma once

class EyeOfRhianneConfiguration;

class GamedataPicker {
  public:
    GamedataPicker(EyeOfRhianneConfiguration&);
    virtual ~GamedataPicker() = default;

    void menu();
    void start();
    bool finished() const;
  private:
    EyeOfRhianneConfiguration& _config;

    bool _finished = false;
};