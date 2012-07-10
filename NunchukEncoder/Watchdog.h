
class Watchdog {
  public:
    Watchdog(unsigned long t) : duration(t), deadline(t + millis()) {}
    
    void reset() {
     deadline = duration + millis(); 
    }
    
    bool hasExpired() {
      return ( (int)(millis() - deadline)) > 0;
    }
  private:
  unsigned long duration;
  volatile unsigned long deadline;
};
