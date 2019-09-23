#include <Servo.h>
#include <SoftwareSerial.h>                                   // Библиотека програмной реализации обмена по UART-протоколу
SoftwareSerial SIM800(2, 3);                                  // RX, TX
#include <Keypad.h> // Подключаем библиотеки 
#include <Password.h> 
Servo servo1;
Servo servo2;


Password password1 = Password( "9" ); 
Password password2 = Password( "998800" ); 
Password password3 = Password( "*" ); 
const byte ROWS = 4; // 4 строки 
const byte COLS = 4; // 4 столбца 
char keys[ROWS][COLS] = { 
{'1','2','3','A'}, 
{'4','5','6','B'}, 
{'7','8','9','C'}, 
{'*','0','#','D'} 
}; 
//устанавливаем пины столбцов и рядов 
byte rowPins[ROWS] = {12,11, 10, 9}; 
byte colPins[COLS] = {8, 7, 6, 5}; 
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS ); 

String _response    = "";                                     // Переменная для хранения ответа модуля
long lastUpdate = millis();                                   // Время последнего обновления
long updatePeriod   = 60000;                                  // Проверять каждую минуту

String phones = "+79222270648";   // Белый список телефонов
const int redLED = A2;              // Status LED
const int greenLED = A3;            // Status LED

void setup() {
servo1.attach(A4);
servo1.write(5); //Поворачиваем сервопривод в положение 0
servo2.attach(A5);
servo2.write(5);
sendSMS(String(phones),String("Good mondey"));

pinMode(redLED, OUTPUT);
pinMode(greenLED, OUTPUT);
digitalWrite(greenLED, HIGH);  
keypad.addEventListener(keypadEvent); //добавляем событие 
keypad.setDebounceTime(250);  
  
  Serial.begin(9600);                                         // Скорость обмена данными с компьютером
  SIM800.begin(9600);                                         // Скорость обмена данными с модемом
  Serial.println(F("Start!"));

  sendATCommand("AT", true);                                  // Отправили AT для настройки скорости обмена данными
  sendATCommand("AT+CMGF=1;&W", true);                        // Включаем текстовый режима SMS (Text mode) и сразу сохраняем значение (AT&W)!
  lastUpdate = millis();                                      // Обнуляем таймер
}

String sendATCommand(String cmd, bool waiting) {
  String _resp = "";                                              // Переменная для хранения результата
  Serial.println(cmd);                                            // Дублируем команду в монитор порта
  SIM800.println(cmd);                                            // Отправляем команду модулю
  if (waiting) {                                                  // Если необходимо дождаться ответа...
    _resp = waitResponse();                                       // ... ждем, когда будет передан ответ
    // Если Echo Mode выключен (ATE0), то эти 3 строки можно закомментировать
    if (_resp.startsWith(cmd)) {                                  // Убираем из ответа дублирующуюся команду
      _resp = _resp.substring(_resp.indexOf("\r", cmd.length()) + 2);
    }
    Serial.println(_resp);                                        // Дублируем ответ в монитор порта
  }
  return _resp;                                                   // Возвращаем результат. Пусто, если проблема
}

String waitResponse() {                                           // Функция ожидания ответа и возврата полученного результата
  String _resp = "";                                              // Переменная для хранения результата
  long _timeout = millis() + 10000;                               // Переменная для отслеживания таймаута (10 секунд)
  while (!SIM800.available() && millis() < _timeout)  {};         // Ждем ответа 10 секунд, если пришел ответ или наступил таймаут, то...
  if (SIM800.available()) {                                       // Если есть, что считывать...
    _resp = SIM800.readString();                                  // ... считываем и запоминаем
  }
  else {                                                          // Если пришел таймаут, то...
    Serial.println("Timeout...");                                 // ... оповещаем об этом и...
  }
  return _resp;                                                   // ... возвращаем результат. Пусто, если проблема
}

bool hasmsg = false;                                              // Флаг наличия сообщений к удалению
void loop() {
  keypad.getKey(); 

String Zagadka = "1 zagadka - Novogodniy yest' odin eto vkusniy...!";
String Mandarin = "2 zagadka - Iz mody ne vyhodyat Ded Moroz vsegda v nih hodit?";
String Valenki = "3 zagadka - Na golove ego vederko, A nos - s'yedobnaya morkovka. Kto on?";
String Error = "Nepravilno, podumai eshe.";
String Snegovik = "Vot eto da - odoleli vi mena. Otkrivau.";
String Privet = "Privet I SMSBOX! Otgadayte 3 zagadki - poluchayte vse podarki.";
  
if (SIM800.available())   {                           // Если модем, что-то отправил...
    _response = waitResponse();                       // Получаем ответ от модема для анализа
    _response.trim();                                 // Убираем лишние пробелы в начале и конце
    Serial.println(_response);                        // Если нужно выводим в монитор порта
    if (_response.indexOf("+CMTI:")>-1) {             // Пришло сообщение об отправке SMS
      lastUpdate = millis() -  updatePeriod;          // Теперь нет необходимости обрабатываеть SMS здесь, достаточно просто
                                                      // сбросить счетчик автопроверки и в следующем цикле все будет обработано
    }
else if (_response.indexOf("A") > -1) {
servo1.attach(A4);
servo1.write(10); //Поворачиваем сервопривод в положение 0
servo2.attach(A5);
servo2.write(-170);
}
else if (_response.indexOf("Q") > -1) {
servo1.attach(A4);
servo1.write(170); //Поворачиваем сервопривод в положение 0
servo2.attach(A5);
servo2.write(-10);
}
else if (_response.indexOf("Snegovik") > -1) { //если увидели команду отправки СМС 
sendSMS(String(phones),String(Snegovik)); //отправляем СМС на номер +71234567890
servo1.attach(A4);
servo1.write(150); //Поворачиваем сервопривод в положение 0
}    
else if (_response.indexOf("Privet") > -1) { //если увидели команду отправки СМС 
sendSMS(String(phones),  String(Privet)); //отправляем СМС на номер +71234567890
delay(10000);
sendSMS(String(phones),  String(Zagadka)); //отправляем СМС на номер +71234567890
}
else if (_response.indexOf("Mandarin") > -1) { //если увидели команду отправки СМС 
sendSMS(String(phones),  String(Mandarin)); //отправляем СМС на номер +71234567890
}
else if (_response.indexOf("Valenki") > -1) { //если увидели команду отправки СМС 
sendSMS(String(phones),  String(Valenki)); //отправляем СМС на номер +71234567890
}
else if (_response.indexOf("+CMT") > -1) { //если увидели команду отправки СМС 
sendSMS(String(phones), String(Error)); //отправляем СМС на номер +71234567890 
}
  }
  if (Serial.available())  {                          // Ожидаем команды по Serial...
    SIM800.write(Serial.read());                      // ...и отправляем полученную команду модему
  };
}

void keypadEvent(KeypadEvent eKey){ 
switch (keypad.getState()){ 
case PRESSED: 
Serial.print(F("Pressed: ")); 
Serial.println(eKey); 
digitalWrite(greenLED,LOW); //включение светодиода для поддтверждения ввода с клавиатуры 
delay(300); 
digitalWrite(greenLED,HIGH); //отключение светодиода 
switch (eKey){ 
case '#': guessPassword(); break; 
default: 
password1.append(eKey); 
password2.append(eKey); 
password3.append(eKey); 
} 
}} 
//sendSMS(String(phones),String("Neznaes chto dalshe? <Pin>")); //отправляем СМС на номер +71234567890 
void guessPassword(){ 
Serial.print(F("Guessing password... ")); 
if (password1.evaluate()){ 
digitalWrite(greenLED,HIGH);
digitalWrite(redLED,HIGH); //активация светодиода красного при неверном коде 
delay(500);
digitalWrite(greenLED,LOW);
digitalWrite(redLED,LOW);
delay(500);
digitalWrite(greenLED,HIGH);
digitalWrite(redLED,HIGH);
delay(500);
digitalWrite(greenLED,LOW);
digitalWrite(redLED,LOW);
delay(500);
digitalWrite(greenLED,HIGH);
digitalWrite(redLED,LOW);
sendSMS(String(phones), String(F("Igraem dalshe. Vipal snegnaydi, naydi snezhinki.")));
//Serial.println("P Ok "); 
password1.reset(); //сброс переменной пароля после корректного ввода 
}
else if (password2.evaluate()){ 
digitalWrite(redLED,HIGH); //активация светодиода красного при неверном коде 
servo2.attach(A5);
servo2.write(150);
//sendSMS(String(phones), String(F("yyyy")));
//Serial.println("P Ok "); // 
password2.reset(); //сброс переменной пароля после корректного ввода 
}
else if (password3.evaluate()){ 
delay(100);
sendSMS(String(phones), String(F("Snezhinki spryatany za mnoy, vozmi otvertku i otkroy.")));
//Serial.println("P Ok "); 
password3.reset(); //сброс переменной пароля после корректного ввода 
}
else { 
//Serial.println("P No "); 
delay(200);
digitalWrite(greenLED,LOW); //активация светодиода красного при неверном коде 
delay(200); 
digitalWrite(greenLED,HIGH); //отключение красного светодиода. 
delay(200);
digitalWrite(greenLED,LOW); //активация светодиода красного при неверном коде 
delay(200); 
digitalWrite(greenLED,HIGH); //отключение красного светодиода. 
delay(200);
digitalWrite(greenLED,LOW); //активация светодиода красного при неверном коде 
delay(200); 
digitalWrite(greenLED,HIGH); //отключение красного светодиода. 
password1.reset(); //сброс переменной пароля после некорректного ввода 
password2.reset(); //сброс переменной пароля после некорректного ввода 
password3.reset(); //сброс переменной пароля после некорректного ввода 
} 
}

void sendSMS(String phone, String message)
{
  sendATCommand("AT+CMGS=\"" + phone + "\"", true);             // Переходим в режим ввода текстового сообщения
  sendATCommand(message + "\r" + (String)((char)26), true);   // После текста отправляем перенос строки и Ctrl+Z
}
