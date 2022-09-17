//GAME LOOP
//roda a 25 ups, grandezas em milisegundos
#define LIMIT 40 // 25 * 40 = 1000 (1s)

unsigned long ini = 0;
unsigned long end = 0;
unsigned int dt = 0;

void setup() {
  Serial.begin(9600);
  ini = millis();
}

void _update() {
  Serial.println("updating");
}

void loop() {
  end = millis();
  dt += (end - ini);
  ini = end;
  
  while (dt >= LIMIT) {
    _update();
    dt -= LIMIT;
  }
}