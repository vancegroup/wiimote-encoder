//#include <Streaming.h>
#include <Arduino.h>
template<typename InType, typename OutType>
class AutoRanging {
public:
  AutoRanging(OutType lowValue, OutType highValue) : lowVal(lowValue), highVal(highValue), outRange(highValue - lowValue) {}
  void setCenter(InType center, InType initialSpread = 1) {
    centerVal = center;
    minVal = center - initialSpread;
    maxVal = center + initialSpread; 
    updateRange();
  }

  OutType process(InType inVal) {
    lastInVal = inVal;
    if (inVal < minVal) {
      minVal = inVal;
      updateRange();
    } 
    else if (inVal > maxVal) {
      maxVal = inVal;
      updateRange();
    }
    const InType clampedVal = inVal < bottomInput ? bottomInput : (inVal > topInput ? topInput : inVal);
    lastValue = (clampedVal - bottomInput) * (outRange/static_cast<float>(inRange)) + lowVal;
    return lastValue;

  }
  
  void dumpStatus(Print & s) const {
    s.print("Seen range [");
   s.print(minVal);
   s.print(", ");
   s.print(maxVal);
   s.println("]");
   s.print("Clamping to range [");
   s.print(bottomInput);
   s.print(", ");
   s.print(topInput);
   s.println("]");
   s.print("Last values: input ");
   s.print(lastInVal);
   s.print(" -> output ");
   s.println(lastValue);
  }

private:
  void updateRange() {
    InType minDist = min(abs(centerVal - minVal), abs(centerVal - maxVal));
    inRange = 2 * minDist;
    topInput = centerVal + minDist;
    bottomInput = centerVal - minDist;
  }
  const OutType lowVal;
  const OutType highVal;
  const OutType outRange;
  OutType lastValue;


  InType lastInVal;
  InType centerVal;
  InType minVal;
  InType maxVal;
  InType inRange;

  InType topInput;
  InType bottomInput;
};
