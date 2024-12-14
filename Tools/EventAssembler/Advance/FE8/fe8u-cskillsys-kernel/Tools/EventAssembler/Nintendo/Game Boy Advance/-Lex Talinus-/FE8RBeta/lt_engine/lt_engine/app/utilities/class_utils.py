
def recursive_subclasses(ctype: type):
    all_subclasses = []
    subclasses = ctype.__subclasses__()
    for subclass in subclasses:
        all_subclasses += recursive_subclasses(subclass)
    all_subclasses += subclasses
    return all_subclasses