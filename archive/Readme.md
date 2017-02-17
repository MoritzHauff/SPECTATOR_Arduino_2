# Archive

Dieser Ordner enthält alle zusätzlich verwendeten Bibliotheken und SourceCodes.

## dio2

Diese Bibliothek dient dem schnelleren Ansteuern der Digital-IO-Pins des Arduinos.
Mehr Informationen in der dio2_Explanation.pdf oder unter: 
https://www.codeproject.com/Articles/732646/Fast-digital-I-O-for-Arduino

## AnalogueInput

Ermöglicht eine genauere und vor allem schnellere Ansteuerung des Analog-Digital-
Wandlers des ATmegas2560. Die Informationen aus der AnalogueInput_Explanation.pdf 
finden sich in der Funktion get_adc() wieder.

## SharpIR-Master

Diese Bibliothek generiert stabilere und in cm umgerechnete Messwerte des Sharp-IR-Sensors.

## I2CDev

Diese Bibliothek wird für die Kommunikation mit dem MPU6050 benötigt.

## MPU6050_2

Diese Biliothek stellt sämtliche Funktionen für das MPU6050 bereit.
Dabei habe ich sie so bearbeitet, dass sie auch in Visual Micro verwendet werden kann.
Es ist also möglich diese Bibliothek ganz normal in den Arduino 'libraries'-Ordner
zu kopieren und in der ArduinoIDE und in Visual Micro zu nutzen.

## Adafruit-MLX90614

Diese Bibliothek von Adafruit wurde leicht verändert damit die sonst etablierte I2C 
Kommunikation nicht gestört wird. Aus diesem Grund muss man selbst dafür sorgen, dass 
Wire.begin() vor dem ersten Zugriff auf einen MLX aufgerufen wird.

## Credits

Moritz Hauff, 16.02.2017

TODO: Write credits



## License



TODO: Write license