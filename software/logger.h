#ifndef LOGGER_H
#define LOGGER_H

// Logger macros
#ifdef DEBUG
    #define LOG_I(text) Serial.print(F("[INFO] ")); \
                        Serial.print(F(text));      \
                        Serial.print(F("\n")) 

    #define LOG_E(text) Serial.print(F("[ERROR] ")); \
                        Serial.print(F(text));       \
                        Serial.print(F("\n"))
    
    #define LOG_TAG_M() Serial.print(F("[MEAS] "))

    #define LOG_M(text) Serial.print(F(text))
#else
    #define LOG_I(x)
    #define LOG_E(text) Serial.print(F("[ERROR] ")); \
                        Serial.print(F(text));       \
                        Serial.print(F("\n"))
    #define LOG_TAG_M()
    #define LOG_M(x)
#endif

#define SEND_MEAS(val) Serial.print(val);     \
                       Serial.print(F(CSV_DIV))

#define SEND_MEAS(val, dec) Serial.print(val, dec);     \
                            Serial.print(F(CSV_DIV)) 

#define SEND_ENDLN()     Serial.print(F("\n"))

#endif // LOGGER_H
