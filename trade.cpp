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
#define SEND_CONFIRM 4
#define FINISH 5

unsigned int connection_state = 0;

String device_data = "";
String received_data = "";

#define OK 'k'

//------------------------------------------------------------------------------//

#define IR 1
#define ER 152

//------------------------------------------------------------------------------//

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
  device_data = String(random(IR, ER));
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

bool is_valid(String rcvd, int irange, int erange) {
  if (rcvd.charAt(0) == 'v' && rcvd.charAt(rcvd.length()-1) == 'w') {
    String temp = rcvd.substring(1, rcvd.length()-1);
    unsigned int valid = temp.toInt();
    if (valid != 0 && valid >= irange && valid < erange) {
      return true;
    }
  }
  return false;
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
        received_data = "";
        while (Serial.available() > 0) {
          char c = Serial.read();
          received_data += c;
        }
        if (is_valid(received_data, IR, ER)) {
          ups_counter = 0;
          received_data = received_data.substring(1, received_data.length()-1);
          connection_state = SEND_DATA;
        } else {
          ups_counter += 1;
          if (ups_counter == (UPS*10)) {
            ups_counter = 0;
            current_state = CONNECTION;
          }
        }
      } else {
        ups_counter += 1;
        if (ups_counter == (UPS*10)) {
          ups_counter = 0;
          current_state = CONNECTION;
        }
      }
    } else if (connection_state == SEND_DATA) {
      String _data = "v";
      _data.concat(device_data);
      _data.concat("w");
      Serial.print(_data);
      Serial.flush();
      connection_state = WAIT_CONFIRM;
    } else if (connection_state == WAIT_CONFIRM) {
      char c = get_char_input();
      if (c == OK) {
        connection_state = FINISH;
        ups_counter = 0;
      } else {
        ups_counter += 1;
        if (ups_counter == (UPS*3)) {
          ups_counter = 0;
          current_state = CONNECTION;
        }
      }
    } else if (connection_state == FINISH) {
      device_data = received_data;
      current_state = CONNECTION;
    }
  } else if (current_state == CLIENT) {
    if (connection_state == SEND_DATA) {
      String _data = "v";
      _data.concat(device_data);
      _data.concat("w");
      Serial.print(_data);
      Serial.flush();
      connection_state = WAIT_DATA;
    } else if (connection_state == WAIT_DATA) {
      if (Serial.available() > 0) {
        received_data = "";
        while (Serial.available() > 0) {
          char c = Serial.read();
          received_data += c;
        }
        if (is_valid(received_data, IR, ER)) {
          ups_counter = 0;
          received_data = received_data.substring(1, received_data.length()-1);
          connection_state = SEND_CONFIRM;
        } else {
          ups_counter += 1;
          if (ups_counter == (UPS*10)) {
            ups_counter = 0;
            current_state = CONNECTION;
          }
        }
      } else {
        ups_counter += 1;
        if (ups_counter == (UPS*10)) {
          ups_counter = 0;
          current_state = CONNECTION;
        }
      }
    } else if (connection_state == SEND_CONFIRM) {
      Serial.write(OK);
      Serial.flush();
      connection_state = FINISH;
    } else if (connection_state == FINISH) {
      device_data = received_data;
      current_state = CONNECTION;
    }
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

//v e w >> define
//refac. func.