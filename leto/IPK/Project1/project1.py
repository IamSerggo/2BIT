# -*- coding: utf-8 -*-
"""
File : First project for IPK - Client for OpenWeatherMap API
Author : Radovan Babic, xbabic09
School : VUT FIT
Year of study : 2BIT, 2018/2019
"""

import re
import sys
import json
import socket

def sendRequest(city, APIkey):
    """
    Sending GET request to OpenWeatherMap servers

    @param City to check weather for
    @param User APIkey

    @return Response from server
    """

    host = 'api.openweathermap.org'
    port = 80
    requestData = '/data/2.5/weather?q='+str(city)+'&APPID='+str(APIkey)
    request = 'GET '+requestData+' HTTP/1.1\r\nHost: '+host+'\r\n\r\n'
    requestInBytes = str.encode(request)

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as soc:
        soc.connect( (host, port) )
        soc.sendall(requestInBytes)
        response = soc.recv(2048)
        soc.close()

    return response

def parseData(city, APIkey):
    """
    Parsing data returned by sendRequest function
    Return code is analyzed and possible errors are handled

    @param City to check weather for
    @param User APIkey

    @return Parsed response from server
    """

    response = sendRequest(city, APIkey)
    decodedResponse = response.decode('utf-8')

    functionCode = re.findall(r'\d\d\d', decodedResponse)[0]
    if ( functionCode != '200' ):
        if ( functionCode == '401' ):
            print("Invalid API key!\n")
            sys.exit(1)
        elif ( functionCode == '404' ):
            print("City not found!\n")
            sys.exit(1)
        else:
            print("Unknown error occurred!\n")
            sys.exit(1)

    parsedResponse = json.loads(decodedResponse.split("\r\n\r\n")[-1])

    weather = parsedResponse['weather'][0]
    main = parsedResponse['main']
    wind = parsedResponse['wind']

    return [weather, main, wind]

def printData(city, APIkey):
    """
    Printing parsed data returned by function parseData
    Data printings are in try/except blocks for possible errors handling

    @param City to check weather for
    @param User APIkey

    @return void
    """

    data = parseData(city, APIkey)

    weather = data[0]
    main = data[1]
    wind = data[2]

    print("\n"+city)

    try:
        print(weather['description'])
    except:
        print("Weather description data not received!")

    try:
        temperature = main['temp'] - 273.15
        print("temp:" + str( "%.1f" % temperature ) +"°C")
    except:
        print("Temperature data not received!")

    try:
        print("humidity:"+ str(main['humidity']) +"%")
    except:
        print("Humidity data not received!")

    try:
        print("pressure:"+ str(main['pressure']) +"hPa")
    except:
        print("Pressure data not received!")

    try:
        print("wind-speed:"+ str(wind['speed']) +"km/h")
    except:
        print("Wind speed data not received!")

    try:
        print("wind-deg:"+ str(wind['deg']) )
    except:
        print("Wind direction data not received!\n")

def main():
    """
    Main funciton for program running
    Checking correctness of program parameters

    @return void
    """

    if ( len(sys.argv) != 3 ):
        print("Wrong number of arguments!\n")
        sys.exit(1)
    elif ( not sys.argv[1].startswith("api_key=") or sys.argv[1] == "api-key=" ):
        print("Wrong API key argument format!\n api-key=<yourapikey>\n")
        sys.exit(1)
    elif ( not sys.argv[2].startswith("city=") or sys.argv[2] == "city=" ):
        print("Wrong city argument format!\n city=<cityname>\n")
        sys.exit(1)
    else:
        APIkey = str(sys.argv[1])
        city = str(sys.argv[2])
        printData(city.replace("city=", ""), APIkey.replace("api-key=", ""))

if __name__ == "__main__":
    main()
