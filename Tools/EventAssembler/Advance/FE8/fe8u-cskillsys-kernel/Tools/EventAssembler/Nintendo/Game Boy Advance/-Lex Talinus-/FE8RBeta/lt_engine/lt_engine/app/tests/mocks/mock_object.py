# anonymous object boilerplate
Object = lambda **kwargs: type("Object", (), kwargs)