from app.engine.component_system.compile_item_system import compile_item_system
from app.engine.component_system.compile_skill_system import compile_skill_system
from app.events.python_eventing.generate_event_command_wrappers import generate_event_command_python_wrappers

def generate_component_system_source():
    compile_skill_system()
    compile_item_system()

def event_command_codegen():
    generate_event_command_python_wrappers()

def generate_all():
    generate_component_system_source()
    event_command_codegen()

if __name__ == '__main__':
    generate_all()
