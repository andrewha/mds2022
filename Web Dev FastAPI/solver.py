import numpy as np

def solve_equation(coeffs: dict) -> list[float]:
    '''
                               2
    Solve quadratic equation ax + bx + c = 0
         2
    D = b - 4ac
                                  b
    1) If D = 0, one root: x = - --- ;
                                 2a
    2) If D < 0, no real roots;
                                           ______                  ______
                                          /2                      /2
                                  -b + \\/b - 4ac         -b - \\/b - 4ac
    3) If D > 0, two roots: x_1 = --------------- , x_2 = --------------- ;
                                        2a                      2a
                                             c
    4) If a = 0 and b != 0, one root: x = - --- ;
                                             b
    
    5) If a = 0 and b = 0 and c = 0, horizontal line y = 0, infinite number of roots;
    
    6) If a = 0 and b = 0 and c != 0, horizontal line y = c, no real roots.
    
    Parameters
    ----------
    coeffs : dict
        Dictionary of coefficients a, b, and c.

    Returns
    -------
    roots : list[float]
        List of real roots (floats).
        If no real roots, return an empty list.
        If infinite number of roots, return list with the string 'infinite number of roots'.
    '''
    a = coeffs['a']
    b = coeffs['b']
    c = coeffs['c']
    roots = []
    if a == 0: # Linear case
        if b == 0: # horizontal line
            if c != 0: # no real roots
                return roots
            else: # infinite number of roots
                x = 'infinite number of roots'
                roots.append(x)
                return roots
        x = round(-c / b, 6)
        if np.abs(x) == 0.0: # convert -0.0 to 0.0
            x = 0.0
        roots.append(x)
        return roots
    # Quadratic case
    D = b ** 2 - 4 * a * c
    if D == 0: # one real root
        x = round(-b / (2 * a), 6)
        if np.abs(x) == 0.0:
            x = 0.0
        roots.append(x)
        return roots
    elif D > 0: # two real roots
        x_1 = round((-b + np.sqrt(D)) / (2 * a), 6)
        if np.abs(x_1) == 0.0:
            x_1 = 0.0
        roots.append(x_1)
        x_2 = round((-b - np.sqrt(D)) / (2 * a), 6)
        if np.abs(x_2) == 0.0:
            x_2 = 0.0
        roots.append(x_2)
        return sorted(roots)
    else: # no real roots
        return roots

def format_title(a: int, b: int, c: int) -> str:
    '''
    Construct a string for polynomial with coefficients a, b, c.
    Return examples:
    $f(x)=5x^2+10x-1$ (for a = 5, b = 10, c = -1)
    $f(x)=-x+5$       (for a = 0, b = -1, c = 5)
    '''
    if a == 0:
        a_str = ''
    elif a == 1:
        a_str = 'x^2'
    elif a == -1:
        a_str = '-x^2'
    else:
        a_str = f'{a}x^2'
    if a_str:
        if b == 1:
            b_str = '+x'
        elif b == -1:
            b_str = '-x'
        else:
            b_str = f'{b:+}x'
    else:
        if b == 1:
            b_str = 'x'
        elif b == -1:
            b_str = '-x'
        else:
            b_str = f'{b:}x'
    if b == 0:
        b_str = ''
    if a_str or b_str:
        if c == 0:
            c_str = ''
        else:
            c_str = f'{c:+}'
    else:
        c_str = f'{c}'
    return f'$f(x)={a_str}{b_str}{c_str}$'
