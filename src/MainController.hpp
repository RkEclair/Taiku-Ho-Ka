#include <string>

#include "SoundManager.hpp"

struct Data;

class MainController {
private:
  std::unique_ptr<Data> data;
  SoundManager sound{};

  void checkSDLError();
  int getRandomEnemyType();

public:
  MainController();
  ~MainController();

  void init(unsigned int);
  void update();
  int run();

  // Move events
  void rotateLeft();
  void rotateRight();

  // Shoot events
  void shoot();

  // Other events
  void quit();
  void fail();
  void explosion(struct Rigidbody const &);
  void updateDisplay();
  void releasedSpace();

  Data const *getData() const { return data.get(); }
};
