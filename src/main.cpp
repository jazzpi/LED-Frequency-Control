#include <Arduino.h>

constexpr int LED_PIN = 9;

constexpr int8_t TIM1TOGGLE_A_ON_COMPARE =
    (0b01 << 6);                         // Toggle OC1A on Compare Match
constexpr int8_t TIM1WGM_CTC = (0b0100); // Clear Timer on Compare Match
constexpr int8_t TIM1CCRA_CTC =
    (TIM1WGM_CTC & 0b11); // WGM11 and WGM10 are set in TCCR1A
constexpr int8_t TIM1CCRB_CTC =
    ((TIM1WGM_CTC >> 2) << 3); // WGM13 and WGM12 are set in TCCR1B
constexpr int8_t TIM1CS_PRESCALE_1024 = (0b101) << 0; // 1024 prescale

void setFrequency(uint16_t frequency) {
  noInterrupts();
  if (frequency == 0) {
    // Disable PWM
    TCCR1A = TIM1CCRA_CTC;
    digitalWrite(LED_PIN, LOW);
  } else {
    // Enable PWM
    TCCR1A = TIM1TOGGLE_A_ON_COMPARE | TIM1CCRA_CTC;
    OCR1A = F_CPU / 1024 / frequency / 2;
    TCNT1 = 0;
  }
  interrupts();
}

void setup() {
  Serial.begin(115200);

  while (!Serial)
    ;

  pinMode(LED_PIN, OUTPUT);
  TCCR1A = TIM1TOGGLE_A_ON_COMPARE | TIM1CCRA_CTC;
  TCCR1B = TIM1CCRB_CTC | TIM1CS_PRESCALE_1024;

  setFrequency(1);
}

void loop() {
  constexpr int DATA_SIZE = 6; // 5 digits + CR (LF is not included in return)
  char data[DATA_SIZE];
  memset(data, 0, DATA_SIZE);
  size_t bytes_read = Serial.readBytesUntil('\n', data, DATA_SIZE);
  if (data[bytes_read - 1] != '\r') {
    return;
  }
  data[bytes_read - 1] = '\0';

  int frequency = atoi(data);
  setFrequency(frequency);
  Serial.println(frequency);
}
