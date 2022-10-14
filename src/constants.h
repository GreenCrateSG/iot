/** MUST **/

#define EC_MIN 100
#define EC_MAX 150
#define LUX_SWITCH 100

/*****/

/**  Ethernet  **/

#define W5200_nSCS 10
#define W5200_nRST 8
#define W5200_PWDN 9
#define W5200_nINT 3  // unused

/*****/


// Split topic from top and bottom
/** TOPICS **/

#define TOPIC       "/arduino"
#define TOP         "/top"
#define BOTTOM      "/bottom"

#define TEMPERATURE "/temperature"
#define HUMIDITY    "/humidity"
#define LUX         "/lux"

#define LIGHT_ONE   "/light_one"
#define LIGHT_TWO   "/light_two"
#define LIGHT_THREE "/light_three"
#define LIGHT_FOUR  "/light_four"

/*****/

/** LIGHT PIN **/

#define LIGHT_ONE_PIN      40
#define LIGHT_TWO_PIN      41
#define LIGHT_THREE_PIN    42
#define LIGHT_FOUR_PIN     43

/*****/

/** PUMP PIN **/

#define MOTOR_A_CW_PIN    30
#define MOTOR_A_ACW_PIN   31
#define MOTOR_B_CW_PIN    32
#define MOTOR_B_ACW_PIN   33
#define MOTOR_CLEAN_CW_PIN 34
#define MOTOR_CLEAN_ACW_PIN 35

/*****/
