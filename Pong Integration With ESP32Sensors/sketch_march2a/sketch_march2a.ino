int flag = 0;
int flag1 = 0;
int flag2 = 0;

int P1Cima = 18;
int P1Baixo = 19;

int valorPOT;
int porcent;

void setup() {
// put your setup code here, to run once:
    Serial.begin(9600);

    pinMode(P1Cima, INPUT_PULLUP);
    pinMode(P1Baixo, INPUT_PULLUP);
}

void loop() {
// put your main code here, to run repeatedly:
    String str = "";

    valorPOT = analogRead(34);
    porcent = map(valorPOT, 0, 4095, 0, 100);

    if(porcent != flag2){
        if(porcent < flag2 - 5){
            str += "P2/01";
        }

        else if(porcent > flag2 + 5){
            str += "P2/-1";
        }
    }

    flag2 = porcent;

    if(digitalRead(P1Cima) == LOW && flag == 0){
        str += "P1/01";
        flag = 1;
    }else if(digitalRead(P1Cima) == HIGH){
        flag = 0;
    }
    
    if(digitalRead(P1Baixo) == LOW && flag1 == 0){
        str += "P1/-1";
        flag1 = 1;
    }else if(digitalRead(P1Baixo) == HIGH){
        flag1 = 0;
    }

    if (str != "") {
        Serial.println(str);
    }

    delay(100);
}