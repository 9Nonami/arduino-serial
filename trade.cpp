//roda a 25 ups, grandezas em milisegundos
#define UPS 25
#define LIMIT 40 // 25 * 40 = 1000 (1s)

unsigned long ini = 0;
unsigned long end = 0;
unsigned int dt = 0;

unsigned int ups_counter = 0;

//------------------------------------------------------------------------------//

#define CONNECTION 'x'
#define SERVER 's'
#define CLIENT 'c'

unsigned char current_state = CONNECTION;

#define WAIT_DATA 1
#define SEND_DATA 2
#define WAIT_CONFIRM 3
#define FINISH 4

unsigned int connection_state = 0;

String device_data = ""; //TODO random
String client_data = "";
String server_data = ""; //TODO

//------------------------------------------------------------------------------//

void setup() {
  Serial.begin(9600);
  ini = millis();
}

char get_char_input () {
  char c = '0';
  if (Serial.available() > 0) {
    c = Serial.read();
    while (Serial.available() > 0) {
      char temp = Serial.read();
    }
  }
  return c;
}

void _update() {
  if (current_state == CONNECTION) {
    char c = get_char_input();
    if (c == SERVER) {
      current_state = SERVER;
      connection_state = WAIT_DATA;
    } else if (c == CLIENT) {
      current_state = CLIENT;
      connection_state = SEND_DATA;
    }
  } else if (current_state == SERVER) {
    if (connection_state == WAIT_DATA) {
      
      if (Serial.available() > 0) {
        client_data = "";
        while (Serial.available() > 0) {
          char c = Serial.read();
          client_data += c;
        }
        if (client_data.startsWith("v") /*&& client_data.endsWith("w")*/) {
          ups_counter = 0;
          connection_state = SEND_DATA;
        }
      } else {
        ups_counter += 1;
        if (ups_counter == (UPS*10)) {
          ups_counter = 0;
          current_state = CONNECTION;
          Serial.println("Timeout!");
        }
      }

    } else if (connection_state == SEND_DATA) {
      //TODO
      Serial.println("Chegou em send.");
    } else if (connection_state == WAIT_CONFIRM) {
      //TODO
    } else if (connection_state == FINISH) {
      //TODO
    }
  } else if (current_state == CLIENT) {
    //TODO
    Serial.println("Client chosen.");
  }
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