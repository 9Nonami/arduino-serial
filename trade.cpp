//roda a 30 ups, grandezas em milisegundos
#define LIMIT 33

unsigned long ini = 0;
unsigned long end = 0;
unsigned long dt = 0; 		//usado para dar trigger de acordo com o ups
unsigned long last_dt = 0;	//armazena o delta entre frames

//------------------------------------------------------------------------------//

#define MAIN_MENU 'm'
#define CONNECTION 'x'
#define SERVER 's'
#define CLIENT 'c'

unsigned char current_state = CONNECTION; //estado principal

//------------------------------------------------------------------------------//

#define WAIT_DATA 1
#define SEND_DATA 2
#define WAIT_CONFIRM 3
#define FINISH 4

unsigned int connection_state = 0; //subdivisao do estado principal

//------------------------------------------------------------------------------//

unsigned long acc = 0; //acumulador para verificar timeout

//------------------------------------------------------------------------------//

String device_data = "";
String client_data = "";
String server_data = "";

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

bool timeout (long _dt, int _limit) {
	acc += _dt;
	if (acc >= _limit) {
		return true;
	}
	return false;
}

void reset_acc() {
	acc = 0;
}

void update(long _dt) {
	if (current_state == MAIN_MENU) {
		char c = get_char_input();
		if (c == CONNECTION) {
			current_state = CONNECTION;
		}
	} else if (current_state == CONNECTION) {
		Serial.println("State: CONNECTION.");
		char c = get_char_input();
		if (c == SERVER) {
			current_state = SERVER;
			connection_state = WAIT_DATA;
			Serial.println("Server chosen.");
		} else if (c == CLIENT) {
			//TODO
			//current_state = CLIENT;
			//connection_state = SEND_DATA;
		}
	} else if (current_state == SERVER) {
		if (connection_state == WAIT_DATA) {
			Serial.println("Waiting data from client.");
			if (Serial.available() > 0) {
				client_data = "";
				while (Serial.available() > 0) {
					char c = Serial.read();
					client_data += c;
				}
				if (client_data[0] == 'v' && client_data[client_data.length() - 1] == 'w') {
					connection_state = SEND_DATA;
					Serial.println(client_data);
					reset_acc();
				}
			} else {
				if (timeout(_dt, 5000)) {
					Serial.println("Timeout! Returning to CONNECTION state.");
					current_state = CONNECTION;
					reset_acc();
				}
			}
		} else if (connection_state == SEND_DATA) {
			//TODO usar flush()
			Serial.println("Chegou em send.");
		}
	} else if (current_state == CLIENT) {
		//TODO
	}
}

void loop() {
	end = millis();
	last_dt = (end - ini);
	ini = end;
	dt += last_dt;
	
	if (dt >= LIMIT) {
		while (dt >= LIMIT) {
			update(last_dt);
			dt -= LIMIT;
			Serial.println("---------------------------------------------");
		}
		//draw();
	}

	//Serial.println(last_dt);
	delay(10); //diminuir se comecar a cair no fixed
}