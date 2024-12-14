from app.map_maker.terrain_database import DB_terrain
from app.map_maker.map_prefab import MapPrefab
from app.map_maker import simplex_noise

# Generate truly random terrain

def generate(tilemap: MapPrefab) -> MapPrefab:
    tilemap.resize(30, 30, 0, 0)
    thickness = 0.25
    
    for x in range(tilemap.width):
        for y in range(tilemap.height):
            pos = (x, y)
            value = simplex_noise.get(pos[0] * thickness, pos[1] * thickness, 0)
            if value > 0.75:
                tilemap.set(pos, None, DB_terrain.get('Hill'))
            elif value > 0.4:
                tilemap.set(pos, None, DB_terrain.get('Plains'))
            elif value > 0.3:
                tilemap.set(pos, None, DB_terrain.get('Sand'))
            else:
                tilemap.set(pos, None, DB_terrain.get('Sea'))

    return tilemap
