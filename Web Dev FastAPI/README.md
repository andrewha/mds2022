This is a simple web app that can solve quadratic and linear equations.
It serves both GET and POST methods.
The app is a one page app, where you can fill the form with the coefficients of your polynomial and send a POST request, 
and then the server will send a response with the found roots back to the same page.
The app also plots a graph of your polynomial and shows the roots, if any.

To run the app:
- ```pip install -r requirements.txt```
- ```uvicorn main:app```
- (optionally) run ```python tester.py``` to manually send GET requests
