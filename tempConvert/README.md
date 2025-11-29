# Temperature Converter — C++ backend + static frontend

Small learning project demonstrating a minimal C++ HTTP backend and a static frontend.

## What this does
- A tiny C++ program exposes an HTTP endpoint that converts temperatures between Celsius and Fahrenheit.
- A simple static frontend (HTML/CSS) can call the backend to show conversions in the browser.
- The project is intended for learning how to build a C++ server and front it with nginx.

## Repository layout
- `convert`           - compiled executable (if present)
- `convert.cpp`       - C++ source for the HTTP backend
- `httplib.h`         - single-file header HTTP library used by `convert.cpp`
- `index.html`        - simple frontend page
- `style.css`         - styles for the frontend

## Build
1. Compile using g++:

   g++ convert.cpp -o convert

2. Run the server:

   ./convert

## Usage
- Open `index.html` in a browser and use the UI to request conversions from the backend.
- Alternatively, call the HTTP endpoint directly. Typical query formats (check `convert.cpp` for exact paths):

  GET /convert?value=100&from=C&to=F

This will return the converted temperature in the response body.

### nginx (To set the frontend to a certain port):
- To place nginx in front of the C++ server, create a proxy pass rule. Example snippet for `/etc/nginx/sites-enabled/default`:
```
  server {
      listen 80;
      server_name _;
      
      root /path/to/tempConvert;           # Your project root
      index index.html;
    
      location / {
          try_files $uri $uri/ =404;
      }

      location /convert/ {
          proxy_pass http://127.0.0.1:5000/;  # adjust port if needed
      }
  }
```
Notes
- The project uses a single-file HTTP header (`httplib.h`) as a tiny dependency; no package manager is required.
- If you change ports or endpoints, update `index.html` or the nginx config accordingly.
- This project is intentionally minimal for learning; it omits production concerns such as security, logging, and robustness.
