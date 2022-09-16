//millis 30 ups
#define LIMIT 33

unsigned long ini = 0;
unsigned long end = 0;
unsigned long dt = 0; 		//usado para dar trigger de acordo com o ups
unsigned long last_dt = 0;	//armazena o delta entre frames

//------------------------------------------------------------------------------//

#define MAIN_MENU 'm'
#define CONNECTION 'c'

unsigned char current_state = MAIN_MENU;

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

void update(long _dt) {
	if (current_state == MAIN_MENU) {
		char c = get_char_input();
		if (c == CONNECTION) {
			current_state = CONNECTION;
		}
	} else if (current_state == CONNECTION) {
		Serial.println("Connection chosen.");
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
		}
		//draw();
	}

	//Serial.println(last_dt);
	delay(10); //diminuir se comecar a cair no fixed
}