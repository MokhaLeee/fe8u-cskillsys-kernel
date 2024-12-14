def get_line(start: tuple, end: tuple, get_opacity) -> bool:
    """
    # Takes in two positions
    # and a function that takes in a position and returns
    # whether it can be seen through
    # and determines whether there is a valid line
    """
    if start == end:
        return True
    x1, y1 = start
    x2, y2 = end
    dx = x2 - x1
    dy = y2 - y1
    x, y = x1, y1

    xstep, ystep = 1, 1
    if dy < 0:
        ystep = -1
        dy = -dy
    if dx < 0:
        xstep = -1
        dx = -dx
    ddy, ddx = 2*dy, 2*dx

    if ddx >= ddy:
        errorprev = error = dx
        for i in range(dx):
            x += xstep
            error += ddy
            # How far off the straight line to the right are you
            if error > ddx:
                y += ystep
                error -= ddx
                if error + errorprev < ddx:  # Bottom square
                    pos = x, y - ystep
                    if pos != end and get_opacity(pos):
                        return False
                elif error + errorprev > ddx:  # Left square
                    pos = x - xstep, y
                    if pos != end and get_opacity(pos):
                        return False
                else:  # Through the middle
                    pos1, pos2 = (x, y - ystep), (x - xstep, y)
                    if get_opacity(pos1) and get_opacity(pos2):
                        return False
            pos = x, y
            if pos != end and get_opacity(pos):
                return False
            errorprev = error
    else:
        errorprev = error = dy
        for i in range(dy):
            y += ystep
            error += ddx
            if error > ddy:
                x += xstep
                error -= ddy
                if error + errorprev < ddy:  # Bottom square
                    pos = x - xstep, y
                    if pos != end and get_opacity(pos):
                        return False
                elif error + errorprev > ddy:  # Left square
                    pos = x, y - ystep
                    if pos != end and get_opacity(pos):
                        return False
                else:  # Through the middle
                    pos1, pos2 = (x, y - ystep), (x - xstep, y)
                    if get_opacity(pos1) and get_opacity(pos2):
                        return False
            pos = x, y
            if pos != end and get_opacity(pos):
                return False
            errorprev = error
    assert x == x2 and y == y2
    return True
