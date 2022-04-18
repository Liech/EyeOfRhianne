#pragma once

class Graphic;

class AnimationControlDialog {
  public:
    AnimationControlDialog(Graphic&);

    void menu();
  private:
    Graphic& _graphic;

};