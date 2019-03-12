# TraceSystem
Simple C++ header to use to measure easyly time functions with multithread capability using chrome://tracing/ as external tool (browser Google Chrome)

## GENIe as generator tool
This repo uses [GENIe](https://github.com/bkaradzic/GENie) to create automagically a lot of soultions for differents platforms or IDEs. Go to the page for more info: [GENIe linkpage](https://github.com/bkaradzic/GENie)
## What is TraceSystem
Is a single header that generates a .json through a simple c++ MACRO that can be used to have a nanosecond measurement and a gui interpretation using Google Chrome and access to [chrome://tracing/](chrome://tracing/) .
For more info visit: [Chrome Tracing doc](https://www.chromium.org/developers/how-tos/trace-event-profiling-tool)
![alt text](https://a77db9aa-a-7b23c8ea-s-sites.googlegroups.com/a/chromium.org/dev/developers/how-tos/trace-event-profiling-tool/abouttracing2.png?attachauth=ANoY7cqfwQ1kZBSfaIGKVDD7spaQ6CMGv5zfPRn0Bd_ozk6C_uqeNerEh90O9lErdX8kjpoXFqzJgbvK9rRqysWQDj1aW9usriydXnK3rBtLag3UphrEKci5FWKQHREktb991oyjvaM1VT1SyOGUGnIWw9cvhHRUhpplD2AMuFm6gElK2FSyHOgktsSluUNyz5dUGbADvfRFbWMfnAqnZ4HIZMxDAJ1dfGC3aFvADUj090B383c-YzOaO6L15zdQ5naoAdo2jUk6pcK4rfHYvYUVf0_WjU-WJA%3D%3D&attredirects=0 "Chrome")
![alt text](https://github.com/TesterPointer/TraceSystem/blob/master/captures/zoom_demo.png "Test Multythreading")
## Dependencies
TraceSystem has not C++ dependencies, only C++11 support. To use the GUI interpreter you need to use Google Chrome
## HOW TO BUILD
### Visual Studio
There is .bat file called vs2015/17, just execute it or use it to have a reference how I create the solution.
#### Posbile bug using Visual Studio
[GENIe](https://github.com/bkaradzic/GENie) doesnt support at the moment Windows SDK finding system, maybe I use a different SDK version, the only thing you have to do is "Retarget the solution" (right-click on the solution, in this case "TraceSystem" INSIDE VISUAL STUDIO and select your SDK solution)
![alt text](https://github.com/TesterPointer/TraceSystem/blob/master/captures/buf_fix1.png "Bug fix 1")
![alt text](https://github.com/TesterPointer/TraceSystem/blob/master/captures/buf_fix2.png "Bug fix 2")
### Others
Try to launch genie executable in the same folder system and you will see that there is a lot of options to generate the projects

