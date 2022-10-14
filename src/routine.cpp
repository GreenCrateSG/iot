#include "routine.h"

// TODO: Auto turn ON light after if lux is low OPT
void auto_light(float _v)
{

    if (_v < LUX_SWITCH)
    {
      digitalWrite(LIGHT_ONE_PIN, LOW);
      digitalWrite(LIGHT_TWO_PIN, LOW);
      digitalWrite(LIGHT_THREE_PIN, LOW);
      digitalWrite(LIGHT_FOUR_PIN, LOW);
    }
    else
    {
      digitalWrite(LIGHT_ONE_PIN, LOW);
      digitalWrite(LIGHT_TWO_PIN, LOW);
      digitalWrite(LIGHT_THREE_PIN, LOW);
      digitalWrite(LIGHT_FOUR_PIN, LOW);
    }


}

// TODO: Auto setting the EC value
