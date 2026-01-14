#include <Ultrasonic.h> // Librairie pour le capteur ultrason Grove
#include <rgb_lcd.h>    // Librairie pour l'écran LCD Grove RGB

// --- Configuration des broches ---
#define BUZZER_PIN 3
#define ECRAN_PIN 2
#define ULTRASONIC_PIN 4 
#define DISTANCE_DETECTION_CM 50 // Distance en cm pour déclencher la détection


Ultrasonic ultrasonic(ULTRASONIC_PIN); // <-- C'EST CETTE LIGNE QUI MANQUAIT

// --- Configuration de l'écran LCD Grove RGB ---
// L'objet lcd est créé avec la librairie rgb_lcd.h
rgb_lcd lcd;

// --- Variables d'état ---
bool musiqueEnCours = false;
unsigned long tempsPrecedentDetection = 0;
const unsigned long delaiAntiRebond = 5000; // Délai de 5 secondes

// --- Mélodie de Noël (Jingle Bells) ---
// Les notes sont définies par leur fréquence en Hz
#define NOTE_E6 1319
#define NOTE_DS6 1245
#define NOTE_D6 1175
#define NOTE_C6 1047
#define NOTE_B5 988
#define NOTE_A5 880
#define NOTE_G5 784
#define NOTE_REST 0

int melody[] = {
  NOTE_E6, NOTE_E6, NOTE_E6, NOTE_E6, NOTE_E6, NOTE_E6, NOTE_E6, NOTE_E6, NOTE_C6, NOTE_D6,
  NOTE_E6, NOTE_D6, NOTE_D6, NOTE_D6, NOTE_D6,
  NOTE_D6, NOTE_E6, NOTE_E6, NOTE_E6, NOTE_E6, NOTE_D6, NOTE_D6, NOTE_E6, NOTE_D6, NOTE_G5
};

int noteDurations[] = {
  8, 8, 4, 8, 8, 4, 8, 8, 8, 8, 2, 8, 8, 8, 8, 8, 8, 8, 16, 16, 8, 8, 8, 8, 4, 4
};

void setup() {
  // Initialisation du buzzer
  pinMode(BUZZER_PIN, OUTPUT);
  noTone(BUZZER_PIN);

  // Initialisation de l'écran LCD Grove RGB
  lcd.begin(16, 2); // Initialise l'écran avec 16 colonnes et 2 lignes
  // On définit une couleur de fond (Rouge, Vert, Bleu) pour Noël
  lcd.setRGB(255, 0, 100); // Un joli rouge/rose

  // Message de bienvenue
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Systeme Pret");
  lcd.setCursor(0, 1);
  lcd.print("En attente...");

  Serial.begin(9600); // Pour le débogage
}

void loop() {
  unsigned long tempsActuel = millis();

  if (!musiqueEnCours && (tempsActuel - tempsPrecedentDetection > delaiAntiRebond)) {

    long distance = ultrasonic.MeasureInCentimeters();

    Serial.print("Distance mesuree (cm): ");
    Serial.println(distance);

    if (distance > 0 && distance < DISTANCE_DETECTION_CM) {
      Serial.print("Presence detectee a : ");
      Serial.print(distance);
      Serial.println(" cm -> DECLANCHEMENT !");

      declencherSequenceNoel();
      tempsPrecedentDetection = tempsActuel;
    }
  }
}

void declencherSequenceNoel() {
  musiqueEnCours = true;

  // 1. Afficher le message sur l'écran LCD GROVE RGB avec une couleur festive
  lcd.clear();
  lcd.setRGB(0, 255, 0); // Change la couleur en vert pour le message !
  lcd.setCursor(0, 0);
  lcd.print("Joyeux Noel !");
  lcd.setCursor(0, 1);
  lcd.print(" oh oh oh ");

  // 2. Jouer la mélodie
  jouerMusiqueNoel();

  // 3. Effacer l'écran et remettre la couleur d'origine
  lcd.clear();
  lcd.setRGB(255, 0, 100); // Retour au rouge/rose initial
  lcd.setCursor(0, 0);
  lcd.print("En attente...");

  musiqueEnCours = false;
}

void jouerMusiqueNoel() {
  for (int thisNote = 0; thisNote < sizeof(melody) / sizeof(int); thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(BUZZER_PIN, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(BUZZER_PIN);
  }
}