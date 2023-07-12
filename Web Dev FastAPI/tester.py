import requests

coeffs = input('Enter space-separated coefficients: ')

params = dict(zip(['a', 'b', 'c'], coeffs.split()))

url = 'http://127.0.0.1:8000/solve'

response = requests.get(url, params=params)

print('Response status:', response.status_code)
print('Response content-type:', response.headers['content-type'])
print('Response body:', response.json())
