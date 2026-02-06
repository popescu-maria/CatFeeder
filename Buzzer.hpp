class Buzzer {
  bool buzzerActive = false;
  unsigned long buzzerStart = 0;
  const unsigned long BUZZER_DURATION = 300; // ms
  const int BUZZER_FREQ = 400;

public:
  void handleBuzzer() {
    if (buzzerActive && millis() - buzzerStart >= BUZZER_DURATION) {
      noTone(BUZZER_PIN);
      buzzerActive = false;
    }
  }

  void playAlarm() {
    if (buzzerActive) return; 
    buzzerActive = true;
    buzzerStart = millis();
    tone(BUZZER_PIN, BUZZER_FREQ);
  }
};
