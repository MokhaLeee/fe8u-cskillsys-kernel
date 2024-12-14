from app.utilities import utils

def distance_from_line(p1, p2, q, strict=True):
    """Generates the distance between point q and the line described by points p1 and p2

    Args:
        p1 tuple(int, int): one point on the line
        p2 tuple(int, int): another point on the line
        q tuple(int, int): the point off the line whose distance we're interested in
        strict (bool): whether or not the line is strictly a line segment, i.e., q's distance should be calculated
                       to the segment rather than the whole line they represent

    Returns:
        (double): the distance between point q and the line p1-p2
    """
    # ax + by + c = 0
    _x = q[0] - p1[0]
    _y = q[1] - p1[1]

    x = p2[0] - p1[0]
    y = p2[1] - p1[1]

    dot = _x * x + _y * y
    len_sq = x * x + y * y

    mult = dot / len_sq

    if mult < 0 and strict:
        x_i = p1[0]
        y_i = p1[1]
    elif mult > 1 and strict:
        x_i = p2[0]
        y_i = p2[1]
    else:
        x_i = p1[0] + mult * x
        y_i = p1[1] + mult * y

    return utils.distance(q, (x_i, y_i))

def float_eq(a: float, b: float, epsilon: float):
    return abs(a - b) < epsilon