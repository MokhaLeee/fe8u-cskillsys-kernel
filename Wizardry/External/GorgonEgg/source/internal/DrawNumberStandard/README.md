
These files provide convenient functions for drawing numbers (as objects) the way that the vanilla window displays HP numbers.

Module declarations should use
```xml
<Internal File="DrawNumberStandard/DrawNumberDigit.lyn.event"/>
```

in addition to the needed internal file (such as `DrawUnsignedSmallNumber.lyn.event`, etc.).

At the moment the available number drawing files are:

  * `DrawUnsignedSmallNumber`: Call `DrawUnsignedSmallNumber` to draw a number smaller than 255
