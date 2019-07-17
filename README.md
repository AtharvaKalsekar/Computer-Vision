# Computer-Vision
All the projects here are related to computer vision and image processing.These programs are developed on Microsoft Visual Studio 2017 IDE (Communtiy version) and using OpenCV library with C++ as the programming language.These programs can also be deployed on Raspberry pi 3 (raspbian OS - Jessie) and windows 10.

##### Object Tracking (HSV method) :
1.  The webcam camera-feed is displayed along with a trackbar window, HSV camera-feed and a window for binary form of the camera-feed.
2.  Keep the object to be tracked (ball in my case) in front of the webcam and adjust the trackbars until you get a clear distinct binary form of the camera-feed of your object.
3.  Now you will se the object being tracked and the co-ordinates of it's center displayed in the main camera-feed. 

##### Object Tracking (Background subtraction method) :
This method can only be used to detect and track a moving object
1.  The webcam camera-feed is displayed along with a window for binary form of the camera-feed.
2.  When the object to be tracked (ball in my case) has a movement in the camera-feed then it is tracked until it ceases to move.
3.  Now you will se the object being tracked in the main camera-feed.

##### Color Detection :
The colour that we are going to detecect should be any among these 3 - Red, Blue, Gold.
1.  The webcam camera-feed is displayed along with a square in it as a ROI.
2.  The color of the object to be detected has to placed such that it lies inside the ROI in the camera-feed.
3.  After suitable positioning of the object, press the 'a' key on the keyboard.
4.  Now you will get to see 5 image windows consisting only the image of the ROI which was croped and other window "colour extract" showing the colour pixels that were recongnized as to belong to a particular color range that was programmed.
5.  The sum of area of the colour in each croped ROI and area of all of the previous crops is printed in the console.The colour detected is also printed. 'a' = Gold, 'b'=Blue, 'r'=Red.

##### Shape Detection :
The name of the shapes can be only among these 3 - Triangle, Square, Pentagon else it will be circle if number of vertices comes out to be greater than 5.

1.  The webcam camera-feed is displayed along with a window for binary form of the camera-feed and a blurred form of the camera-feed.
2.  When the object (white in color) whose shape is to be detected is placed in front of the wbecam, it's binary form is seen in the camerafeed.
3.  Press the key 'a' on the keyboard. This will open 3 new windows -
    ( a ) RefImg- Consisting of the image taken at the instant key 'a' was pressed.
    ( b ) Edges- Consisting of the edges of objects in the RefImg.
    ( c ) Approx Poly- Cosisting of the approximated shape of the edges from the image in the "Edges" window.
4. The number of vertices and the name of the shape for each approximated shape in "Approx Poly" window is printed in the console.

##### Size Measurement :
This method can only be used to detect and track a moving object
1.  The webcam camera-feed is displayed.
2.  The object whose size is to be determined is kept on a plain surface along with a bottle cap (my reference object) such that there are no edges other than that of the bottel cap and the object in the camera-feed and the bottle cap has to pe placed such that it is leftmost in the camera-feed.
3.  Press the 'a' key on the keyboard.This will open 3 new windows-
    ( a ) RefImg- Consisting of the image taken at the instant key 'a' was pressed.
    ( b ) Edges- Consisting of the edges of objects in the RefImg.
    ( c ) Measurements- Consisting of the objects surrounded by a bounding box and the height and breadth being displayed in the bottom right corner of the bounding box.
