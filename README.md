# ESP Manually Approved Requests

## Introduction

What if network requests had to be manually approved? Well, look no further! This project utilizes a front-end interface to send an RGB value to a NodeMCU webserver. Once the webserver receives a request, it will wait for 10 seconds. If the button is pushed within that time, the request is approved and the proper LED is activated. If not, then the request is rejected with a status of 403 (Forbidden).

__Important__: the file contained in `server/` is the code for the NodeMCU microprocessor, _NOT_ a server for the front-end. If your browser has issues sending requests from local files to local IPs, most Unix systems have a built-in webserver via Python (`python -m SimpleHTTPServer` for Python2).


### Why?

I thought it'd be neat! And I wanted to familiarize myself more with JavaScript. For such a simple project, I did go a bit overboard with the JS, but that was the purpose of it. Including the NodeMCU in the ecosystem just made it more interesting to me so that I wouldn't lose focus.

## Demo

You can see a demo [here](https://www.youtube.com/watch?v=ivKMvHVHMCA).
