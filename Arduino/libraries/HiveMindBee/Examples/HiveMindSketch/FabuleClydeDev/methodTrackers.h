

#include "colortypes.h"
#include "ClydeEEPROM.h"
#include "ClydeModule.h"

/**
 * Main Clyde class that provides the interface to the device.
 */
class CClyde {
 
private:
  CModulePosition m_modules[CModulePosition::NUM_MODULES];
  CAmbientLight m_ambient;
  CWhiteLight m_white;
  CClydeEEPROM m_eeprom;
  CEye m_eye;
  CAmbientCycle m_cycle;
  CMouth m_mouth;
  
public:
  CClyde();
  void begin();
  bool wasEyeCalibratedOnce() { return m_eye.onceCalibrated; }
  void updateEye();
  void updateMouth();
  void updateAmbientLight();
  void updateWhiteLight();
  void updatePersonalities();
  CModulePosition* module(uint8_t m) { return &m_modules[m]; }
  CClydeEEPROM* eeprom() { return &m_eeprom; }
  CAmbientLight* ambient() { return &m_ambient; }
  void setAmbient(const RGB &c);
  void fadeAmbient(const RGB &c, float spd);
  CWhiteLight* white() { return &m_white; }
  void setWhite(uint8_t b);
  void fadeWhite(uint8_t b, float spd);
  void switchLights();
  CAmbientCycle* cycle() { return &m_cycle; }
  void setCycle(ECycleType type, uint8_t steps, const RGB *colors, const uint8_t *intervals, ECycleLoop loop);
  void setCycle(ECycleType type, uint8_t steps, const RGB *colors, const uint16_t *intervals, ECycleLoop loop);
  void setCycle(ECycleType type, uint8_t steps, const RGB *colors, const uint32_t *intervals, ECycleLoop loop);
  void setCycleStep(uint8_t step);
  void cycleNextStep(uint32_t now);
  void speedUpCycle(uint32_t factor);
  void stopCycle();
  void blink(const RGB& rgb, uint32_t onDuration, uint32_t offDuration, uint8_t numBlinks);
  EOpCode setPlayMode(EPlayMode playmode);
  EOpCode play(uint16_t index);
  EOpCode playState();
  EOpCode setVolume(uint8_t volume);  
  EOpCode pause(void);
  EOpCode stop(void);
  
private:
  void detectPersonalities();
  void detectMouth();
  void calibrateEye(uint16_t irValue);
  bool wasEyePressed(uint16_t irValue);
  void updateAmbientLight(float *value, uint8_t target, float speed);
  void showAmbientLight();
  void showWhiteLight();
  void setCycle(ECycleType type, uint8_t steps, const RGB* colors, ECycleLoop loop);
  void updateCycle();
  void updateCycleNextStep(uint32_t now);

