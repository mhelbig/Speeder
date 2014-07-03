char readUserInputs(void)
{
  if (Serial.available() > 0)
  {
    return(toupper(Serial.read()));
  }
  else
  {
    return(0);
  }
}
