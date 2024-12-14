class MinimapCatalog():
    single_map = {'Grass': (1, 0),
                  'House': (2, 0),
                  'Shop': (3, 0),
                  'Switch': (4, 0),
                  'Fort': (5, 0),
                  'Ruins': (6, 0),
                  'Forest': (8, 0),
                  'Thicket': (9, 0),
                  'Hill': (11, 0),
                  'Floor': (12, 0),
                  'Pillar': (13, 0),
                  'Throne': (14, 0),
                  'Chest': (15, 0),
                  'Mountain': (4, 1),
                  'Desert': (10, 0),
                  'Waste': (11, 1),
                  'Snow': (12, 1),
                  'Dark_Snow': (13, 1),
                  'Pier': (14, 1)}
    complex_map = {'Wall': (0, 2),
                   'River': (0, 3),
                   'Sand': (0, 4),
                   'Sea': (0, 5),
                   'Lava': (0, 8)}
    other_map = {'Cliff': (8, 6),
                 'Desert_Cliff': (12, 6),
                 'Snow_Cliff': (4, 7),
                 'Door': (7, 1),
                 'Bridge': (0, 1),
                 'Coast': (0, 7),
                 'Lava_Coast': (0, 10)}

    def get_minimap_types(self):
        r = list(self.single_map.items()) + list(self.complex_map.items()) + list(self.other_map.items())
        return r
