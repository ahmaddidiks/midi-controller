
// padNote = mapping aplikasi studio one
// cutOff  = nilai minimum (paling lirih yang bisa diterima)
// Sensitivity = sensitifitas channel sensor

int kickPadNote = 36;
int kickCutOff = 250;
int kickSensitivity = 50;

int snarePadNote = 39;
int snareCutOff = 250;
int snareSensitivity = 50;

int tom1PadNote = 72;
int tom1CutOff = 250;
int tom1Sensitivity = 50;

int tom2PadNote = 70;
int tom2CutOff = 250;
int tom2Sensitivity = 50;

int floorPadNote = 66;
int floorCutOff = 250;
int floorSensitivity = 50;

int hihetPadNote = 56;
int hihetCutOff = 250;
int hihetSensitivity = 50;

int crashPadNote = 77;
int crashCutOff = 250;
int crashSensitivity = 50;

int ridePadNote = 61;
int rideCutOff = 250;
int rideSensitivity = 50;

byte PadNote[8]  = { kickPadNote, snarePadNote, tom1PadNote, tom2PadNote, floorPadNote, hihetPadNote, crashPadNote, ridePadNote };
int Sensitivity[8]        = { kickSensitivity, snareSensitivity, tom1Sensitivity, tom2Sensitivity, floorSensitivity, hihetSensitivity, crashSensitivity, rideSensitivity };
int PadCutOff[8]          = { kickCutOff, snareCutOff, tom1CutOff, tom2CutOff, floorCutOff, hihetCutOff, crashCutOff, rideCutOff };

byte status;
int MaxPlayTime[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
#define midichanne 0;
boolean activePad[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
int PinPlayTime[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
boolean VelocityFlag = true;
int analogPin = A0;
int pin = 0;
int hitavg = 0;
int pad = 0;
int r0 = 0;
int r1 = 0;
int r2 = 0;
int count = 0;
int multiplex[8];

// void setup adalh fungsi yang diesekusi paling awal.
void setup() {
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  Serial.begin(115200);
}

//Pada fungsi Loop digunakan untuk mengulang Fungsi void readSensors dan void checkSensors..
void loop() {
  if (Serial.available())
  {
    char temp = Serial.read();
  }
  Serial.flush();
  readSensors();
  checkSensors();
  delay(2);
}

//Fungsi untuk membaca ada sensor/getaran dari piezo...
void readSensors() {
  for (count = 0; count <= 7; count++) {
    r2 = bitRead(count, 0);
    r1 = bitRead(count, 1);
    r0 = bitRead(count, 2);
    digitalWrite(2, r0);
    digitalWrite(3, r1);
    digitalWrite(4, r2);
    multiplex[count] = analogRead(analogPin) * Sensitivity[count];
  }
}
//Fungsi untuk mengecek sensor/piezoelectrik......
void checkSensors() {
  for (int pin = 0; pin <= 7; pin++) {
    hitavg = multiplex[pin];
    pad = pin;
    if ((hitavg > PadCutOff[pad])) {
      if ((activePad[pad] == false)) {
        if (VelocityFlag == true) {
          hitavg = (hitavg / 8) - 1;
        } else {
          hitavg = 127;
        }

        if (hitavg >= 250) hitavg = 250;

        MIDI_TX(144, PadNote[pad], hitavg);
        PinPlayTime[pad] = 0;
        activePad[pad] = true;
      } else {
        PinPlayTime[pad] = PinPlayTime[pad] + 1;
      }
    } else if ((activePad[pad] == true)) {
      PinPlayTime[pad] = PinPlayTime[pad] + 1;
      if (PinPlayTime[pad] > MaxPlayTime[pad]) {
        activePad[pad] = false;
        MIDI_TX(128, PadNote[pad], 0);
      }
    }
  }
}

void MIDI_TX(byte MESSAGE, byte PITCH, byte VELOCITY) {
  MESSAGE |=  byte(0);
  Serial.write(MESSAGE);
  Serial.write(PITCH);
  Serial.write(VELOCITY);
}
