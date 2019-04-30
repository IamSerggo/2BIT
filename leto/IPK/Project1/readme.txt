HODNOTENIE : zle som precital zadanie, miesto api-key malo byt api_key 
	   : dostal som teda 17/20 (program funkcny na fullku)

!! IPK Project no.1 -- Radovan Babic, xbabic09 !!

Description:
    Light-weight client for OpenWeatherMap API
    Implemented in Python3
    Required input : city and your APIkey
    Makefile for client usage present

Implementation:
    Client is using socket library to establish socket connection with OpenWeather servers
    After connection client sends request to obtain data about weather in selected city
    Response is decoded and parsed using json library
    Information about actual weather, temperature, humidity, pressure, wind speed and direction is printed to stdout
    Client can handle returned function codes and errors while obtaining data

How to use:
    User needs to get his free APIkey from OpenWeather
        How to obtain APIkey:
            1. Register your free account at 'https://home.openweathermap.org/users/sign_up'
            2. After signing up, sign in
            3. Go to 'https://home.openweathermap.org/api_keys' and generate new APIkey
            4. Wait for your APIkey to be confirmed (it can take up to several minutes)
            5. Use your APIkey (you will find all your APIkeys at link mentioned in step 3.)

    Client is started using included Makefile
        How to use Makefile and client:
            1. Select a city which information you want to obtain
            2. Prepare your APIkey
            3. Open terminal in client location and use 'make run' option

        How to use 'make run':
            Type 'make run api-key=<YOURAPIKEY> city=<YOURCITY>' into terminal
