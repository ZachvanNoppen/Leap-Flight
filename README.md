# Leap-Flight

##TODO
- So far I just have to get the wing flapping working. I know how to do it, but it might take some adjusting so here's the version without it so you guys can get started on it.
- There is no range on where the bird can fly but you guys can add that when you do the sensor out of bounds test (like in class)

#Controls
##WASD for moving the bird on screen 
Right now none of it interpolates since I figured that may be a pain to debug with. It messed me up when doing the camera at the start so we can add it later

##UP/DOWN for camera movement



#How it all works

Without you going into the code, basically I just put the camera in place and move all the objects around it. That way I didn't have to mess with camera movement.
In hindsight that might not have been that hard. All object in openframeworks inherit ofNode (https://openframeworks.cc/documentation/3d/ofNode/#show_setGlobalOrientation)
and it's really easy to transform/parent object. Other than that I just threw a light in and we have a demo type thing. 


