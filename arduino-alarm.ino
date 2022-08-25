// TO CONFIGURE!!!
bool s_debug = false;
long s_min_minutes = 25;
long s_max_minutes = 35;
long s_hours_till_start = 27;

long ONE_SECOND = 1000;
long ONE_MINUTE = ONE_SECOND * 60;
long ONE_HOUR = ONE_MINUTE * 60;

// global variables
bool m_started = false;
long s_start = ONE_HOUR * s_hours_till_start;
long m_min_wait = ONE_MINUTE * s_min_minutes;
long m_max_wait = ONE_MINUTE * s_max_minutes;
float s_accelerator = 0.9;

int s_pulse_time = ONE_SECOND * 1.5;
int s_led_pin = 13;

// debug variables
int s_pulse_pin = 5;
int s_debug_high_pin = 10;

void setup_debug()
{
  if (!s_debug)
    return;

  Serial.begin(9600);
  s_start = ONE_SECOND * 10;
  m_min_wait = 20*ONE_SECOND;
  m_max_wait = 60 * ONE_SECOND;
  s_pulse_time = ONE_SECOND * 0.1;

  pinMode(s_debug_high_pin, OUTPUT);
  digitalWrite(s_debug_high_pin, 1);  
}

void setup()
{  
  setup_debug();
  
  // Set led on board off to reduce power consumption
  pinMode(s_led_pin, OUTPUT);
  digitalWrite(s_led_pin, LOW);

  pinMode(s_pulse_pin, OUTPUT);
}


void pulse()
{
  if (s_debug)
    Serial.println("Enabling pulse for " + String(s_pulse_time/ONE_SECOND) + " seconds");
    
  digitalWrite(s_pulse_pin, HIGH);
  delay(s_pulse_time);
  digitalWrite(s_pulse_pin, LOW);

  if (s_debug)
    Serial.println("Disabling pulse");
}


long calculate_wait_time()
{
  if (!m_started)
  {
    m_started = true;
    return s_start;
  }
  
  long wait_time = random(m_min_wait, m_max_wait);
  m_min_wait = max(round(m_min_wait*s_accelerator), 0);
  if (s_debug)
    Serial.println("Next min wait time: " + String(m_min_wait));
  m_max_wait = max(round(m_max_wait*s_accelerator), 1);
  if (s_debug)
    Serial.println("Next max wait time: " + String(m_max_wait));
  return wait_time;
}

long calculate_time_to_wait(long start_wait, long wait_time)
{
  return wait_time - (millis() - start_wait);
}

void wait()
{
  long wait_time = calculate_wait_time();

  if (s_debug) 
  {
    Serial.println("Waiting for " + String(wait_time/ONE_SECOND) + " seconds");
    Serial.flush();
  }

  long start_wait = millis();
  long eight_seconds = 8*ONE_SECOND;
  while (calculate_time_to_wait(start_wait, wait_time) > eight_seconds*2)
  {
    delay(eight_seconds);
  }

  long time_to_wait = calculate_time_to_wait(start_wait, wait_time);
  delay(time_to_wait);
}

void loop()
{
  wait();
  pulse();
}
