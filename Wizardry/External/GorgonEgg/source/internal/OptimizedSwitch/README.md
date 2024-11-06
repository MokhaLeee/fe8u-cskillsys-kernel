
This allows for code to use GCC's `__gnu_thumb1_case_uqi` optimized switch function. When using the `switch` statement, the compiler may generate a call to this function, whcih is not present in FE8 by default.

Credit to [Contro](https://github.com/masterofcontroversy/) for this.

Module declaration files should use the following
```xml
<Internal File="OptimizedSwitch/Installer.event"/>
```

if they use the `switch` statement in their code.
