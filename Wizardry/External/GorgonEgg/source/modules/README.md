
# Modules

This folder is for modules that come prepackaged with GORGON-EGG.

A `module` is a package of code, data, graphics, etc. that typically represents a single window element. A user's `config` file selects any number of `module`s to apply to the window. A module consists of a folder that contains exactly one `module declaration` file and any number of other files. When building, all `modules` required by the `config` are gathered into the `output` folder and are used to generate a number of output files.

The modules selected by the config (in addition to any definition overrides specified in the config) come together to generate three special files in the output called `GeneratedInstaller.event`, `GeneratedDefinitions.h` and `GeneratedASMDefinitions.s`.

When a `module declaration` file specifies a file to include, it refers to the output file required, that is if you need a function in `Example.c`, the file you specify would be `Example.lyn.event`, and the build process will automatically take care of generating `Example.lyn.event` from `Example.c`.

---

## Module Declarations

A `module declaration` file is an XML-formatted file (with the .xml extension) that details what a module provides to the window, which files it uses, and what definitions it creates. `Module declaration` files consist of exactly one `Module` tag that contains any number of the following tags:

  * `Include`: Add an EA-formatted file to the Event Assembler installer output
  * `Definition`: Add a C definition to the generated C header
  * `ASMDefinition`: Add a THUMB assembly definition to the generated ASM include
  * `Internal`: Add an EA-formatted internal file to the Event Assembler installer output
  * `Static`, `Dynamic`, `TilemapDestination`, `Extend`, `Retract`, `Init`, and `InvalidPosition`: Add a file/function that handles a specific window task

For example, a simple `module declaration` file might look something like this:
```xml
<Module Name="ExampleModule">
  <Static File="ExampleModule.lyn.event" Name="ExampleModule_Static"/>
  <Internal File="ExampleInternal.lyn.event"/>
  <Definition Name="ExampleDefinition1" Value="42"/>
  <Definition Name="ExampleDefinition2">
    #define Foo(bar) ((bar) + 42)
  </Definition>
</Module>
```

---

### `Include`

`Include` tags specify files to be included in the resulting Event Assembler installer, and are typically used for things that are not well-expressed in C or THUMB assembly.

An `Include` tag has one attribute, named `File`, which specifies the file path (relative to the module declaration file) of the file to be included.

The file should be formatted for being `#include`d using Event Assembler.

Example, in the `SomeModule` folder:
```xml
<Include File="SomeEAFile.event"/>
```

This generates the following line in the generated Event Assembler installer:
```
#include "source/SomeModule/SomeEAFile.event"
```

---

### `Definition`

`Definition`s are used to add defined values, macros, or other things for use in C source files.

A `Definition` tag has two forms. The first form has the attributes `Name` and `Value`. This adds a definition into the generated C header file with the form
```c
#define Name Value
```

As such, `Name` must be a valid C identifier.

Example:
```xml
<Definition Name="Foo" Value="42"/>
```

This generates the following definition in the generated C header file:
```c
#define Foo 42
```

The second form of the `Definition` tag has one attribute, `Name`, which is used internally to determine definition shadowing. In this form, a `Definition` tag's text is used to form the definition, and is copied verbatim into the generated C header file.

Example:
```xml
<Definition Name="Foo">
  #define Foo(bar) ((bar) + 42)
</Definition>
```

This generates the following definition in the generated C header file:
```c
#define Foo(bar) ((bar) + 42)
```

This allows for arbitrary definitions, macros, etc. to be defined by a module.

---

### `ASMDefinition`

`ASMDefinition`s are used to add defined values, macros, or other things for use in THUMB assembly source files.

An `ASMDefinition` tag has two forms. The first form has the attributes `Name` and `Value`. This adds a definition into the generated THUMB assembly include file with the form
```arm
.set Name, Value
```

As such, the `Name` must be a valid ASM identifier.

Example:
```xml
<ASMDefinition Name="Bar" Value="42"/>
```

This generates the following definition in the generated THUMB assembly include file:
```arm
.set Bar, 42
```

The second form of the `ASMDefinition` tag has one attribute, `Name`, which is used internally to determine definition shadowing. In this form, an `ASMDefinition` tag's text is used to form the definition, and is copied verbatim into the generated THUMB assembly include file.

Example:
```xml
<ASMDefinition Name="Foo">
  .macro Foo Frob, Baz
    .long \Frob, \Baz
  .endm
</ASMDefinition>
```

This generates the following definition in the generated THUMB assembly include file:
```arm
.macro Foo, Frob, Baz
  .long \Frob, \Baz
.endm
```

---

### `Internal`

`Internal` tags specify Event Assembler-formatted files found in the `internal` folder that are commonly shared between multiple modules.

An `Internal` tag has one attribute, `File`, which specifies the path of the file, relative to the `internal` folder.

Example:
```xml
<Internal File="SomeSharedThing/SomeSharedThing.lyn.event"/>
```

This generates the following line in the generated Event Assembler installer:
```
#include "source/internal/SomeSharedThing/SomeSharedThing.lyn.event"
```

---

### `Static`

`Static` tags define functions that are run when the window is being rebuilt.

A `Static` tag has two attributes, `File` and `Name`. The `Name` attribute specifies the function's name, and the `File` attribute specifies the file that the function is found in.

Example:
```xml
<Static File="Example.lyn.event" Name="Example_Static"/>
```

This will `#include` the file (if specified by multiple tags, it will only be included once) in the generated Event Assembler installer and will add a function pointer to the `STATIC_CALLS` macro in the generated C header file, which will be called like
```c
Example_Static(proc, udp);
```

See the [Static source file](../../template/Static.c) for more information.

---

### `Dynamic`

`Dynamic` tags define functions that are run every frame that the window is active.

A `Dynamic` tag has two attributes, `File` and `Name`. The `Name` attribute specifies the function's name, and the `File` attribute specifies the file that the function is found in.

Example:
```xml
<Dynamic File="Example.lyn.event" Name="Example_Dynamic"/>
```

This will `#include` the file (if specified by multiple tags, it will only be included once) in the generated Event Assembler installer and will add a function pointer to the `DYNAMIC_CALLS` macro in the generated C header file, which will be called like
```c
Example_Dynamic(proc, udp);
```

See the [Dynamic source file](../../template/Dynamic.c) for more information.

---

### `TilemapDestination`

`TilemapDestination` tags define functions that determine where the (full extended) tilemap is drawn to. Typically, a config includes exactly one module that contains this tag.

A `TilemapDestination` tag has two attributes, `File` and `Name`. The `Name` attribute specifies the function's name, and the `File` attribute specifies the file that the function is found in.

Example:
```xml
<TilemapDestination File="Example.lyn.event" Name="Example_TilemapDestination"/>
```

This will `#include` the file (if specified by multiple tags, it will only be included once) in the generated Event Assembler installer and will add a function pointer to the `TILEMAP_DESTINATION` macro in the generated C header file, which will be called like
```c
coords = Example_TilemapDestination(proc, coords);
```

See the [CopyTilemap](../../template/CopyTilemap.c) and [GetWindowPosition](../../template/GetWindowPosition.c) files for more information.

---

### `Extend`

`Extend` tags define functions that handle extending the window every frame.

A `Extend` tag has two attributes, `File` and `Name`. The `Name` attribute specifies the function's name, and the `File` attribute specifies the file that the function is found in.

Example:
```xml
<Extend File="Example.lyn.event" Name="Example_Extend"/>
```

This will `#include` the file (if specified by multiple tags, it will only be included once) in the generated Event Assembler installer and will add a function pointer to the `EXTEND_CALLS` macro in the generated C header file, which will be called like
```c
Example_Extend(proc);
```

See the [Extend source file](../../template/Extend.c) for more information.

### `Retract`

`Retract` tags define functions that handle retracting the window every frame.

A `Retract` tag has two attributes, `File` and `Name`. The `Name` attribute specifies the function's name, and the `File` attribute specifies the file that the function is found in.

Example:
```xml
<Retract File="Example.lyn.event" Name="Example_Retract"/>
```

This will `#include` the file (if specified by multiple tags, it will only be included once) in the generated Event Assembler installer and will add a function pointer to the `RETRACT_CALLS` macro in the generated C header file, which will be called like
```c
Example_Retract(proc);
```

See the [Retract source file](../../template/Retract.c) for more information.

---

### `Init`

`Init` tags define functions that handle initializing data that must only happen once, before the window is even built. It is for things that do not rely on the unit being selected.

An `Init` tag has two attributes, `File` and `Name`. The `Name` attribute specifies the function's name, and the `File` attribute specifies the file that the function is found in.

Example:
```xml
<Init File="Example.lyn.event" Name="Example_Init"/>
```

This will `#include` the file (if specified by multiple tags, it will only be included once) in the generated Event Assembler installer and will add a function pointer to the `INIT_CALLS` macro in the generated C header file, which will be called like
```c
Example_Init(proc);
```

See the [Init source file](../../template/Init.c) for more information.

---

### InvalidPosition

`InvalidPosition` tags define functions that return `TRUE` if the window should not be rebuilt.

A `InvalidPosition` tag has two attributes, `File` and `Name`. The `Name` attribute specifies the function's name, and the `File` attribute specifies the file that the function is found in.

Example:
```xml
<InvalidPosition File="Example.lyn.event" Name="Example_InvalidPosition"/>
```

This will `#include` the file (if specified by multiple tags, it will only be included once) in the generated Event Assembler installer and will add a function pointer to the `INVALID_POSITION_CALLS` macro in the generated C header file, which will be called like
```c
invalidPosition |= Example_InvalidPosition(proc, quadrant, invalidPosition);
```

See the [Reset source file](../../template/Reset.c) for more information.

---
