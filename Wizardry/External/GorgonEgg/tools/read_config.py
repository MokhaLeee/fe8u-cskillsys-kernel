
import sys
import shutil
import xml.etree.ElementTree as ET
from pathlib import Path
from argparse import ArgumentParser


def get_attrib(elem: ET.Element, key: str) -> object:
  """Get a case-insensitive attribute from an Element."""
  lowered = {k.lower(): v for k, v in elem.attrib.items()}
  return lowered.get(key.lower())


def get_required_attrib(
    elem: ET.Element,
    key: str,
    error: str,
  ) -> object:
  """
  Get a case-insensitive required attribute from an Element.

  This throws an error if the attribute is not found.
  """
  if (attrib := get_attrib(elem, key)) is None:
    raise ValueError(error)
  return attrib


def discover_modules(path: Path) -> dict[str, tuple[Path, dict]]:
  """Search through a folder and find all modules."""
  modules = {}

  # A valid module is a folder that contains a .xml file within it,
  # which consists of a <Module Name="ModuleName">...</Module> tag.

  for potential in [f for f in path.glob("**/*.xml")]:
    tree = ET.parse(potential)
    root = tree.getroot()

    if root.tag.lower() != "module":
      continue

    # Modules should have a `Name` attribute that defines their
    # name, but if it's missing we'll default to using the filename
    # of the module.

    module_name = get_attrib(root, "name")
    if module_name is None:
      module_name = str(potential.stem)

    modules[module_name] = potential

  return modules


def read_config(
    config_path: Path
  ) -> tuple[list[str], dict[str, str], dict[str, str]]:
  """Read a config file and return its modules and definitions."""
  # The top-level element of our config should always be <Config>.
  tree = ET.parse(config_path)
  root = tree.getroot()

  if root.tag.lower() != "config":
    raise ValueError(
        f"Configuration file '{config_path}' missing opening <Config> tag."
      )

  modules, definitions, asm_definitions = [], {}, {}

  for child in root:

    match child.tag.lower():

      case "module":

        modules.append(get_required_attrib(
            child,
            "name",
            "<Module> tag is missing its 'Name' attribute."
          ))

      case "definition":

        name = get_required_attrib(
            child,
            "name",
            "<Definition> tag is missing its 'Name' attribute."
          )

        # A definition can have either a `Value` attribute
        # or it can use its text to write out the definition, like:
        # <Definition Name="Foo">
        #   #define Foo(bar) \
        #     Frobber(bar);
        # </Definition>
        # The `Name` attribute here is used to identify the
        # definition in either case.

        value = get_attrib(child, "value")
        text = child.text

        if (value is None) and (text.strip() == ""):
          raise ValueError(
              "<Definition> tag is missing its value."
            )

        if value is not None:
          definitions[name] = f"#define {name} {value}"

        else:
          definitions[name] = text

      case "asmdefinition":

        name = get_required_attrib(
            child,
            "name",
            "<ASMDefinition> tag is missing its 'Name' attribute."
          )

        # An ASM definition can have either a `Value` attribute
        # or it can use its text to write out the definition, like:
        # <ASMDefinition Name="Foo">
        #   SET_FUNC Foo, (0x08B2A610+1)
        # </ASMDefinition>
        # The `Name` attribute here is used to identify the
        # definition in either case.

        value = get_attrib(child, "value")
        text = child.text

        if (value is None) and (text.strip() == ""):
          raise ValueError(
              "<ASMDefinition> tag is missing its value."
            )

        if value is not None:
          asm_definitions[name] = f".set {name}, {value}"

        else:
          asm_definitions[name] = text

  return modules, definitions, asm_definitions


def attr_error(tag: str, attr: str, module: Path) -> str:
  """Build an error message for missing attributes."""
  return f"<{tag}> in module '{module}' is missing its '{attr}' attribute."


def read_module(module_path: Path, module_contents: dict):
  """Read a module."""
  tree = ET.parse(module_path)
  root = tree.getroot()

  for child in root:

    def standard_call(tag: str) -> None:
      """
      Fetch module contents for a standard call component.

      For module components that consist of some function name and
      a file to include, store the function name to a specific
      module_contents set and add the file name to our Include set.
      """
      file = get_required_attrib(
          child,
          "file",
          attr_error(tag, "File", module_path),
        )
      name = get_required_attrib(
          child,
          "name",
          attr_error(tag, "Name", module_path),
        )
      module_contents[tag].add(name)
      module_contents["Include"].add(module_path.with_name(file))

    match child.tag.lower():

      case "include":
        file = get_required_attrib(
            child,
            "file",
            attr_error("Include", "File", module_path),
          )
        module_contents["Include"].add(module_path.with_name(file))

      case "definition":
        name = get_required_attrib(
            child,
            "name",
            attr_error("Definition", "Name", module_path),
          )

        # A definition can have either a `Value` attribute
        # or it can use its text to write out the definition, like:
        # <Definition Name="Foo">
        #   #define Foo(bar) \
        #     Frobber(bar);
        # </Definition>
        # The `Name` attribute here is used to identify the
        # definition in either case.

        value = get_attrib(child, "value")
        text = child.text

        if (value is None) and (text.strip() == ""):
          raise ValueError(
              f"<Definition> tag in module '{module_path}' "
              f"is missing its value."
            )

        if value is not None:
          module_contents["Definition"][name] = f"#define {name} {value}"

        else:
          module_contents["Definition"][name] = text

      case "asmdefinition":
        name = get_required_attrib(
            child,
            "name",
            attr_error("ASMDefinition", "Name", module_path),
          )

        # An ASM definition can have either a `Value` attribute
        # or it can use its text to write out the definition, like:
        # <ASMDefinition Name="Foo">
        #   SET_FUNC Foo, (0x08B2A610+1)
        # </ASMDefinition>
        # The `Name` attribute here is used to identify the
        # definition in either case.

        value = get_attrib(child, "value")
        text = child.text

        if (value is None) and (text.strip() == ""):
          raise ValueError(
              f"<ASMDefinition> tag in module '{module_path}' "
              f"is missing its value."
            )

        if value is not None:
          module_contents["ASMDefinition"][name] = f".set {name}, {value}"

        else:
          module_contents["ASMDefinition"][name] = text

      case "internal":
        file = get_required_attrib(
            child,
            "file",
            attr_error("Internal", "File", module_path),
          )

        # Use the module's first part (which should be either
        # `custom` or `source`) as the first part of the internal
        # file's path.

        flavor = module_path.parts[0]
        module_contents["Internal"].add(Path(flavor) / "internal" / file)

      case "static":
        standard_call("Static")

      case "dynamic":
        standard_call("Dynamic")

      case "tilemapdestination":
        standard_call("TilemapDestination")

      case "extend":
        standard_call("Extend")

      case "retract":
        standard_call("Retract")

      case "init":
        standard_call("Init")

      case "invalidposition":
        standard_call("InvalidPosition")


def implement_config(
    requested_modules: list[str],
    override_definitions: dict[str, str],
    override_asm_definitions: dict[str, str],
    available_modules: dict[str, Path],
    output_folder: Path,
  ) -> None:
  """Fetch modules and generate output installers and definitions."""
  # Before we do any real work, throw an error if we can't
  # find our config's modules in our available modules.

  for module in requested_modules:
    if module not in available_modules.keys():
      raise ValueError(f"Unknown module '{module}'.")

  module_contents = {
      "Definition": {},
      "ASMDefinition": {},
      "Include": set(),
      "Internal": set(),
      "Static": set(),
      "Dynamic": set(),
      "TilemapDestination": set(),
      "Extend": set(),
      "Retract": set(),
      "Init": set(),
      "InvalidPosition": set(),
    }

  for module in requested_modules:
    read_module(available_modules[module], module_contents)

  module_contents["Definition"] |= override_definitions
  module_contents["ASMDefinition"] |= override_asm_definitions

  # Now, actually copy our module files into our output folder.

  for module in requested_modules:
    module_folder = available_modules[module].parent
    shutil.copytree(
        module_folder.resolve(),
        (output_folder / module_folder).resolve(),
        dirs_exist_ok=True,
      )

  # And our internal files.

  for internal in module_contents["Internal"]:
    internal_folder = internal.parent
    shutil.copytree(
        internal_folder.resolve(),
        (output_folder / internal_folder).resolve(),
        dirs_exist_ok=True,
      )

  # Create our definitions files.

  asm_def_lines = ["\n".join(module_contents["ASMDefinition"].values()), ""]
  def_lines = ["\n".join(module_contents["Definition"].values()), ""]

  static_lines = ["#define STATIC_CALLS"]
  static_decls = []
  for static in module_contents["Static"]:
    static_lines.append("  " + static + ",")
    static_decls.append("void " + static + "(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp);")

  static_lines.append("  NULL")

  def_lines.append("\n".join(static_decls))
  def_lines.append(" \\\n".join(static_lines))
  def_lines.append("")

  dynamic_lines = ["#define DYNAMIC_CALLS"]
  dynamic_decls = []
  for dynamic in module_contents["Dynamic"]:
    dynamic_lines.append("  " + dynamic + ",")
    dynamic_decls.append("void " + dynamic + "(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp);")

  dynamic_lines.append("  NULL")

  def_lines.append("\n".join(dynamic_decls))
  def_lines.append(" \\\n".join(dynamic_lines))
  def_lines.append("")

  tmd_lines = ["#define TILEMAP_DESTINATION"]
  tmd_decls = []
  for tmd in module_contents["TilemapDestination"]:
    tmd_lines.append("  " + tmd + ",")
    tmd_decls.append("struct Vec2 " + tmd + "(struct PlayerInterfaceProc* proc, struct Vec2 coords);")

  tmd_lines.append("  NULL")

  def_lines.append("\n".join(tmd_decls))
  def_lines.append(" \\\n".join(tmd_lines))
  def_lines.append("")

  invalid_lines = ["#define INVALID_POSITION_CALLS"]
  invalid_decls = []
  for invalid in module_contents["InvalidPosition"]:
    invalid_lines.append("  " + invalid + ",")
    invalid_decls.append("bool " + invalid + "(struct PlayerInterfaceProc* proc, int quadrant, bool invalidPosition);")

  invalid_lines.append("  NULL")

  def_lines.append("\n".join(invalid_decls))
  def_lines.append(" \\\n".join(invalid_lines))
  def_lines.append("")

  extend_lines = ["#define EXTEND_CALLS"]
  extend_decls = []
  for extend in module_contents["Extend"]:
    extend_lines.append("  " + extend + ",")
    extend_decls.append("void " + extend + "(struct PlayerInterfaceProc* proc);")

  extend_lines.append("  NULL")

  def_lines.append("\n".join(extend_decls))
  def_lines.append(" \\\n".join(extend_lines))
  def_lines.append("")

  retract_lines = ["#define RETRACT_CALLS"]
  retract_decls = []
  for retract in module_contents["Retract"]:
    retract_lines.append("  " + retract + ",")
    retract_decls.append("void " + retract + "(struct PlayerInterfaceProc* proc);")

  retract_lines.append("  NULL")

  def_lines.append("\n".join(retract_decls))
  def_lines.append(" \\\n".join(retract_lines))
  def_lines.append("")

  init_lines = ["#define INIT_CALLS"]
  init_decls = []
  for init in module_contents["Init"]:
    init_lines.append("  " + init + ",")
    init_decls.append("void " + init + "(struct PlayerInterfaceProc* proc);")

  init_lines.append("  NULL")

  def_lines.append("\n".join(init_decls))
  def_lines.append(" \\\n".join(init_lines))
  def_lines.append("")

  with (output_folder / "GeneratedASMDefinitions.s").open("w") as asmdefsfile:
    asmdefsfile.write("\n".join(asm_def_lines))

  with (output_folder / "GeneratedDefinitions.h").open("w") as defsfile:
    defsfile.write("\n".join(def_lines))

  # Create our installer.

  installer_lines = []
  for file in module_contents["Include"]:
    installer_lines.append(f'#include "{file}"')

  for file in module_contents["Internal"]:
    installer_lines.append(f'#include "{file}"')

  with (output_folder / "GeneratedInstaller.event").open("w") as installerfile:
    installerfile.write("\n".join(installer_lines))


def main() -> int:
  """
  Handle commandline parsing.

  Parse a configuration file to discover modules and definitions to include
  in our `output` folder.
  """
  parser = ArgumentParser()
  parser.add_argument(
      "config",
      type=Path,
      help="An XML-formatted configuration file.",
    )
  parser.add_argument(
      "output_folder",
      type=Path,
      help="A folder to place the output into.",
    )
  parser.add_argument(
      "native_module_folder",
      type=Path,
      help="A folder containing default modules.",
    )
  parser.add_argument(
      "custom_module_folder",
      type=Path,
      help="A folder containing custom modules.",
    )
  args = parser.parse_args()

  # Before we begin parsing the config, test if our
  # folders and config actually exist.

  if not (config := args.config).is_file():
    raise ValueError(f"Unable to open configuration file '{config}'.")

  native_module_folder = args.native_module_folder
  custom_module_folder = args.custom_module_folder
  output_folder = args.output_folder

  folders = [native_module_folder, custom_module_folder, output_folder]
  for folder in folders:
    if not folder.is_dir():
      raise ValueError(f"'{folder}' is not a valid directory.")

  # We're going to assemble a dict of available modules
  # before reading the config, which should make it easier
  # to select the ones actually used in our config.

  available_modules = discover_modules(native_module_folder)
  available_modules |= discover_modules(custom_module_folder)

  # Fetch our module list and config defs.

  config_modules, config_defs, config_asm_defs = read_config(config)

  implement_config(
      config_modules, config_defs, config_asm_defs,
      available_modules, output_folder,
    )

  return 0


if __name__ == '__main__':
  sys.exit(main())
