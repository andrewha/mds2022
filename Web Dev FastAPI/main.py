import io
import base64
from fastapi import FastAPI, Request, Form, HTTPException
from fastapi.templating import Jinja2Templates
from fastapi.staticfiles import StaticFiles
import matplotlib
import matplotlib.pyplot as plt
import numpy as np
from solver import solve_equation
from solver import format_title

matplotlib.use('agg') # Non-interactive back-end

app = FastAPI()
templates = Jinja2Templates(directory='templates')
app.mount('/static', StaticFiles(directory='./static'), name='static')

# 1. Root endpoint for GET requests
@app.get("/")
async def read_root(request: Request):
    # 200 OK
    return templates.TemplateResponse('index.html', context={'request': request, 'coef_a': '1', 'coef_b': '0', 'coef_c': '0'})

# 2. Solve endpoint for GET requests
@app.get("/solve")
async def solve_get(request: Request, a: str, b: str, c: str):
    # If number of parameters is more than 3, raise validation exception
    if len(request.query_params) > 3:
        # Raise 422: Unprocessable Entity. The client request contains semantic errors, and the server can't process it.
        raise HTTPException(status_code=422, detail='Too many parameters: a, b, c only expected.')
    # If a or b or c is not a number, raise validation exception
    try:
        a = int(float(a))
        b = int(float(b))
        c = int(float(c))
    except Exception as e:
        # Raise 422: Unprocessable Entity. The client request contains semantic errors, and the server can't process it.
        raise HTTPException(status_code=422, detail=repr(e))
    try:
        roots = solve_equation({'a': a, 'b': b, 'c': c})
        # 200 OK
        return {"roots": roots}
    except Exception as e:
        # Cannot solve equation
        # Raise 500: Internal Server Error
        raise HTTPException(status_code=500, detail=repr(e))

# 3. Solve and plot endpoint for POST requests
@app.post("/")
async def solve_and_plot(request: Request, coef_a: str=Form('1'), coef_b: str=Form('0'), coef_c: str=Form('0')):
    # If a or b or c is not a number, raise validation exception
    try:
        a = int(float(coef_a))
        b = int(float(coef_b))
        c = int(float(coef_c))
    except Exception as e:
        # Raise 422: Unprocessable Entity. The client request contains semantic errors, and the server can't process it.
        raise HTTPException(status_code=422, detail=repr(e))
    if a != 0:
        vertex_x = -b / (2 * a)
    else:
        vertex_x = 0
    try:
        roots = solve_equation({'a': a, 'b': b, 'c': c})
    except Exception as e:
        # Cannot solve equation
        # Raise 500: Internal Server Error
        raise HTTPException(status_code=500, detail=repr(e))
    # Find plotting x range
    # If only one or no roots found, we try to find roots for a similar function which intersects x axis (pseudo-roots)
    if len(roots) == 2: # No pseudo-roots needed, as we already have two points to construct x range
        pseudo_roots = roots
    else:
        try:
            pseudo_roots = solve_equation({'a': np.abs(a), 'b': b, 'c': -2 * (np.abs(c) + 1)})
        except Exception as e:
            # Cannot solve equation
            # Raise 500: Internal Server Error
            raise HTTPException(status_code=500, detail=repr(e))    
    if len(pseudo_roots) == 2:
        x_min = vertex_x - 1.5 * max(np.abs(pseudo_roots[0]), np.abs(pseudo_roots[1]))
        x_max = vertex_x + 1.5 * max(np.abs(pseudo_roots[0]), np.abs(pseudo_roots[1]))
        # Most likely underflow, falling back to some range
        if np.abs(pseudo_roots[0]) == 0 and np.abs(pseudo_roots[1]) == 0:
            x_min, x_max = -10.0, 10.0
    elif len(pseudo_roots) == 1:
        x_min = -1.5 * np.abs(pseudo_roots[0])
        x_max = 1.5 * np.abs(pseudo_roots[0])
        # Most likely underflow, falling back to some range
        if np.abs(pseudo_roots[0]) == 0:
            x_min, x_max = -10.0, 10.0
    else: # Pseudo-roots are not found, falling back to some range
        x_min, x_max = -10.0, 10.0
    print(roots, pseudo_roots, vertex_x)
    # Plotting
    # Quadratic polynomial function
    f = lambda x: a * x ** 2 + b * x + c
    plot_title = format_title(a, b, c)
    xs = np.linspace(x_min, x_max, 100)
    fs = f(xs)
    pngImage = io.BytesIO() # Bytes object to store the image of the plot in memory
    plt.axhline(y=0.0, color='C2', linestyle='--') # x axis
    plt.axvline(x=0.0, color='C2', linestyle='--') # y axis
    plt.plot(xs, fs, color='C0', label='$f(x)$') # function
    plt.plot(vertex_x, f(vertex_x), marker='.', color='C0') # parabola's vertex
    if len(roots) > 0:
        if not isinstance(roots[0], str):
            plt.plot(roots, np.zeros_like(roots), 'o', color='C1', label='Root(s) of $f(x)$') # finite number of roots
        else:
            plt.plot(np.linspace(-10, 10, 25), np.zeros_like([0] * 25), 'o', color='C1', label='Root(s) of $f(x)$') # infinite number of roots
    y_min, y_max = plt.gca().get_ylim()
    # Root(s) labels
    if len(roots) == 2: # labels for two roots
        plt.text(roots[0] - (x_max - x_min) / 30, f(roots[0]) - (y_max - y_min) / 20, '$x_1$', fontsize=10, bbox=dict(boxstyle='circle', alpha=0.75, facecolor='xkcd:white', edgecolor='xkcd:white'))
        plt.text(roots[1] + (x_max - x_min) / 100, f(roots[1]) - (y_max - y_min) / 20, '$x_2$', fontsize=10, bbox=dict(boxstyle='circle', alpha=0.75, facecolor='xkcd:white', edgecolor='xkcd:white'))
    if len(roots) == 1:
        if not isinstance(roots[0], str): # label for one root
            plt.text(roots[0] + (x_max - x_min) / 100, f(roots[0]) - (y_max - y_min) / 20, '$x$', fontsize=10, bbox=dict(boxstyle='circle', alpha=0.75, facecolor='xkcd:white', edgecolor='xkcd:white'))
        else: # label for infinite number of roots
            plt.text((x_max - x_min) / 100, -(y_max - y_min) / 20, 'infinite number of roots')
    if len(roots) == 0: # label for no real roots
        plt.text((x_max - x_min) / 100, -(y_max - y_min) / 30, 'no real roots')
    plt.title(plot_title)
    plt.xlabel('$x$')
    plt.ylabel('$y$')
    plt.legend(loc='upper right')
    plt.tight_layout()
    plt.grid(linewidth=0.5)
    plt.savefig(pngImage) # Save not to a file on disk but to memory
    plt.clf() # clear figure for next plot
    img = base64.b64encode(pngImage.getvalue()).decode('ascii')
    # 200 OK
    return templates.TemplateResponse('index.html', context={'request': request, 'roots': roots, 'coef_a': a, 'coef_b': b, 'coef_c': c, 'image': img})
